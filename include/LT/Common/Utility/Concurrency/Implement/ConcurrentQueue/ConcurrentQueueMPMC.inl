using namespace LT;

template<typename T>
ConcurrentQueue<T, MPMC>::ConcurrentQueue(size_t _size)
{
	size_t cap = 1;
	while (cap < _size) cap <<= 1;

	m_mask = (m_capacity = cap) - 1;
	m_buffer = std::make_unique<Cell[]>(m_capacity);

	for (int i = 0; i < m_capacity; i++)
		m_buffer[i].m_sequence = i;
}
template<typename T>
bool ConcurrentQueue<T, MPMC>::Enqueue(const T* __restrict _src)
{
	size_t t = m_tail.value.load(std::memory_order_acquire);

	size_t idx = t & m_mask;
	size_t sequence = m_buffer[idx].m_sequence.load(std::memory_order_relaxed);
	int64_t difference =
		static_cast<int64_t>(sequence) -
		static_cast<int64_t>(t);

	auto UpdateCellState = [&]()
		{
			idx = t & m_mask;
			sequence = m_buffer[idx].m_sequence.load(std::memory_order_relaxed);
			difference =
				static_cast<int64_t>(sequence) -
				static_cast<int64_t>(t);
		};

	while (true)
	{
		if (difference < 0) return false;
		else if (difference == 0)
		{
			if (!m_tail.value.compare_exchange_weak(t, t + 1,
				std::memory_order_acq_rel,
				std::memory_order_acquire))
			{
				UpdateCellState();
				continue;
			}

			if constexpr (std::is_trivially_copyable_v<T>)
				memcpy(&m_buffer[idx].m_data, _src, sizeof(T));
			else
				m_buffer[idx].m_data = std::move(*_src);

			m_buffer[idx].m_sequence.store(
				t + 1, 
				std::memory_order_release);
			return true;
		}
		else
		{
			t = m_tail.value.load(std::memory_order_relaxed);
			UpdateCellState();
			continue;
		}
	}
}

template<typename T>
bool ConcurrentQueue<T, MPMC>::Dequeue(T* __restrict _dest)
{
	size_t h = m_head.load(std::memory_order_acquire);
	
	size_t idx = h & m_mask;
	size_t sequence = m_buffer[idx].m_sequence.load(std::memory_order_relaxed);
	int64_t difference = 
		static_cast<int64_t>(sequence) -
		static_cast<int64_t>(h + 1);

	auto UpdateCellState = [&]()
		{
			idx = h & m_mask;
			sequence = m_buffer[idx].m_sequence.load(std::memory_order_relaxed);
			difference =
				static_cast<int64_t>(sequence) -
				static_cast<int64_t>(h + 1);
		};

	while (true)
	{
		if (difference < 0) return false;
		else if (difference == 0)
		{
			if (!m_head.value.compare_exchange_weak(h, h + 1,
				std::memory_order_acq_rel,
				std::memory_order_acquire))
			{
				UpdateCellState();
				continue;
			}

			if constexpr (std::is_trivially_copyable_v<T>)
				memcpy(_dest, &m_buffer[idx].m_data, sizeof(T));
			else
				*_dest = std::move(m_buffer[idx].m_data);

			m_buffer[idx].m_sequence.store(
				h + m_capacity,
				std::memory_order_release);
			return true;

		}
		else
		{
			h = m_head.value.load(std::memory_order_relaxed);
			UpdateCellState();
			continue;
		}
	}
}

template<typename T>
bool ConcurrentQueue<T, MPMC>::TryDequeue(T* __restrict _dest)
{
	size_t h = m_head.value.load(std::memory_order_acquire);

	size_t idx = h & m_mask;
	size_t sequence = m_buffer[idx].m_sequence.load(std::memory_order_relaxed);
	int64_t difference =
		static_cast<int64_t>(sequence) -
		static_cast<int64_t>(h + 1);

	auto UpdateCellState = [&]()
		{
			idx = h & m_mask;
			sequence = m_buffer[idx].m_sequence.load(std::memory_order_relaxed);
			difference =
				static_cast<int64_t>(sequence) -
				static_cast<int64_t>(h + 1);
		};

	while (true)
	{
		if (difference == 0)
		{
			if (!m_head.value.compare_exchange_strong(h, h + 1,
				std::memory_order_acq_rel,
				std::memory_order_acquire)) return false;

			if constexpr (std::is_trivially_copyable_v<T>)
				memcpy(_dest, &m_buffer[idx].m_data, sizeof(T));
			else
				*_dest = std::move(m_buffer[idx].m_data);

			m_buffer[idx].m_sequence.store(
				h + m_capacity,
				std::memory_order_release);
			return true;

		}
		else return false;
	}
}

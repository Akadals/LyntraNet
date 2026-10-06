using namespace LT;

template<typename T>
ConcurrentStack<T, MPMC>::ConcurrentStack(size_t _size)
{
	size_t cap = 1;
	while (cap < _size) cap <<= 1;

	m_mask = (m_capacity = cap) - 1;
	m_buffer = std::make_unique<T[]>(m_capacity);

	for (int i = 0; i < m_capacity; i++)
		m_buffer[i].m_sequence = i;
}

template<typename T>
bool ConcurrentStack<T, MPMC>::Push(const T& _data)
{
	size_t t = m_top.value.load(std::memory_order_relaxed);

	size_t sequence = m_buffer[t].m_sequence.load(std::memory_order_relaxed);
	int64_t difference =
		static_cast<int64_t>(sequence) -
		static_cast<int64_t>(t);

	auto UpdateCellState = [&]()
		{
			sequence = m_buffer[t].m_sequence.load(std::memory_order_relaxed);
			difference =
				static_cast<int64_t>(sequence) -
				static_cast<int64_t>(t);
		};

	while (true)
	{
		if (difference < 0) return false;
		else if (difference == 0)
		{
			if (!m_top.value.compare_exchange_weak(t, t + 1,
				std::memory_order_acq_rel,
				std::memory_order_acquire))
			{
				UpdateCellState();
				continue;
			}

			if constexpr (std::is_trivially_copyable_v<T>)
				memcpy(&m_buffer[t], &_data, sizeof(T));
			else
				m_buffer[t] = _data;

			m_buffer[t].m_sequence.store(
				t + 1,
				std::memory_order_release);
			return true;
		}
		else
		{
			t = m_top.value.load(std::memory_order_relaxed);
			UpdateCellState();
			continue;
		}
	}
}

template<typename T>
bool ConcurrentStack<T, MPMC>::Pop(T& _data)
{
	size_t t = m_top.value.load(std::memory_order_acquire);

	if (t-- == 0) return false;

	if constexpr (std::is_trivially_copyable_v<T>)
		memcpy(&_data, &m_buffer[t], sizeof(T));
	else
		_data = m_buffer[t];

	m_top.value.store(t, std::memory_order_release);
	return true;
}

template<typename T>
void ConcurrentStack<T, MPMC>::Top(T* _data)
{
	size_t t = m_top.value.load(std::memory_order_acquire);

	if (t-- == 0) return;

	if constexpr (std::is_trivially_copyable_v<T>)
		memcpy(&_data, &m_buffer[t], sizeof(T));
	else
		_data = m_buffer[t];
}
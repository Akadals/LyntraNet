using namespace LT;

template<typename T>
ConcurrentQueue<T, SPSC>::ConcurrentQueue(size_t _size)
{
	size_t cap = 1;
	while (cap < _size) cap <<= 1;

	m_mask = (m_capacity = cap) - 1;
	m_buffer = std::make_unique<T[]>(m_capacity);
}

template<typename T>
bool ConcurrentQueue<T, SPSC>::Enqueue(const T* __restrict _src)
{
	size_t h = m_head.value.load(std::memory_order_acquire);
	size_t t = m_tail.value.load(std::memory_order_relaxed);

	if (t - h >= m_capacity) return false;

	size_t idx = t & m_mask;

	if constexpr (std::is_trivially_copyable_v<T>)
		memcpy(&m_buffer[idx], _src, sizeof(T));
	else
		m_buffer[idx] = std::move(*_src);

	m_tail.value.store(t + 1, std::memory_order_release);

	return true;
}

template<typename T>
bool ConcurrentQueue<T, SPSC>::Dequeue(T* __restrict _dest)
{
	size_t h = m_head.value.load(std::memory_order_relaxed);
	size_t t = m_tail.value.load(std::memory_order_acquire);

	if (t - h == 0) return false;

	size_t idx = h & m_mask;

	if constexpr (std::is_trivially_copyable_v<T>)
		memcpy(_dest, &m_buffer[idx], sizeof(T));
	else
		*_dest = std::move(m_buffer[idx]);

	m_head.value.store(h + 1, std::memory_order_release);

	return true;
}
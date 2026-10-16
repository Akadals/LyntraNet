using namespace LT;

template<typename T>
ConcurrentStack<T, SPSC>::ConcurrentStack(size_t _size)
{
	m_capacity = _size;
	m_buffer = std::make_unique<T[]>(m_capacity);
}

template<typename T>
bool ConcurrentStack<T, SPSC>::Push(const T& _data)
{
	size_t t = m_top.value.load(std::memory_order_relaxed);

	if (t >= m_capacity) return false;

	if constexpr (std::is_trivially_copyable_v<T>)
		memcpy(&m_buffer[t], &_data, sizeof(T));
	else
		m_buffer[t] = _data;

	m_top.value.store(t + 1, std::memory_order_release);
	return true;
}

template<typename T>
bool ConcurrentStack<T, SPSC>::Pop(T& _data)
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
void ConcurrentStack<T, SPSC>::Top(T* _data)
{
	size_t t = m_top.value.load(std::memory_order_acquire);

	if (t-- == 0) return;

	if constexpr (std::is_trivially_copyable_v<T>)
		memcpy(&_data, &m_buffer[t], sizeof(T));
	else
		_data = m_buffer[t];
}
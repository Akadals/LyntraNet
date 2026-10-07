using namespace LT;

template<typename T>
ConcurrentStack<T, SPMC>::ConcurrentStack(size_t _size)
{
	size_t cap = 1;
	while (cap < _size) cap <<= 1;

	m_mask = (m_capacity = cap) - 1;
	m_buffer = std::make_unique<T[]>(m_capacity);
}
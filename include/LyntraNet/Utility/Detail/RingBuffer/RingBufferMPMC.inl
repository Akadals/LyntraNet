using namespace LT;

RingBuffer<MPMC>::RingBuffer<MPMC>(size_t _size)
{
	if (_size == 0) throw;

	size_t cap = 1;
	while (cap < _size) cap <<= 1;

	m_mask = (m_capacity = cap) - 1;
	m_buffer = std::make_unique<std::byte[]>(m_capacity);
}

bool RingBuffer<MPMC>::TryWrite(const std::byte* __restrict _src, size_t _len)
{
	return false;
}
bool RingBuffer<MPMC>::TryWrite(std::span<const std::byte> _src)
{
	return false;
}

bool RingBuffer<MPMC>::TryRead(std::byte* __restrict _dest, size_t _len)
{
	return false;
}
bool RingBuffer<MPMC>::TryRead(std::span<std::byte> _dest)
{
	return false;
}

void RingBuffer<MPMC>::ReadPreview(std::byte* __restrict _dest, size_t _len) const
{

}
void RingBuffer<MPMC>::ReadPreview(std::span<std::byte> _dest) const
{

}

void RingBuffer<MPMC>::Clear()
{

}

size_t RingBuffer<MPMC>::Size() const noexcept
{
	return 0;
}
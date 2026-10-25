using namespace LT;

RingBuffer<SPSC>::RingBuffer<SPSC>(size_t _size)
{
	if (_size == 0) throw;

	size_t cap = 1;
	while (cap < _size) cap <<= 1;

	m_mask = (m_capacity = cap) - 1;
	m_buffer = std::make_unique<std::byte[]>(m_capacity);
}
RingBuffer<SPSC>::~RingBuffer() {}

template<size_t ByteSize>
bool RingBuffer<SPSC>::TryWrite(const std::byte* __restrict _src)
{
	static_assert(ByteSize > 0);
	if constexpr (ByteSize > 256)
		return TryWrite(_src, ByteSize);
	else
	{
		size_t h = m_head.value.load(std::memory_order_acquire);
		size_t t = m_tail.value.load(std::memory_order_relaxed);

		if ((t + ByteSize) - h > m_capacity) [[unlikely]]
			return false;

		size_t idx = t & m_mask;

		const size_t remaining = m_capacity - idx;

		std::byte* buffer = m_buffer.get();

		if (remaining >= ByteSize)
			Memory::Copy<ByteSize>(buffer + idx, _src);
		else [[unlikely]]
		{
			Memory::Copy(buffer + idx, _src, remaining);
			Memory::Copy(buffer, _src + remaining, ByteSize - remaining);
		}

		m_tail.value.store(t + ByteSize, std::memory_order_release);

		return true;
	}
}

bool RingBuffer<SPSC>::TryWrite(const std::byte* __restrict _src, size_t _len)
{
	if (_len == 0) [[unlikely]]
		return true;

	size_t h = m_head.value.load(std::memory_order_acquire);
	size_t t = m_tail.value.load(std::memory_order_relaxed);

	if ((t + _len) - h > m_capacity) [[unlikely]]
		return false;

	size_t idx = t & m_mask;

	const size_t remaining = m_capacity - idx;
	size_t first = _len < remaining ? _len : remaining;

	std::byte* buffer = m_buffer.get();

	Memory::Copy(buffer + idx, _src, first);
	if (_len > remaining) [[unlikely]]
		Memory::Copy(buffer, _src + first, _len - first);

	m_tail.value.store(t + _len, std::memory_order_release);

	return true;
}

bool RingBuffer<SPSC>::TryWrite(std::span<const std::byte> _src)
{
	size_t h = m_head.value.load(std::memory_order_acquire);
	size_t t = m_tail.value.load(std::memory_order_relaxed);

	size_t len = _src.size();

	if ((t + len) - h > m_capacity) [[unlikely]]
		return false;

	size_t idx = t & m_mask;

	const size_t remaining = m_capacity - idx;
	size_t first = len < remaining ? len : remaining;

	std::byte* buffer = m_buffer.get();
	const std::byte* src = _src.data();

	Memory::Copy(buffer + idx, src, first);
	if (len > remaining) [[unlikely]]
		Memory::Copy(buffer, src + first, len - first);

	m_tail.value.store(t + len, std::memory_order_release);

	return true;
}

template<size_t ByteSize>
bool RingBuffer<SPSC>::TryRead(std::byte* __restrict _dest)
{
	static_assert(ByteSize > 0);
	if constexpr (ByteSize > 256)
		return TryRead(_dest, ByteSize);
	else
	{
		size_t h = m_head.value.load(std::memory_order_relaxed);
		size_t t = m_tail.value.load(std::memory_order_acquire);

		if (t - h < ByteSize) [[unlikely]]
			return false;

		size_t idx = h & m_mask;

		const size_t remaining = m_capacity - idx;

		std::byte* buffer = m_buffer.get();

		if (remaining >= ByteSize)
			Memory::Copy<ByteSize>(_dest, buffer + idx);
		else [[unlikely]]
		{
			Memory::Copy(_dest, buffer + idx, remaining);
			Memory::Copy(_dest + remaining, buffer, ByteSize - remaining);
		}

		m_head.value.store(h + ByteSize, std::memory_order_release);

		return true;
	}
}

bool RingBuffer<SPSC>::TryRead(std::byte* __restrict _dest, size_t _len)
{
	if (_len == 0) [[unlikely]]
		return true;

	size_t h = m_head.value.load(std::memory_order_relaxed);
	size_t t = m_tail.value.load(std::memory_order_acquire);

	if (t - h < _len) [[unlikely]]
		return false;

	size_t idx = h & m_mask;

	const size_t remaining = m_capacity - idx;
	size_t first = _len < remaining ? _len : remaining;
	
	std::byte* buffer = m_buffer.get();

	Memory::Copy(_dest, buffer + idx, first);
	if (_len > remaining) [[unlikely]]
		Memory::Copy(_dest + first, buffer, _len - first);

	m_head.value.store(h + _len, std::memory_order_release);

	return true;
}

bool RingBuffer<SPSC>::TryRead(std::span<std::byte> _dest)
{
	size_t h = m_head.value.load(std::memory_order_relaxed);
	size_t t = m_tail.value.load(std::memory_order_acquire);

	size_t len = _dest.size();

	if (t - h < len) [[unlikely]]
		return false;

	size_t idx = h & m_mask;

	const size_t remaining = m_capacity - idx;
	size_t first = len < remaining ? len : remaining;

	std::byte* buffer = m_buffer.get();
	std::byte* dest = _dest.data();

	Memory::Copy(dest, buffer + idx, first);
	if (len > remaining) [[unlikely]]
		Memory::Copy(dest + first, buffer, len - first);

	m_head.value.store(h + len, std::memory_order_release);

	return true;
}

template<size_t ByteSize>
void RingBuffer<SPSC>::ReadPreview(std::byte* __restrict _dest) const
{
	static_assert(ByteSize > 0);
	if constexpr (ByteSize > 256)
		return ReadPreview(_dest, ByteSize);
	else
	{

		size_t h = m_head.value.load(std::memory_order_relaxed);
		size_t t = m_tail.value.load(std::memory_order_acquire);

		if (t - h < ByteSize) [[unlikely]]
			return;

		size_t idx = h & m_mask;

		const size_t remaining = m_capacity - idx;

		std::byte* buffer = m_buffer.get();

		if (remaining >= ByteSize)
			Memory::Copy<ByteSize>(_dest, buffer + idx);
		else [[unlikely]]
		{
			Memory::Copy(_dest, buffer + idx, remaining);
			Memory::Copy(_dest + remaining, buffer, ByteSize - remaining);
		}
	}
}

void RingBuffer<SPSC>::ReadPreview(std::byte* __restrict _dest, size_t _len) const
{
	if (_len == 0) [[unlikely]]
		return;

	size_t h = m_head.value.load(std::memory_order_relaxed);
	size_t t = m_tail.value.load(std::memory_order_acquire);

	if (t - h < _len) [[unlikely]]
		return;

	size_t idx = h & m_mask;

	const size_t remaining = m_capacity - idx;
	size_t first = _len < remaining ? _len : remaining;

	std::byte* buffer = m_buffer.get();

	Memory::Copy(_dest, buffer + idx, first);
	if (_len > remaining) [[unlikely]]
		Memory::Copy(_dest + first, buffer, _len - first);
}

void RingBuffer<SPSC>::ReadPreview(std::span<std::byte> _dest) const
{
	size_t h = m_head.value.load(std::memory_order_relaxed);
	size_t t = m_tail.value.load(std::memory_order_acquire);

	size_t len = _dest.size();

	if (t - h < len) [[unlikely]]
		return;

	size_t idx = h & m_mask;

	const size_t remaining = m_capacity - idx;
	size_t first = len < remaining ? len : remaining;

	std::byte* buffer = m_buffer.get();
	std::byte* dest = _dest.data();

	Memory::Copy(dest, buffer + idx, first);
	if (len > remaining) [[unlikely]]
		Memory::Copy(dest + first, buffer, len - first);
}

void RingBuffer<SPSC>::Clear()
{
	size_t t = m_tail.value.load(std::memory_order_relaxed);
	m_head.value.store(t, std::memory_order_relaxed);
}

size_t RingBuffer<SPSC>::Size() const noexcept
{
	const size_t t = m_tail.value.load(std::memory_order_relaxed);
	const size_t h = m_head.value.load(std::memory_order_relaxed);
	return t - h;
}
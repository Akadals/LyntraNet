using namespace LT;

RingBuffer<FastSPSC>::RingBuffer(size_t _size)
{
	if (_size == 0) throw;

	size_t cap = 1;
	while (cap < _size) cap <<= 1;

	m_mask = (m_capacity = cap) - 1;
	m_buffer = std::make_unique<std::byte[]>(m_capacity);
}

template<size_t ByteSize>
bool RingBuffer<FastSPSC>::TryWrite(const std::byte* __restrict _src)
{
	static_assert(ByteSize > 0);
	if constexpr (ByteSize > 256)
		return TryWrite(_src, ByteSize);
	else
	{
		size_t t = m_producer.value;

		if ((t + ByteSize) - m_producer.cachedValue > m_capacity) [[unlikely]]
		{
			m_producer.cachedValue = m_consumer.value;
			if ((t + ByteSize) - m_producer.cachedValue > m_capacity) [[unlikely]]
				return false;
		}

		const size_t idx = t & m_mask;
		const size_t remaining = m_capacity - idx;

		std::byte* buffer = m_buffer.get();

		if (remaining >= ByteSize)
			Memory::Copy<ByteSize>(buffer + idx, _src);
		else [[unlikely]]
		{
			Memory::Copy(buffer + idx, _src, remaining);
			Memory::Copy(buffer, _src + remaining, ByteSize - remaining);
		}

		m_producer.value += ByteSize;

		return true;
	}
}

bool RingBuffer<FastSPSC>::TryWrite(const std::byte* __restrict _src, size_t _len)
{
	if (_len == 0) [[unlikely]]
		return true;

	size_t t = m_producer.value;

	if ((t + _len) - m_producer.cachedValue > m_capacity) [[unlikely]]
	{
		m_producer.cachedValue = m_consumer.value;
		if ((t + _len) - m_producer.cachedValue > m_capacity) [[unlikely]]
			return false;
	}

	const size_t idx = t & m_mask;
	const size_t remaining = m_capacity - idx;

	size_t first = _len < remaining ? _len : remaining;

	std::byte* buffer = m_buffer.get();

	Memory::Copy(buffer + idx, _src, first);
	if (_len > remaining) [[unlikely]]
		Memory::Copy(buffer, _src + first, _len - first);

	m_producer.value += _len;

	return true;
}

bool RingBuffer<FastSPSC>::TryWrite(std::span<const std::byte> _src)
{
	size_t len = _src.size();

	size_t t = m_producer.value;

	if ((t + len) - m_producer.cachedValue > m_capacity) [[unlikely]]
	{
		m_producer.cachedValue = m_consumer.value;
		if ((t + len) - m_producer.cachedValue > m_capacity) [[unlikely]]
			return false;
	}

	const size_t idx = t & m_mask;
	const size_t remaining = m_capacity - idx;

	size_t first = len < remaining ? len : remaining;

	std::byte* buffer = m_buffer.get();
	const std::byte* src = _src.data();

	Memory::Copy(buffer + idx, src, first);
	if (len > remaining) [[unlikely]]
		Memory::Copy(buffer, src + first, len - first);

	m_producer.value += len;

	return true;
}

template<size_t ByteSize>
bool RingBuffer<FastSPSC>::TryRead(std::byte* __restrict _dest)
{
	static_assert(ByteSize > 0);
	if constexpr (ByteSize > 256)
		return TryRead(_dest, ByteSize);
	else
	{
		size_t h = m_consumer.value;

		if (m_consumer.cachedValue - h < ByteSize) [[unlikely]]
		{
			m_consumer.cachedValue = m_producer.value;
			if (m_consumer.cachedValue - h < ByteSize) [[unlikely]]
				return false;
		}

		const size_t idx = h & m_mask;
		const size_t remaining = m_capacity - idx;

		std::byte* buffer = m_buffer.get();

		if (remaining >= ByteSize)
			Memory::Copy<ByteSize>(_dest, buffer + idx);
		else [[unlikely]]
		{
			Memory::Copy(_dest, buffer + idx, remaining);
			Memory::Copy(_dest + remaining, buffer, ByteSize - remaining);
		}

		m_consumer.value += ByteSize;
		return true;
	}
}

bool RingBuffer<FastSPSC>::TryRead(std::byte* __restrict _dest, size_t _len)
{
	if (_len == 0) [[unlikely]]
		return true;

	size_t h = m_consumer.value;

	if (m_consumer.cachedValue - h < _len) [[unlikely]]
	{
		m_consumer.cachedValue = m_producer.value;
		if (m_consumer.cachedValue - h < _len) [[unlikely]]
			return false;
	}

	const size_t idx = h & m_mask;
	const size_t remaining = m_capacity - idx;

	size_t first = _len < remaining ? _len : remaining;
	
	std::byte* buffer = m_buffer.get();

	Memory::Copy(_dest, buffer + idx, first);
	if (_len > remaining) [[unlikely]]
		Memory::Copy(_dest + first, buffer, _len - first);

	m_consumer.value += _len;

	return true;
}

bool RingBuffer<FastSPSC>::TryRead(std::span<std::byte> _dest)
{
	size_t len = _dest.size();

	size_t h = m_consumer.value;

	if (m_consumer.cachedValue - h < len) [[unlikely]]
	{
		m_consumer.cachedValue = m_producer.value;
		if (m_consumer.cachedValue - h < len) [[unlikely]]
			return false;
	}

	const size_t idx = h & m_mask;
	const size_t remaining = m_capacity - idx;

	size_t first = len < remaining ? len : remaining;

	std::byte* buffer = m_buffer.get();
	std::byte* dest = _dest.data();

	Memory::Copy(dest, buffer + idx, first);
	if (len > remaining) [[unlikely]]
		Memory::Copy(dest + first, buffer, len - first);

	m_consumer.value += len;

	return true;
}

template<size_t ByteSize>
void RingBuffer<FastSPSC>::ReadPreview(std::byte* __restrict _dest) const
{
	static_assert(ByteSize > 0);
	if constexpr (ByteSize > 256)
		return ReadPreview(_dest, ByteSize);
	else
	{
		size_t h = m_consumer.value;

		if (m_consumer.cachedValue - h < ByteSize) [[unlikely]]
		{
			m_consumer.cachedValue = m_producer.value;
			if (m_consumer.cachedValue - h < ByteSize) [[unlikely]]
				return;
		}

		const size_t idx = h & m_mask;
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

void RingBuffer<FastSPSC>::ReadPreview(std::byte* __restrict _dest, size_t _len) const
{
	if (_len == 0) [[unlikely]]
		return;

	size_t h = m_consumer.value;

	if (m_consumer.cachedValue - h < _len) [[unlikely]]
	{
		m_consumer.cachedValue = m_producer.value;
		if (m_consumer.cachedValue - h < _len) [[unlikely]]
			return;
	}

	const size_t idx = h & m_mask;
	const size_t remaining = m_capacity - idx;

	size_t first = _len < remaining ? _len : remaining;

	std::byte* buffer = m_buffer.get();

	Memory::Copy(_dest, buffer + idx, first);
	if (_len > remaining) [[unlikely]]
		Memory::Copy(_dest + first, buffer, _len - first);
}

void RingBuffer<FastSPSC>::ReadPreview(std::span<std::byte> _dest) const
{
	size_t len = _dest.size();

	size_t h = m_consumer.value;

	if (m_consumer.cachedValue - h < len) [[unlikely]]
	{
		m_consumer.cachedValue = m_producer.value;
		if (m_consumer.cachedValue - h < len) [[unlikely]]
			return;
	}

	const size_t idx = h & m_mask;
	const size_t remaining = m_capacity - idx;

	size_t first = len < remaining ? len : remaining;

	std::byte* buffer = m_buffer.get();
	std::byte* dest = _dest.data();

	Memory::Copy(dest, buffer + idx, first);
	if (len > remaining) [[unlikely]]
		Memory::Copy(dest + first, buffer, len - first);
}

void RingBuffer<FastSPSC>::Clear()
{
	m_consumer.value = m_producer.value;
	m_producer.cachedValue = m_consumer.cachedValue;
}

size_t RingBuffer<FastSPSC>::Size() const noexcept
{
	const size_t t = m_producer.value;
	const size_t h = m_consumer.value;
	return t - h;
}
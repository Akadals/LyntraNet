using namespace LT;

RingBuffer<ZeroCopy>::WriteRegion RingBuffer<ZeroCopy>::AcquireWriteRegion()
{
	size_t h = m_head.value.load(std::memory_order_acquire);
	size_t t = m_tail.value.load(std::memory_order_relaxed);

	size_t readable = t - h;
	size_t writable = m_capacity - readable;

	size_t headIdx = h & m_mask;
	size_t tailIdx = t & m_mask;

	size_t contiguous =
		(tailIdx >= headIdx)
		? (m_capacity - tailIdx)
		: (headIdx - tailIdx);
	size_t size =
		std::min(writable, contiguous);

	return
	{
		&m_buffer[tailIdx],
		size,
		&m_buffer[0],
		writable - size,
		writable
	};
}

RingBuffer<ZeroCopy>::ReadRegion RingBuffer<ZeroCopy>::AcquireReadRegion()
{
	size_t h = m_head.value.load(std::memory_order_relaxed);
	size_t t = m_tail.value.load(std::memory_order_acquire);

	size_t readable = t - h;

	size_t headIdx = h & m_mask;
	size_t tailIdx = t & m_mask;

	size_t contiguous =
		(tailIdx >= headIdx)
		? tailIdx - headIdx
		: m_capacity - headIdx;

	size_t size =
		std::min(readable, contiguous);

	return
	{
		&m_buffer[headIdx],
		size,
		&m_buffer[0],
		readable - size,
		readable
	};
}

void RingBuffer<ZeroCopy>::CommitWrite(size_t _size)
{
	size_t tail = m_tail.value.load(std::memory_order_relaxed);
	m_tail.value.store(tail + _size, std::memory_order_release);
}

void RingBuffer<ZeroCopy>::CommitRead(size_t _size)
{
	size_t head = m_head.value.load(std::memory_order_relaxed);
	m_head.value.store(head + _size, std::memory_order_release);
}
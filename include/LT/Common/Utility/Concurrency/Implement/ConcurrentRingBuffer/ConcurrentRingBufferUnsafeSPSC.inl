namespace LT {
	RingBuffer<SPSC_UNSAFE>::RingBuffer(size_t _size) {
		if (_size == 0) throw;

		m_capacity = 1;
		while (m_capacity < _size) m_capacity <<= 1;

		m_mask = m_capacity - 1;
		m_buffer = std::make_unique<std::byte[]>(m_capacity);
	}

	template<size_t ByteSize>
	bool RingBuffer<SPSC_UNSAFE>::TryWriteImpl(const std::byte* __restrict _src) noexcept {
		static_assert(ByteSize > 0);
		if constexpr (ByteSize > 256)
			return TryWriteImpl(_src, ByteSize);
		else {
			size_t h = m_head.value;

			if ((h + ByteSize) - m_head.cachedValue > m_capacity) [[unlikely]] {
				m_head.cachedValue = m_tail.value;
				if ((h + ByteSize) - m_head.cachedValue > m_capacity) [[unlikely]]
					return false;
			}

			const size_t idx = h & m_mask;
			const size_t remaining = m_capacity - idx;

			std::byte* buffer = m_buffer.get();

			if (remaining >= ByteSize)
				Memory::Copy<ByteSize>(buffer + idx, _src);
			else [[unlikely]] {
				Memory::Copy(buffer + idx, _src, remaining);
				Memory::Copy(buffer, _src + remaining, ByteSize - remaining);
			}

			m_head.value += ByteSize;

			return true;
		}
	}

	bool RingBuffer<SPSC_UNSAFE>::TryWriteImpl(const std::byte* __restrict _src, size_t _len) noexcept {
		if (_len == 0) [[unlikely]]
			return true;

		size_t h = m_head.value;

		if ((h + _len) - m_head.cachedValue > m_capacity) [[unlikely]] {
			m_head.cachedValue = m_tail.value;
			if ((h + _len) - m_head.cachedValue > m_capacity) [[unlikely]]
				return false;
		}

		const size_t idx = h & m_mask;
		const size_t remaining = m_capacity - idx;

		size_t first = _len < remaining ? _len : remaining;

		std::byte* buffer = m_buffer.get();

		Memory::Copy(buffer + idx, _src, first);
		if (_len > remaining) [[unlikely]]
			Memory::Copy(buffer, _src + first, _len - first);

		m_head.value += _len;

		return true;
	}

	bool RingBuffer<SPSC_UNSAFE>::TryWriteImpl(std::span<const std::byte> _src) noexcept {
		size_t len = _src.size();

		size_t h = m_head.value;

		if ((h + len) - m_head.cachedValue > m_capacity) [[unlikely]] {
			m_head.cachedValue = m_tail.value;
			if ((h + len) - m_head.cachedValue > m_capacity) [[unlikely]]
				return false;
		}

		const size_t idx = h & m_mask;
		const size_t remaining = m_capacity - idx;

		size_t first = len < remaining ? len : remaining;

		std::byte* buffer = m_buffer.get();
		const std::byte* src = _src.data();

		Memory::Copy(buffer + idx, src, first);
		if (len > remaining) [[unlikely]]
			Memory::Copy(buffer, src + first, len - first);

		m_head.value += len;

		return true;
	}

	template<size_t ByteSize>
	bool RingBuffer<SPSC_UNSAFE>::TryReadImpl(std::byte* __restrict _dest) noexcept {
		static_assert(ByteSize > 0);
		if constexpr (ByteSize > 256)
			return TryRead(_dest, ByteSize);
		else {
			size_t t = m_tail.value;

			if (m_tail.cachedValue - t < ByteSize) [[unlikely]] {
				m_tail.cachedValue = m_head.value;
				if (m_tail.cachedValue - t < ByteSize) [[unlikely]]
					return false;
			}

			const size_t idx = t & m_mask;
			const size_t remaining = m_capacity - idx;

			std::byte* buffer = m_buffer.get();

			if (remaining >= ByteSize)
				Memory::Copy<ByteSize>(_dest, buffer + idx);
			else [[unlikely]] {
				Memory::Copy(_dest, buffer + idx, remaining);
				Memory::Copy(_dest + remaining, buffer, ByteSize - remaining);
			}

			m_tail.value += ByteSize;
			return true;
		}
	}

	bool RingBuffer<SPSC_UNSAFE>::TryReadImpl(std::byte* __restrict _dest, size_t _len) noexcept {
		if (_len == 0) [[unlikely]]
			return true;

		size_t t = m_tail.value;

		if (m_tail.cachedValue - t < _len) [[unlikely]] {
			m_tail.cachedValue = m_head.value;
			if (m_tail.cachedValue - t < _len) [[unlikely]]
				return false;
		}

		const size_t idx = t & m_mask;
		const size_t remaining = m_capacity - idx;

		size_t first = _len < remaining ? _len : remaining;

		std::byte* buffer = m_buffer.get();

		Memory::Copy(_dest, buffer + idx, first);
		if (_len > remaining) [[unlikely]]
			Memory::Copy(_dest + first, buffer, _len - first);

		m_tail.value += _len;

		return true;
	}

	bool RingBuffer<SPSC_UNSAFE>::TryReadImpl(std::span<std::byte> _dest) noexcept {
		size_t len = _dest.size();

		size_t t = m_tail.value;

		if (m_tail.cachedValue - t < len) [[unlikely]] {
			m_tail.cachedValue = m_head.value;
			if (m_tail.cachedValue - t < len) [[unlikely]]
				return false;
		}

		const size_t idx = t & m_mask;
		const size_t remaining = m_capacity - idx;

		size_t first = len < remaining ? len : remaining;

		std::byte* buffer = m_buffer.get();
		std::byte* dest = _dest.data();

		Memory::Copy(dest, buffer + idx, first);
		if (len > remaining) [[unlikely]]
			Memory::Copy(dest + first, buffer, len - first);

		m_tail.value += len;

		return true;
	}

	template<size_t ByteSize>
	void RingBuffer<SPSC_UNSAFE>::ReadPreviewImpl(std::byte* __restrict _dest) noexcept {
		static_assert(ByteSize > 0);
		if constexpr (ByteSize > 256)
			return ReadPreviewImpl(_dest, ByteSize);
		else {
			size_t t = m_tail.value;

			if (m_tail.cachedValue - t < ByteSize) [[unlikely]] {
				m_tail.cachedValue = m_head.value;
				if (m_tail.cachedValue - t < ByteSize) [[unlikely]]
					return;
			}

			const size_t idx = t & m_mask;
			const size_t remaining = m_capacity - idx;

			std::byte* buffer = m_buffer.get();

			if (remaining >= ByteSize)
				Memory::Copy<ByteSize>(_dest, buffer + idx);
			else [[unlikely]] {
				Memory::Copy(_dest, buffer + idx, remaining);
				Memory::Copy(_dest + remaining, buffer, ByteSize - remaining);
			}
		}
	}

	void RingBuffer<SPSC_UNSAFE>::ReadPreviewImpl(std::byte* __restrict _dest, size_t _len) noexcept {
		if (_len == 0) [[unlikely]]
			return;

		size_t t = m_tail.value;

		if (m_tail.cachedValue - t < _len) [[unlikely]] {
			m_tail.cachedValue = m_head.value;
			if (m_tail.cachedValue - t < _len) [[unlikely]]
				return;
		}

		const size_t idx = t & m_mask;
		const size_t remaining = m_capacity - idx;

		size_t first = _len < remaining ? _len : remaining;

		std::byte* buffer = m_buffer.get();

		Memory::Copy(_dest, buffer + idx, first);
		if (_len > remaining) [[unlikely]]
			Memory::Copy(_dest + first, buffer, _len - first);
	}

	void RingBuffer<SPSC_UNSAFE>::ReadPreviewImpl(std::span<std::byte> _dest) noexcept {
		size_t len = _dest.size();

		size_t t = m_tail.value;

		if (m_tail.cachedValue - t < len) [[unlikely]] {
			m_tail.cachedValue = m_head.value;
			if (m_tail.cachedValue - t < len) [[unlikely]]
				return;
		}

		const size_t idx = t & m_mask;
		const size_t remaining = m_capacity - idx;

		size_t first = len < remaining ? len : remaining;

		std::byte* buffer = m_buffer.get();
		std::byte* dest = _dest.data();

		Memory::Copy(dest, buffer + idx, first);
		if (len > remaining) [[unlikely]]
			Memory::Copy(dest + first, buffer, len - first);
	}

	void RingBuffer<SPSC_UNSAFE>::ClearImpl() noexcept {
		m_tail.value = m_head.value;
		m_head.cachedValue = m_tail.cachedValue;
	}

	size_t RingBuffer<SPSC_UNSAFE>::SizeImpl() const noexcept {
		const size_t h = m_head.value;
		const size_t t = m_tail.value;
		return h - t;
	}
}
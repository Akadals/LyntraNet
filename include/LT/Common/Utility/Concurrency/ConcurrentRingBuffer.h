#ifndef __INCL_LYNTRA_RING_BUFFER_H__
#define __INCL_LYNTRA_RING_BUFFER_H__

#include <LT/Memory.h>
#include <atomic>
#include <memory>
#include <span>
#include <algorithm>

#include <LT/Common/Utility/Concurrency/ConcurrentPolicy.h>

namespace LT {
	template <typename Derived>
	class RingBufferContainer : public CRTPContainer<Derived> {
	protected:
		size_t m_capacity;
	public:
		size_t Size()		const noexcept { return this->DerivedImpl().SizeImpl(); }
		size_t Capacity()	const noexcept { return m_capacity; }

		template<size_t ByteSize>
		bool TryWrite(const std::byte* __restrict _src) noexcept
		{ return this->DerivedImpl().template TryWriteImpl<ByteSize>(_src); }
		bool TryWrite(const std::byte* __restrict _src, size_t _len) noexcept
		{ return this->DerivedImpl().TryWriteImpl(_src, _len); }
		bool TryWrite(std::span<const std::byte> _src) noexcept
		{ return this->DerivedImpl().TryWriteImpl(_src); }


		template<size_t ByteSize>
		bool TryRead(std::byte* __restrict _dest) noexcept
		{ return this->DerivedImpl().template TryReadImpl<ByteSize>(_dest); }
		bool TryRead(std::byte* __restrict _dest, size_t _len) noexcept
		{ return this->DerivedImpl().TryReadImpl(_dest, _len); }
		bool TryRead(std::span<std::byte> _dest) noexcept
		{ return this->DerivedImpl().TryReadImpl(_dest); }


		template<size_t ByteSize>
		void ReadPreview(std::byte* __restrict _dest) noexcept
		{ return this->DerivedImpl().template ReadPreviewImpl<ByteSize>(_dest); }
		void ReadPreview(std::byte* __restrict _dest, size_t _len) noexcept
		{ return this->DerivedImpl().ReadPreviewImpl(_dest, _len); }
		void ReadPreview(std::span<std::byte> _dest) noexcept
		{ return this->DerivedImpl().ReadPreviewImpl(_dest); }

		void Clear() noexcept { return this->DerivedImpl().ClearImpl(); }
	};

	template<ConcurrentPolicy, size_t Size>
	class RingBuffer;

	template<size_t Size>
	class RingBuffer<SPSC, Size> : RingBufferContainer<RingBuffer<SPSC, Size>> {
		friend RingBufferContainer<RingBuffer<SPSC, Size>>;
	private:
		std::unique_ptr<std::byte[]> m_buffer;

		size_t m_mask;

		mutable CacheLineAtomic m_head;
		mutable CacheLineAtomic m_tail;
	private:
		RingBuffer();

		template<size_t ByteSize>
		bool TryWriteImpl(const std::byte* __restrict _src)					noexcept;
		bool TryWriteImpl(const std::byte* __restrict _src, size_t _len)	noexcept;
		bool TryWriteImpl(std::span<const std::byte> _src)					noexcept;

		template<size_t ByteSize>
		bool TryReadImpl(std::byte* __restrict _dest)				noexcept;
		bool TryReadImpl(std::byte* __restrict _dest, size_t _len)	noexcept;
		bool TryReadImpl(std::span<std::byte> _dest)				noexcept;

		template<size_t ByteSize>
		void ReadPreviewImpl(std::byte* __restrict _dest)				noexcept;
		void ReadPreviewImpl(std::byte* __restrict _dest, size_t _len)	noexcept;
		void ReadPreviewImpl(std::span<std::byte> _dest)				noexcept;

		void ClearImpl()	noexcept;
		size_t SizeImpl()	const noexcept;
	};

	template<size_t Size>
	class RingBuffer<SPSC_UNSAFE, Size> : RingBufferContainer<RingBuffer<SPSC_UNSAFE, Size>> {
		friend RingBufferContainer<RingBuffer<SPSC_UNSAFE, Size>>;
	private:
		std::unique_ptr<std::byte[]> m_buffer;

		size_t m_mask;

		mutable CacheLineCached m_head;
		mutable CacheLineCached m_tail;
	private:
		RingBuffer();

		template<size_t ByteSize>
		bool TryWriteImpl(const std::byte* __restrict _src)					noexcept;
		bool TryWriteImpl(const std::byte* __restrict _src, size_t _len)	noexcept;
		bool TryWriteImpl(std::span<const std::byte> _src)					noexcept;

		template<size_t ByteSize>
		bool TryReadImpl(std::byte* __restrict _dest)				noexcept;
		bool TryReadImpl(std::byte* __restrict _dest, size_t _len)	noexcept;
		bool TryReadImpl(std::span<std::byte> _dest)				noexcept;

		template<size_t ByteSize>
		void ReadPreviewImpl(std::byte* __restrict _dest)				noexcept;
		void ReadPreviewImpl(std::byte* __restrict _dest, size_t _len)	noexcept;
		void ReadPreviewImpl(std::span<std::byte> _dest)				noexcept;

		void ClearImpl()	noexcept;
		size_t SizeImpl()	const noexcept;
	};

	template<size_t Size>
	class RingBuffer<MPSC, Size> : public RingBufferContainer<RingBuffer<MPSC, Size>> {
		friend RingBufferContainer<RingBuffer<MPSC, Size>>;
	private:
		size_t m_mask;

		mutable CacheLineAtomic m_head;
		mutable CacheLineAtomic m_tail;
	public:
		RingBuffer();

		template<size_t ByteSize>
		bool TryWriteImpl(const std::byte* __restrict _src)					noexcept;
		bool TryWriteImpl(const std::byte* __restrict _src, size_t _len)	noexcept;
		bool TryWriteImpl(std::span<const std::byte> _src)					noexcept;

		template<size_t ByteSize>
		bool TryReadImpl(std::byte* __restrict _dest)				noexcept;
		bool TryReadImpl(std::byte* __restrict _dest, size_t _len)	noexcept;
		bool TryReadImpl(std::span<std::byte> _dest)				noexcept;

		template<size_t ByteSize>
		void ReadPreviewImpl(std::byte* __restrict _dest)				noexcept;
		void ReadPreviewImpl(std::byte* __restrict _dest, size_t _len)	noexcept;
		void ReadPreviewImpl(std::span<std::byte> _dest)				noexcept;

		void ClearImpl()	noexcept;
		size_t SizeImpl()	const noexcept;
	};

	template<size_t Size>
	class RingBuffer<SPMC, Size> : public RingBufferContainer<RingBuffer<SPMC, Size>> {
		friend RingBufferContainer<RingBuffer<SPMC, Size>>;
	private:
		size_t m_mask;

		mutable CacheLineAtomic m_head;
		mutable CacheLineAtomic m_tail;
	public:
		RingBuffer(size_t _size);

		template<size_t ByteSize>
		bool TryWriteImpl(const std::byte* __restrict _src)					noexcept;
		bool TryWriteImpl(const std::byte* __restrict _src, size_t _len)	noexcept;
		bool TryWriteImpl(std::span<const std::byte> _src)					noexcept;

		template<size_t ByteSize>
		bool TryReadImpl(std::byte* __restrict _dest)				noexcept;
		bool TryReadImpl(std::byte* __restrict _dest, size_t _len)	noexcept;
		bool TryReadImpl(std::span<std::byte> _dest)				noexcept;

		template<size_t ByteSize>
		void ReadPreviewImpl(std::byte* __restrict _dest)				noexcept;
		void ReadPreviewImpl(std::byte* __restrict _dest, size_t _len)	noexcept;
		void ReadPreviewImpl(std::span<std::byte> _dest)				noexcept;

		void ClearImpl()	noexcept;
		size_t SizeImpl()	const noexcept;
	};

	template<size_t Size>
	class RingBuffer<MPMC, Size> : public RingBufferContainer<RingBuffer<MPMC, Size>> {
		friend RingBufferContainer<RingBuffer<MPMC, Size>>;
	private:
		std::unique_ptr<std::byte[]> m_buffer;

		size_t m_mask;

		mutable CacheLineAtomic m_reserveHead;
		mutable CacheLineAtomic m_publishHead;

		mutable CacheLineAtomic m_reserveTail;
		mutable CacheLineAtomic m_publishTail;
	public:
		RingBuffer();

		template<size_t ByteSize>
		bool TryWriteImpl(const std::byte* __restrict _src)					noexcept;
		bool TryWriteImpl(const std::byte* __restrict _src, size_t _len)	noexcept;
		bool TryWriteImpl(std::span<const std::byte> _src)					noexcept;

		template<size_t ByteSize>
		bool TryReadImpl(std::byte* __restrict _dest)				noexcept;
		bool TryReadImpl(std::byte* __restrict _dest, size_t _len)	noexcept;
		bool TryReadImpl(std::span<std::byte> _dest)				noexcept;

		template<size_t ByteSize>
		void ReadPreviewImpl(std::byte* __restrict _dest)				noexcept;
		void ReadPreviewImpl(std::byte* __restrict _dest, size_t _len)	noexcept;
		void ReadPreviewImpl(std::span<std::byte> _dest)				noexcept;

		void ClearImpl()	noexcept;
		size_t SizeImpl()	const noexcept;
	};

	template<size_t Size>
	class RingBuffer<ZeroCopy, Size> {
	public:
		struct Region
		{
			std::byte* firstPtr = nullptr;
			size_t firstSize = 0;
			std::byte* secondPtr = nullptr;
			size_t secondSize = 0;

			Region(
				_Out_writes_bytes_all_(_firstSize)	std::byte* _firstPtr,
				_Out_								size_t _firstSize,
				_Out_writes_bytes_all_(_secondSize)	std::byte* _secondPtr,
				_Out_								size_t _secondSize) :
				firstPtr(_firstPtr),
				firstSize(_firstSize),
				secondPtr(_secondPtr),
				secondSize(_secondSize
				) {}

			bool Empty() const
			{
				return firstSize + secondSize == 0;
			}
		};
		struct WriteRegion : public Region
		{
			size_t writableSize = 0;
			WriteRegion(
				_Out_writes_bytes_all_(_firstSize)	std::byte* _firstPtr,
				_Out_								size_t _firstSize,
				_Out_writes_bytes_all_(_secondSize) std::byte* _secondPtr,
				_Out_								size_t _secondSize,
				_Out_								size_t _writableSize) :
				Region(
					_firstPtr,
					_firstSize,
					_secondPtr,
					_secondSize),
				writableSize(_writableSize) {}
		};
		struct ReadRegion : public Region
		{
			size_t readableSize = 0;
			ReadRegion(
				_Out_writes_bytes_all_(_firstSize)	std::byte* _firstPtr,
				_Out_								size_t _firstSize,
				_Out_writes_bytes_all_(_secondSize) std::byte* _secondPtr,
				_Out_								size_t _secondSize,
				_Out_								size_t _readableSize
			) :
				Region(
					_firstPtr,
					_firstSize,
					_secondPtr,
					_secondSize),
				readableSize(_readableSize) {}
		};
	private:
		std::unique_ptr<std::byte[]> m_buffer;

		size_t m_capacity;
		size_t m_mask;

		mutable CacheLineAtomic m_head;
		mutable CacheLineAtomic m_tail;
	public:
		RingBuffer<ZeroCopy>(
			_In_ size_t _size
		);

		WriteRegion AcquireWriteRegion();
		ReadRegion AcquireReadRegion();

		void CommitWrite(
			_In_ size_t _size
		);
		void CommitRead(
			_In_ size_t _size
		);
	};
}

#include <LT/Common/Utility/Concurrency/Implement/ConcurrentRingBuffer/ConcurrentRingBufferSPSC.inl>
#include <LT/Common/Utility/Concurrency/Implement/ConcurrentRingBuffer/ConcurrentRingBufferMPSC.inl>
#include <LT/Common/Utility/Concurrency/Implement/ConcurrentRingBuffer/ConcurrentRingBufferSPMC.inl>
#include <LT/Common/Utility/Concurrency/Implement/ConcurrentRingBuffer/ConcurrentRingBufferMPMC.inl>
#include <LT/Common/Utility/Concurrency/Implement/ConcurrentRingBuffer/ConcurrentRingBufferUnsafeSPSC.inl>
#include <LT/Common/Utility/Concurrency/Implement/ConcurrentRingBuffer/ConcurrentRingBufferZeroCopy.inl>
#endif //__INCL_LYNTRA_RING_BUFFER_H__
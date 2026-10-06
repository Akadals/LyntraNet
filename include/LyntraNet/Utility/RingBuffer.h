#ifndef __INCL_LYNTRA_RING_BUFFER_H__
#define __INCL_LYNTRA_RING_BUFFER_H__

#include "ConcurrentPolicy.h"
#include <LyntraNet/Memory.h>
#include <atomic>
#include <memory>
#include <span>

namespace LT
{
	template<ConcurrentPolicy>
	class RingBuffer;

	template<>
	class RingBuffer<SPSC>
	{
	private:
		std::unique_ptr<std::byte[]> m_buffer;

		size_t m_capacity;
		size_t m_mask;

		mutable CacheLineAtomic m_head;
		mutable CacheLineAtomic m_tail;
	public:
		RingBuffer<SPSC>(size_t _size);
		~RingBuffer();

		template<size_t ByteSize>
		bool TryWrite(
			_In_reads_bytes_(ByteSize) const std::byte* __restrict _src
		);
		bool TryWrite(
			_In_reads_bytes_(_len)	const std::byte* __restrict _src,
			_In_					size_t _len
		);
		bool TryWrite(
			_In_ std::span<const std::byte> _src
		);

		template<size_t ByteSize>
		bool TryRead(
			_Out_writes_bytes_all_(ByteSize) std::byte* __restrict _dest
		);
		bool TryRead(
			_Out_writes_bytes_all_(_len)	std::byte* __restrict _dest, 
			_In_							size_t _len
		);
		bool TryRead(
			_Out_ std::span<std::byte> _dest
		);

		template<size_t ByteSize>
		void ReadPreview(
			_Out_writes_bytes_all_(ByteSize) std::byte* __restrict _dest
		) const;
		void ReadPreview(
			_Out_writes_bytes_all_(_len)	std::byte* __restrict _dest, 
			_In_							size_t _len
		) const;
		void ReadPreview(
			_Out_ std::span<std::byte> _dest
		) const;

		void Clear();

		size_t Size() const noexcept;
		size_t Capacity() const { return m_capacity; }
	};

	template<>
	class RingBuffer<MPSC>
	{
	private:

		size_t m_capacity;
		size_t m_mask;

		mutable CacheLineAtomic m_head;
		mutable CacheLineAtomic m_tail;
	public:
		RingBuffer<MPSC>(size_t _size);
	};

	template<>
	class RingBuffer<SPMC>
	{
	private:
		size_t m_capacity;
		size_t m_mask;

		mutable CacheLineAtomic m_head;
		mutable CacheLineAtomic m_tail;
	public:
		RingBuffer<SPMC>(size_t _size);
	};
	
	template<>
	class RingBuffer<MPMC>
	{
	private:
		std::unique_ptr<std::byte[]> m_buffer;

		size_t m_capacity;
		size_t m_mask;

		mutable CacheLineAtomic m_reserveHead;
		mutable CacheLineAtomic m_publishHead;

		mutable CacheLineAtomic m_reserveTail;
		mutable CacheLineAtomic m_publishTail;
	public:
		RingBuffer<MPMC>(
			_In_ size_t _size
		);

		bool TryWrite(
			_In_reads_bytes_(_len)	const std::byte* __restrict _src,
			_In_					size_t _len
		);
		bool TryWrite(
			_In_ std::span<const std::byte> _src
		);

		bool TryRead(
			_Out_writes_bytes_all_(_len)	std::byte* __restrict _dest,
			_In_							size_t _len
		);
		bool TryRead(
			_Out_ std::span<std::byte> _dest
		);

		void ReadPreview(
			_Out_writes_bytes_all_(_len)	std::byte* __restrict _dest,
			_In_							size_t _len
		) const;
		void ReadPreview(
			_Out_ std::span<std::byte> _dest
		) const;

		void Clear();

		size_t Size() const noexcept;
		size_t Capacity() const { return m_capacity; }
	};
	template<>
	class RingBuffer<FastSPSC>
	{
	private:
		std::unique_ptr<std::byte[]> m_buffer;

		size_t m_capacity;
		size_t m_mask;

		mutable CacheLineCached m_producer;
		mutable CacheLineCached m_consumer;
	public:
		RingBuffer<FastSPSC>(size_t _size);

		template<size_t ByteSize>
		bool TryWrite(
			_In_reads_bytes_(ByteSize) const std::byte* __restrict _src
		);
		bool TryWrite(
			_In_reads_bytes_(_len)	const std::byte* __restrict _src,
			_In_					size_t _len
		);
		bool TryWrite(
			_In_ std::span<const std::byte> _src
		);

		template<size_t ByteSize>
		bool TryRead(
			_Out_writes_bytes_all_(ByteSize) std::byte* __restrict _dest
		);
		bool TryRead(
			_Out_writes_bytes_all_(_len)	std::byte* __restrict _dest, 
			_In_							size_t _len
		);
		bool TryRead(
			_Out_ std::span<std::byte> _dest
		);

		template<size_t ByteSize>
		void ReadPreview(
			_Out_writes_bytes_all_(ByteSize) std::byte* __restrict _dest
		) const;
		void ReadPreview(
			_Out_writes_bytes_all_(_len)	std::byte* __restrict _dest,
			_In_							size_t _len
		) const;
		void ReadPreview(
			_Out_ std::span<std::byte> _dest
		) const;

		void Clear();

		size_t Size() const noexcept;
		size_t Capacity() const { return m_capacity; }
	};
	template<>
	class RingBuffer<ZeroCopy>
	{
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
				_Out_								size_t _secondSize
				) :
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
				_Out_								size_t _writableSize
			) :
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
} //namespace LyntraNet


#include "Detail/RingBuffer/RingBufferSPSC.inl"
#include "Detail/RingBuffer/RingBufferMPSC.inl"
#include "Detail/RingBuffer/RingBufferSPMC.inl"
#include "Detail/RingBuffer/RingBufferMPMC.inl"
#include "Detail/RingBuffer/RingBufferFastSPSC.inl"
#include "Detail/RingBuffer/RingBufferZeroCopy.inl"
#endif //__INCL_LYNTRA_RING_BUFFER_H__
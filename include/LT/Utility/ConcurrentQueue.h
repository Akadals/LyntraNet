#ifndef __INCL_CONCURRENT_QUEUE_H__
#define __INCL_CONCURRENT_QUEUE_H__

#include "ConcurrentPolicy.h"
#include <atomic>
#include <memory>


namespace LT
{
	template<typename T, ConcurrentPolicy Policy>
	class ConcurrentQueue;

	template<typename T>
	class ConcurrentQueue<T, SPSC>
	{
	private:
		std::unique_ptr<T[]> m_buffer;

		size_t m_capacity;
		size_t m_mask;

		mutable CacheLineAtomic m_head;
		mutable CacheLineAtomic m_tail;
	public:
		ConcurrentQueue(size_t _size);
		bool Enqueue(const T* __restrict _src);
		bool Dequeue(T* __restrict _dest);
	};

	template<typename T>
	class ConcurrentQueue<T, MPSC>
	{

	};

	template<typename T>
	class ConcurrentQueue<T, SPMC>
	{

	};

	template<typename T>
	class ConcurrentQueue<T, MPMC>
	{
	private:
		struct Cell
		{
			std::atomic<size_t> m_sequence;
			T m_data;
		};
		std::unique_ptr<Cell[]> m_buffer;

		size_t m_capacity;
		size_t m_mask;

		mutable CacheLineAtomic m_head;
		mutable CacheLineAtomic m_tail;
	public:
		ConcurrentQueue<T, MPMC>(size_t _size);
		bool Enqueue(const T* __restrict _src);
		bool Dequeue(T* __restrict _dest);
		bool TryDequeue(T* __restrict _dest);
	};
}

#include "Detail/ConcurrentQueue/ConcurrentQueueSPSC.inl"
#include "Detail/ConcurrentQueue/ConcurrentQueueMPSC.inl"
#include "Detail/ConcurrentQueue/ConcurrentQueueSPMC.inl"
#include "Detail/ConcurrentQueue/ConcurrentQueueMPMC.inl"

#endif
#ifndef __INCL_LYNTRA_CONCURRENT_STACK_H__
#define __INCL_LYNTRA_CONCURRENT_STACK_H__

#include "ConcurrentPolicy.h"
#include <atomic>
#include <memory>

namespace LT
{
	template<typename T, ConcurrentPolicy Policy>
	class ConcurrentStack;
	template<typename T>
	class ConcurrentStack<T, SPSC>
	{
	private:
		std::unique_ptr<T[]> m_buffer;

		size_t m_capacity;

		mutable CacheLineAtomic m_top;
	public:
		ConcurrentStack(size_t _size);
		bool Push(const T& _data);
		bool Pop(T& _dest);
		void Top(T* _dest);
	};
	template<typename T>
	class ConcurrentStack<T, MPSC>
	{
	private:
		std::unique_ptr<T[]> m_buffer;

		size_t m_capacity;
		size_t m_mask;

		mutable CacheLineAtomic m_top;
	public:
		ConcurrentStack(size_t _size);
	};
	template<typename T>
	class ConcurrentStack<T, SPMC>
	{
	private:
		std::unique_ptr<T[]> m_buffer;

		size_t m_capacity;
		size_t m_mask;

		mutable CacheLineAtomic m_top;
	public:
		ConcurrentStack(size_t _size);
	};
	template<typename T>
	class ConcurrentStack<T, MPMC>
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

		mutable CacheLineAtomic m_top;
	public:
		ConcurrentStack(size_t _size);
		bool Push(const T& _data);
		bool Pop(T& _dest);
		void Top(T* _dest);
	};
}

#include "Detail/ConcurrentStack/ConcurrentStackSPSC.inl"
#include "Detail/ConcurrentStack/ConcurrentStackMPSC.inl"
#include "Detail/ConcurrentStack/ConcurrentStackSPMC.inl"
#include "Detail/ConcurrentStack/ConcurrentStackMPMC.inl"
#endif

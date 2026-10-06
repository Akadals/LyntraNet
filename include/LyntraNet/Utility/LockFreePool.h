#ifndef __INCL_LYNTRA_LOCK_FREE_POOL_H__
#define __INCL_LYNTRA_LOCK_FREE_POOL_H__

#include "ConcurrentStack.h"

namespace LT
{
	template<typename T, size_t Size = 0>
	class LockFreePool;

	template<typename T, size_t Size>
	class LockFreePool
	{
	private:
		ConcurrentStack<T, MPMC> m_stack;
	public:
		explicit LockFreePool() :
			m_stack(Size) {};
		T Acquire();
		void Release(T&& _obj);
	};
	template<typename T>
	class LockFreePool<T, 0>
	{
	private:
		size_t m_size;
		size_t m_capacity;
	public:
		LockFreePool(size_t _size) :
			m_size(_size), m_capacity(_size) {};
		T Acquire();
		void Release(T&& _obj);
	};
}

#include "Detail/LockFreePool.inl"
#endif
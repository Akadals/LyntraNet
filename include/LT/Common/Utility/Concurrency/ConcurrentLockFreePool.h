#ifndef __INCL_LYNTRA_LOCK_FREE_POOL_H__
#define __INCL_LYNTRA_LOCK_FREE_POOL_H__

#include <memory>
#include <atomic>
#include <cstdint>
#include <cstddef>

#include <LT/Common/Utility/Concurrency/ConcurrentPolicy.h>

namespace LT {
    enum class PoolObjectState : uint8_t {
        FREE,
        IN_USE
    };

    struct PoolHandle {
        uint32_t index;
        uint32_t generation;
    };

    struct PoolHead {
        uint32_t index;
        uint32_t generation;
    };

    template<typename T>
    struct Node
    {
        T object;
        std::atomic<uint32_t> next;
        std::atomic<PoolObjectState> state;
    };

    template<typename T>
    class PoolLease
    {
    public:
        PoolLease(const PoolLease&) = delete;
        PoolLease& operator=(const PoolLease&) = delete;

        PoolLease(PoolLease&&) noexcept;
        PoolLease& operator=(PoolLease&&) noexcept;

        ~PoolLease();

        T* Get() noexcept;
        T* operator->() noexcept;

        void Reset();
    };

    template<typename Derived, typename T>
    class LockFreePoolContainer : public CRTPContainer<Derived> {
    protected:
        size_t m_capacity;
    public:
        size_t Size() const noexcept                { return this->DerivedImpl().SizeImpl(); }
        size_t Capacity() const noexcept            { return this->m_capacity; }
        T* Acquire() noexcept                       { return this->DerivedImpl().AcquireImpl(); }
        PoolLease<T> AcquireLeased() noexcept       { return this->DerivedImpl().AcquireLeasedImpl(); }
        void Release(T* _val) noexcept              { this->DerivedImpl().ReleaseImpl(_val); }
        void Release(PoolLease<T> _val) noexcept    { this->DerivedImpl().ReleaseImpl(_val); }
    };

    template<typename ConcurrentPolicy, typename T, size_t Size = 0>
    class LockFreePool;

    template<typename T, size_t Size>
    class LockFreePool<SPSC, T, Size> : public LockFreePoolContainer<LockFreePool<SPSC, T, Size>, T>
    {
        static_assert(Size > 0|| Size <= UINT32_MAX);
        using Container = LockFreePoolContainer<
            LockFreePool<SPSC, T, Size>, T>;
        friend Container;
    private:
        std::unique_ptr<Node<T>[]> m_nodes;
        std::atomic<uint64_t> m_head;
    private:
        size_t SizeImpl() const noexcept { return Size; }
        T* AcquireImpl() noexcept;
        PoolLease<T> AcquireLeasedImpl() noexcept;
        void ReleaseImpl(T* _val) noexcept;
        void ReleaseImpl(PoolLease<T> _val) noexcept;
    public:
        LockFreePool();
    };

    template<typename T>
    class LockFreePool<SPSC, T, 0> : public LockFreePoolContainer<LockFreePool<SPSC, T, 0>, T>
    {
        using Container = LockFreePoolContainer<
            LockFreePool<SPSC, T, 0>, T>;
        friend Container;
    private:
        size_t m_size;
        std::unique_ptr<Node<T>[]> m_nodes;
        std::atomic<uint64_t> m_head;
    private:
        size_t SizeImpl() const noexcept { return m_size; }
        T* AcquireImpl() noexcept;
        PoolLease<T> AcquireLeasedImpl() noexcept;
        void ReleaseImpl(T* _val) noexcept;
        void ReleaseImpl(PoolLease<T> _val) noexcept;
    public:
        explicit LockFreePool(size_t _size);
    };
}

#include <LT/Common/Utility/Concurrency/Implement/ConcurrentLockFreePool/ConcurrentLockFreePoolSPSC.inl>
#include <LT/Common/Utility/Concurrency/Implement/ConcurrentLockFreePool/ConcurrentLockFreePoolMPSC.inl>
#include <LT/Common/Utility/Concurrency/Implement/ConcurrentLockFreePool/ConcurrentLockFreePoolSPMC.inl>
#include <LT/Common/Utility/Concurrency/Implement/ConcurrentLockFreePool/ConcurrentLockFreePoolMPMC.inl>
#include <LT/Common/Utility/Concurrency/Implement/ConcurrentLockFreePool/ConcurrentLockFreePoolUnsafeSPSC.inl>
#endif // __INCL_LYNTRA_LOCK_FREE_POOL_H__
#ifndef __INCL_LYNTRA_LOCK_FREE_POOL_H__
#define __INCL_LYNTRA_LOCK_FREE_POOL_H__

#include <memory>
#include <atomic>
#include <cstdint>
#include <cstddef>

namespace LT
{
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

    template<typename Derived>
    class LockFreePoolContainer : public CRTPContainer<Derived> {
    public:
        size_t Size() const noexcept {
            return DerivedImpl().SizeImpl();
        }
    };

    template<typename T, size_t Size = 0>
    class LockFreePool;

    template<typename T, size_t Size>
    class LockFreePool : public LockFreePoolContainer<LockFreePool<T, Size>>
    {
        static_assert(Size > 0);
        static_assert(Size <= UINT32_MAX);

    private:
        std::unique_ptr<Node<T>[]> m_nodes;
        std::atomic<uint64_t> m_head;

    public:
        LockFreePool();
        SizeImpl() const noexcept { return Size; }
    };

    template<typename T>
    class LockFreePool<T, 0>
    {
    private:
        size_t m_size;
        size_t m_capacity;

        std::unique_ptr<Node<T>[]> m_nodes;
        std::atomic<uint64_t> m_head;

    public:
        explicit LockFreePool(size_t _size);
    };
}

#endif
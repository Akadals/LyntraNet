namespace LT {
	template<typename T, size_t Size>
	LockFreePool<SPSC, T, Size>::LockFreePool() {

	}
	template<typename T>
	LockFreePool<SPSC, T, 0>::LockFreePool(size_t _size) {

	}
	template<typename T, size_t Size>
	T* LockFreePool<SPSC, T, Size>::AcquireImpl() noexcept {
		return {};
	}
	template<typename T>
	T* LockFreePool<SPSC, T, 0>::AcquireImpl() noexcept {
		return {};
	}
	template<typename T, size_t Size>
	PoolLease<T> LockFreePool<SPSC, T, Size>::AcquireLeasedImpl() noexcept {
		return {};
	}
	template<typename T>
	PoolLease<T> LockFreePool<SPSC, T, 0>::AcquireLeasedImpl() noexcept {
		return {};
	}
	template<typename T, size_t Size>
	void LockFreePool<SPSC, T, Size>::ReleaseImpl(T* _val) noexcept {

	}
	template<typename T>
	void LockFreePool<SPSC, T, 0>::ReleaseImpl(T* _val) noexcept {

	}
	template<typename T, size_t Size>
	void LockFreePool<SPSC, T, Size>::ReleaseImpl(PoolLease<T> _val) noexcept {

	}
	template<typename T>
	void LockFreePool<SPSC, T, 0>::ReleaseImpl(PoolLease<T> _val) noexcept {

	}
}
using namespace LT;

template<typename T, size_t Size>
T LockFreePool<T, Size>::Acquire()
{
	return T();
}

template<typename T, size_t Size>
void LockFreePool<T, Size>::Release(T&& _obj)
{
	m_stack.Push(std::move(_obj));
}

template<typename T>
T LockFreePool<T, 0>::Acquire()
{
	return T();
}

template<typename T>
void LockFreePool<T, 0>::Release(T&& _obj)
{

}

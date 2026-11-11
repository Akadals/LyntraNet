#ifndef __INCL_LLYNTRA_DEFINE_TYPE_H__
#define __INCL_LLYNTRA_DEFINE_TYPE_H__

#ifdef _MEMORY_
#define U_PTR std::unique_ptr
#define S_PTR std::shared_ptr
#endif

#ifdef _VECTOR_
template<typename T>
using VEC = std::vector<T>;

template<typename T>
using VEC3 = std::vector<std::vector<T>>;
#endif

#ifdef _UNORDERED_MAP_
template<typename K, typename V, typename H = std::hash<K>>
using DICT = std::unordered_map<K, V, H>;
#endif

#ifdef _ATOMIC_
template<typename T>
using ATOMIC = std::atomic<T>;
#endif

#endif //__INCL_LLYNTRA_DEFINE_TYPE_H__

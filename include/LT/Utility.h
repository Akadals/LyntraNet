#ifndef __INCL_LYNTRA_UTLITY_H__
#define __INCL_LYNTRA_UTLITY_H__

#include <vector>
#include <unordered_map>

#define U_PTR std::unique_ptr
#define S_PTR std::shared_ptr

template<typename T>
using VEC = std::vector<T>;

template<typename T>
using VEC2 = std::vector<std::vector<T>>;

template<typename K, typename V, typename H = std::hash<K>>
using DICT = std::unordered_map<K, V, H>;

#endif
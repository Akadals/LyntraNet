#ifndef __INCL_LYNTRA_CONCURRENT_HASH_MAP_H__
#define __INCL_LYNTRA_CONCURRENT_HASH_MAP_H__

#include "ConcurrentPolicy.h"
#include <atomic>
#include <memory>

namespace LT
{
	template<typename Key, typename Value, ConcurrentPolicy Policy>
	class ConcurrentHashMap;

	template<typename Key, typename Value>
	class ConcurrentHashMap<Key, Value, SPSC>
	{

	};
	template<typename Key, typename Value>
	class ConcurrentHashMap<Key, Value, MPSC>
	{

	};
	template<typename Key, typename Value>
	class ConcurrentHashMap<Key, Value, SPMC>
	{

	};
	template<typename Key, typename Value>
	class ConcurrentHashMap<Key, Value, MPMC>
	{
	public:
		void Add(Key _key, Value _value); //юс╫ц
	};
}

#include "Detail/ConcurrentHashMap/ConcurrentHashMapSPSC.inl"
#include "Detail/ConcurrentHashMap/ConcurrentHashMapMPSC.inl"
#include "Detail/ConcurrentHashMap/ConcurrentHashMapSPMC.inl"
#include "Detail/ConcurrentHashMap/ConcurrentHashMapMPMC.inl"
#endif
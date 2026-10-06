#ifndef __INCL_LYNTRA_MEMORY_H__
#define __INCL_LYNTRA_MEMORY_H__

#include <cstdint>
#include <cstring>
#include <immintrin.h>

namespace LT::Memory
{
	template<size_t Size>
    __forceinline void Copy(
        _Out_writes_bytes_all_(Size)    void* _dest,
        _In_                            const void* _src
    );

    __forceinline void Copy(
        _Out_writes_bytes_all_(_size)   void* _dest,
        _In_                            const void* _src,
        _In_                            size_t _size
    );
}
#endif
#ifndef __INCL_LYNTRA_CALLBACK_BRIDGE_H__
#define __INCL_LYNTRA_CALLBACK_BRIDGE_H__

#include <cstdint>

#ifdef LT_EXPORTS
#define LT_API __declspec(dllexport)
#else
#define LT_API __declspec(dllimport)
#endif

using SendCallback = void(__cdecl*)(
    int32_t result,
    void* userData
    );

extern "C"
{
    LT_API int32_t __cdecl SendPacket(
        uint32_t header,
        const uint8_t* payload,
        int32_t payloadSize,
        SendCallback callback,
        void* userData
    ); //수정필요
}
#endif
#include <LT/Windows/Network/Packet/CallBackBridge.h>
#include "Packet.pb.h"
int32_t __cdecl SendPacket(
    uint32_t header,
    const uint8_t* payload,
    int32_t payloadSize,
    SendCallback callback,
    void* userData
)
{
    if (payloadSize < 0)
        return -1;
    if (payloadSize > 0 && payload == nullptr)
        return -1;

    //±¸¼º

    return 0;
}
#ifndef __INCL_LYNTRA_PROTOCOL_H__
#define __INCL_LYNTRA_PROTOCOL_H__

enum class ProtocolType : uint8_t {
	TCP, UDP, KCP, QUIC, RTP, SRTP
};

#endif
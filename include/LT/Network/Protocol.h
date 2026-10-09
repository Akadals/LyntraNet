#ifndef __INCL_LYNTRA_PROTOCOL_H__
#define __INCL_LYNTRA_PROTOCOL_H__
#include <concepts>

#define PROTOCOL_TCP 0
#define PROTOCOL_UDP 1
#define PROTOCOL_KCP 2
#define PROTOCOL_QUIC 3

struct TCP {};
struct UDP {};
struct KCP {};
struct QUIC {};

template<typename Protocol>
concept TProtocol =
std::same_as<Protocol, TCP> || std::same_as<Protocol, UDP> ||
std::same_as<Protocol, QUIC> || std::same_as<Protocol, KCP>;

#endif
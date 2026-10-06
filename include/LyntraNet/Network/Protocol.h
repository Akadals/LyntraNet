#ifndef __INCL_LYNTRA_PROTOCOL_H__
#define __INCL_LYNTRA_PROTOCOL_H__
#include <concepts>

struct TCP {};
struct UDP {};
struct KCP {};
struct QUIC {};

template<typename Protocol>
concept TProtocol =
std::same_as<Protocol, TCP> || std::same_as<Protocol, UDP> ||
std::same_as<Protocol, QUIC> || std::same_as<Protocol, KCP>;

#endif
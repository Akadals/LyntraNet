#ifndef __INCL_LYNTRA_IP_ADDRESS_H__
#define __INCL_LYNTRA_IP_ADDRESS_H__

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <cstring>
#include <regex>
#include <optional>
#include <sal.h>

namespace LT
{

	typedef enum class IPVersion {
		INVALID, IPv4, IPv6
	} IPVERSION;

	struct IPv4 {
		IN_ADDR m_address;
		char m_addressStr[INET_ADDRSTRLEN];
	};
	struct IPv6 {
		IN6_ADDR m_address;
		char m_addressStr[INET6_ADDRSTRLEN];
	};

	typedef union Address {
		IPv4 ipv4;
		IPv6 ipv6;
	} ADDRESS;

	typedef class IPAddress
		IPADDRESS, * PIPADDRESS;

	class IPAddress {
	private:
		IPVERSION m_ipVersion;
		ADDRESS m_ipAddress;
	public:
		IPAddress() :
			m_ipVersion(IPVersion::INVALID) {}
		explicit IPAddress(_In_ const char* _ip);
		explicit IPAddress(_In_ const IN_ADDR& _ipv4);
		explicit IPAddress(_In_ const IN6_ADDR& _ipv6);

		BOOL IsIPv4() const { return m_ipVersion == IPVersion::IPv4; }
		BOOL IsIPv6() const { return m_ipVersion == IPVersion::IPv6; }
		BOOL IsValid() const { return m_ipVersion != IPVersion::INVALID; }
		BOOL IsLoopback() const;
		BOOL IsBroadcast() const;
		BOOL IsLinkLocalAllNodes() const;

		bool operator==(const IPAddress& _ip) const;
		bool operator!=(const IPAddress& _ip) const;

		IPVersion IPVersion() const { return m_ipVersion; }

		std::string ToString() const;
	public:
		static std::optional<IN_ADDR> ParseToIPv4(_In_ const char* _ip);
		static std::optional<IN6_ADDR> ParseToIPv6(_In_ const char* _ip);

		static IPAddress AnyIPv4() { return IPAddress("0.0.0.0"); }
		static IPAddress AnyIPv6() { return IPAddress("::"); }

		static IPAddress LoopbackIPv4() { return IPAddress("127.0.0.1"); }
		static IPAddress LoopbackIPv6() { return IPAddress("::1"); }
		
		static IPAddress BroadcastIPv4() { return IPAddress("255.255.255.255"); }
		static IPAddress LinkLocalAllNodesIPv6() { return IPAddress("ff02::1"); }

		static BOOL IsValidIPv4(_In_ const char* _ip);
		static BOOL IsValidIPv6(_In_ const char* _ip);
	};
}
#endif
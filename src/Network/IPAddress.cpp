#include <LT/Windows/Network/IPAddress.h>

namespace LT {
	IPAddress::IPAddress(const char* ip)
		: m_ipVersion(IPVersion::INVALID) {
		if (auto ipv4 = ParseToIPv4(ip)) {
			m_ipAddress.ipv4.m_address = *ipv4;

			if (inet_ntop(
				AF_INET,
				&m_ipAddress.ipv4.m_address,
				m_ipAddress.ipv4.m_addressStr,
				sizeof(m_ipAddress.ipv4.m_addressStr)) != nullptr) {
				m_ipVersion = IPVersion::IPv4;
			}
		}
		else if (auto ipv6 = ParseToIPv6(ip)) {
			m_ipAddress.ipv6.m_address = *ipv6;

			if (inet_ntop(
				AF_INET6,
				&m_ipAddress.ipv6.m_address,
				m_ipAddress.ipv6.m_addressStr,
				sizeof(m_ipAddress.ipv6.m_addressStr)) != nullptr) {
				m_ipVersion = IPVersion::IPv6;
			}
		}
	}
	IPAddress::IPAddress(const IN_ADDR& ipv4)
		: m_ipVersion(IPVersion::IPv4) {
		m_ipAddress.ipv4.m_address = ipv4;

		if (inet_ntop(
			AF_INET,
			&m_ipAddress.ipv4.m_address,
			m_ipAddress.ipv4.m_addressStr,
			sizeof(m_ipAddress.ipv4.m_addressStr)) == nullptr)
		{
			m_ipVersion = IPVersion::INVALID;
		}
	}

	IPAddress::IPAddress(const IN6_ADDR& ipv6)
		: m_ipVersion(IPVersion::IPv6) {
		m_ipAddress.ipv6.m_address = ipv6;

		if (inet_ntop(
			AF_INET6,
			&m_ipAddress.ipv6.m_address,
			m_ipAddress.ipv6.m_addressStr,
			sizeof(m_ipAddress.ipv6.m_addressStr)) == nullptr)
		{
			m_ipVersion = IPVersion::INVALID;
		}
	}
	BOOL IPAddress::IsLoopback() const {
		if (m_ipVersion == IPVersion::IPv4) {
			const uint32_t ip =
				ntohl(m_ipAddress.ipv4.m_address.s_addr);

			return (ip & 0xFF000000u) == 0x7F000000u;
		}

		if (m_ipVersion == IPVersion::IPv6) {
			const auto& addr = m_ipAddress.ipv6.m_address;

			if (IN6_IS_ADDR_LOOPBACK(&addr))
				return TRUE;

			if (IN6_IS_ADDR_V4MAPPED(&addr))
				return addr.s6_addr[12] == 127;
		}

		return FALSE;
	}
	BOOL IPAddress::IsBroadcast() const {
		if (m_ipVersion != IPVersion::IPv4)
			return FALSE;

		return m_ipAddress.ipv4.m_address.s_addr
			== INADDR_BROADCAST;
	}
	BOOL IPAddress::IsLinkLocalAllNodes() const {
		if (m_ipVersion != IPVersion::IPv6)
			return FALSE;

		static constexpr UCHAR expected[16] = {
			0xFF, 0x02, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 1
		};

		return std::memcmp(
			m_ipAddress.ipv6.m_address.s6_addr,
			expected,
			sizeof(expected)) == 0;
	}
	bool IPAddress::operator==(const IPAddress& ip) const {
		if (m_ipVersion != ip.m_ipVersion)
			return false;

		switch (m_ipVersion)
		{
		case IPVersion::IPv4:
			return m_ipAddress.ipv4.m_address.s_addr
				== ip.m_ipAddress.ipv4.m_address.s_addr;
		case IPVersion::IPv6:
			return std::memcmp(
				&m_ipAddress.ipv6.m_address,
				&ip.m_ipAddress.ipv6.m_address,
				sizeof(IN6_ADDR)) == 0;
		default: return true;
		}
	}

	bool IPAddress::operator!=(const IPAddress& ip) const {
		return !(*this == ip);
	}
	std::string IPAddress::ToString() const {
		if (m_ipVersion == IPVersion::IPv4)
			return m_ipAddress.ipv4.m_addressStr;
		else if (m_ipVersion == IPVersion::IPv6)
			return m_ipAddress.ipv6.m_addressStr;
		return "";
	}
	std::optional<IN_ADDR> IPAddress::ParseToIPv4(const char* ip) {
		if (ip == nullptr)
			return std::nullopt;

		IN_ADDR addr{};

		if (InetPtonA(AF_INET, ip, &addr) != 1)
			return std::nullopt;

		return addr;
	}

	std::optional<IN6_ADDR> IPAddress::ParseToIPv6(const char* ip) {
		if (ip == nullptr)
			return std::nullopt;

		IN6_ADDR addr{};

		if (InetPtonA(AF_INET6, ip, &addr) != 1)
			return std::nullopt;

		return addr;
	}
	BOOL IPAddress::IsValidIPv4(const char* ip) {
		if (ip == nullptr)
			return FALSE;

		IN_ADDR address{};
		return InetPtonA(AF_INET, ip, &address) == 1;
	}
	BOOL IPAddress::IsValidIPv6(const char* _ip) {
		if (_ip == nullptr)
			return FALSE;

		IN6_ADDR address{};
		return InetPtonA(AF_INET6, _ip, &address) == 1;
	}
}
#include "LyntraNet/Network/IPAddress.h"

using namespace LT;

IPAddress::IPAddress(std::string_view)
{}

void IPAddress::SetAddress(const sockaddr * _addr, socklen_t _len)
{
	m_length = _len;
	memcpy(&m_storage, _addr, _len);
}

ADDRESS_FAMILY IPAddress::Family() const
{
	return m_storage.ss_family;
}

size_t IPAddress::Hash() const
{
	return size_t();
}

bool IPAddress::IsIPv4() const
{
	return Family() == AF_INET;
}

bool IPAddress::IsIPv6() const
{
	return Family() == AF_INET6;
}

bool IPAddress::IsLoopback() const
{
	if (Family() == AF_INET)
	{
		const sockaddr_in* addr =
			reinterpret_cast<const sockaddr_in*>(&m_storage);
		uint32_t ip = ntohl(addr->sin_addr.s_addr);
		return (ip & 0xFF000000) == 0x7F000000;
	}
	else if(Family() == AF_INET6)
	{
		const sockaddr_in6* addr =
			reinterpret_cast<const sockaddr_in6*>(&m_storage);
		if (IN6_IS_ADDR_LOOPBACK(&addr->sin6_addr))
			return true;
		if (IN6_IS_ADDR_V4MAPPED(&addr->sin6_addr))
		{
			const uint32_t* v4_mapped = 
				reinterpret_cast<const uint32_t*>(addr->sin6_addr.s6_addr + 12);
			uint32_t ip = ntohl(*v4_mapped);
			return (ip & 0xFF000000) == 0x7F000000;
		}
	}
	return false;
}

bool IPAddress::IsAny() const
{
	return false;
}

bool IPAddress::IsBroadcast() const
{
	return false;
}

bool IPAddress::IsMulticast() const
{
	return false;
}

bool IPAddress::operator==(const IPAddress&) const
{
	return false;
}

bool IPAddress::operator!=(const IPAddress&) const
{
	return false;
}

const sockaddr* IPAddress::Data() const
{
	return reinterpret_cast<const sockaddr*>(&m_storage);
}

sockaddr* IPAddress::Data()
{
	return reinterpret_cast<sockaddr*>(&m_storage);
}

socklen_t IPAddress::Length() const
{
	return m_length;
}

uint16_t IPAddress::Port() const
{
	switch (Family())
	{
	case AF_INET:
		return ntohs(
			reinterpret_cast<const sockaddr_in*>
			(&m_storage)->sin_port);
	case AF_INET6:
		return ntohs(
			reinterpret_cast<const sockaddr_in6*>
			(&m_storage)->sin6_port);
	default: return 0;
	}
}

std::string IPAddress::ToString() const
{
	char buffer[INET6_ADDRSTRLEN]{};

	switch (Family())
	{
	case AF_INET:
		inet_ntop(
			AF_INET,
			&reinterpret_cast<const sockaddr_in*>
			(&m_storage)->sin_addr,
			buffer,
			sizeof(buffer));
		break;
	case AF_INET6:
		inet_ntop(
			AF_INET6,
			&reinterpret_cast<const sockaddr_in6*>
			(&m_storage)->sin6_addr,
			buffer,
			sizeof(buffer));
		break;
	default:
		strcpy_s(buffer, "Unknown");
		break;
	}

	return buffer;
}

std::string IPAddress::ToStringWithPort() const
{
	return ToString() + ":" + std::to_string(Port());
}

IPAddress IPAddress::AnyIPv4()
{
	return IPAddress();
}

IPAddress IPAddress::AnyIPv6()
{
	return IPAddress();
}

uint16_t IPAddress::AnyPort()
{
	return 0;
}

IPAddress IPAddress::LoopbackIPv4()
{
	return IPAddress();
}

IPAddress IPAddress::LoopbackIPv6()
{
	return IPAddress();
}

IPAddress IPAddress::Parse(std::string_view _ip)
{
	return IPAddress();
}

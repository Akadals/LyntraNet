#ifndef __INCL_LYNTRA_IPADDRESS_H__
#define __INCL_LYNTRA_IPADDRESS_H__

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <sal.h>

#define IPV4_DEFAULT_LOOPBACK "127.0.0.1"
#define IPV6_DEFAULT_LOOPBACK "::1" 
// 수정 필요


namespace LT
{
	class IPAddress
	{
	private:
		sockaddr_storage m_storage = {};
		socklen_t m_length = {};
	public:
		explicit IPAddress(std::string_view = IPV4_DEFAULT_LOOPBACK);

		void SetAddress(
			_In_reads_to_ptr_(_len) const sockaddr* _addr,
			_In_					socklen_t _len);

		ADDRESS_FAMILY Family() const;

		size_t Hash() const;

		bool IsIPv4() const;
		bool IsIPv6() const;
		bool IsLoopback() const;
		bool IsAny() const;
		bool IsBroadcast() const;
		bool IsMulticast() const;

		bool operator==(const IPAddress&) const;
		bool operator!=(const IPAddress&) const;

		const sockaddr* Data() const;
		sockaddr* Data();
		socklen_t Length() const;
		uint16_t Port() const;

		std::string ToString() const;
		std::string ToStringWithPort() const;
	public:
		static IPAddress AnyIPv4();
		static IPAddress AnyIPv6();

		static uint16_t AnyPort();

		static IPAddress LoopbackIPv4();
		static IPAddress LoopbackIPv6();

		static IPAddress Parse(std::string_view);
	};

	typedef IPAddress IPEndPoint;
}
#endif
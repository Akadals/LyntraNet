#ifndef __INCL_LYNTRA_IPENDPOINT_H__
#define __INCL_LYNTRA_IPENDPOINT_H__

#include <LyntraNet/Network/IPAddress.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <sal.h>


namespace LT
{
	class IPEndPoint
	{
	private:
		IPAddress m_address;
		uint16_t m_port;
	public:
#ifdef ADDRESS_IPV6_BASED
		IPEndPoint() :
			m_address(IPAddress::LoopbackIPv6()),
			m_port(IPAddress::AnyPort()) {}
#else
		IPEndPoint() :
			m_address(IPAddress::LoopbackIPv4()),
			m_port(IPAddress::AnyPort()) {}
#endif
		IPEndPoint(const IPAddress& _address, uint16_t _port) :
			m_address(_address),
			m_port(_port) {}

		void SetIPAddress(const IPAddress& _address) { m_address = _address; }
		IPAddress GetAddress() const noexcept { return m_address; }

		void SetPort(uint16_t _port) { m_port = _port; }
		uint16_t GetPort() const noexcept { return m_port; }
	};
}
#endif
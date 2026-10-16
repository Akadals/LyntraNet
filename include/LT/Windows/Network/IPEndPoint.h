#ifndef __INCL_LYNTRA_IP_ENDPOINT_H__
#define __INCL_LYNTRA_IP_ENDPOINT_H__

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <sal.h>

#include <LT/Windows/Network/IPAddress.h>
namespace LT
{
	class IPEndPoint
	{
	private:
		IPADDRESS			m_ipAddress;
		UINT16				m_port;
		SOCKADDR_STORAGE	m_storage;
		socklen_t			m_length;
	public:
		IPEndPoint() = default;
		explicit IPEndPoint(
			_In_ const IPAddress& _ip,
			_In_ UINT16 _port) :
			m_ipAddress(_ip),
			m_port(_port),
			m_storage{},
			m_length{} {}
		IPEndPoint(_In_ const IPEndPoint& _endpoint) = default;
		IPEndPoint(
			_In_reads_to_ptr_(_len) const sockaddr* _addr,
			_In_ socklen_t _len) { Set(_addr, _len); }

		void Set(_In_ const IPEndPoint& _endpoint) { *this = _endpoint; }
		void Set(
			_In_reads_to_ptr_(_len) const sockaddr* _addr,
			_In_ socklen_t _len);

		ADDRESS_FAMILY Family() const { return m_storage.ss_family; }

		bool operator==(const IPEndPoint& _endpoint) const {
			return m_ipAddress == _endpoint.m_ipAddress
				&& m_port == _endpoint.m_port;
		}
		bool operator!=(const IPEndPoint& _endpoint) const { return !(*this == _endpoint); }

		IPAddress GetIPAddress() const { return m_ipAddress; }
		UINT16 Port() const { return m_port; }
		const sockaddr* Data() const { return reinterpret_cast<const sockaddr*>(&m_storage); }
		sockaddr* Data() { return reinterpret_cast<sockaddr*>(&m_storage); }
		socklen_t Length() const { return m_length; }
		socklen_t* Length() { return &m_length; }
	public:
		static UINT16 AnyPort() { return 0; }
	};
}
#endif
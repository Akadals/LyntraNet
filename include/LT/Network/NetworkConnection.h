#ifndef __INCL_LYNTRA_NETWORK_CONNECTION_H__
#define __INCL_LYNTRA_NETWORK_CONNECTION_H__

#include <LT/Utility.h>
#include <LT/Network/Transport/ITransport.h>
#include <LT/Network/Transport/TCPTransport.h>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <atomic>

namespace LT
{
	typedef class NetworkConnection
		NET_CONNECTION, * PNET_CONNECTION;

	struct ConnectionStats
	{

	};

	class alignas(64) NetworkConnection
	{
	public:
		static const size_t RECV_BUF_SIZE = 8192;
		static const size_t SEND_BUF_SIZE = 8192;
	private:
		uint64_t connectionId;

		std::unique_ptr<Transport::ITransport> m_transport;

		RingBuffer<ZeroCopy> m_recvBuf = { RECV_BUF_SIZE };
		RingBuffer<ZeroCopy> m_sendBuf = { SEND_BUF_SIZE };

		DWORD m_timeoutTime;
		ConnectionStats m_stats;
	public:
		NetworkConnection() = default;

		void SetTransport(_In_ std::unique_ptr<Transport::ITransport> _transport)
		{ m_transport = std::move(_transport); }

		void Bind(
			_In_ SOCKET _socket,
			_In_ const SOCKADDR_IN& _localAddr,
			_In_opt_ const SOCKADDR_IN& _remoteAddr)
		{ m_transport->GetSocket().Bind(_socket, _localAddr, _remoteAddr); }

		DWORD Read();
		DWORD Write();
	};
}
#endif
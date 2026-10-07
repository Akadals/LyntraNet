#ifndef __INCL_LYNTRA_NETWORK_SOCKET_H__
#define __INCL_LYNTRA_NETWORK_SOCKET_H__

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <mswsock.h>
#include <mstcpip.h>
#include <sal.h>

#include <LT/Network/IPAddress.h>
#include <LT/Utility.h>
#include <LT/Packet.h>
#include <LT/Network/IOContext.h>


namespace LT::Socket
{
	typedef class NetworkSocket
		NET_SOCK, * PNET_SOCK;

	struct SocketOption
	{
		int st_so_rcvbuf_size = 8192;
		int st_so_sndbuf_size = 8192;
		bool b_so_reuseaddr = true;

#pragma region TCP
		bool b_tcp_nodelay = true;
		bool b_so_keepalive = true;
		tcp_keepalive ka_vals =
		{
			.onoff = 1,
			.keepalivetime = 30000,
			.keepaliveinterval = 5000
		};
#pragma endregion
		SocketOption() = default;
	};

	class NetworkSocket
	{
	public:
		static const size_t IO_CONTEXT_POOL_SIZE = 128;
	protected:
		SOCKET m_sock = INVALID_SOCKET;

		IPAddress m_local;
		IPAddress m_remote;
	protected:
		LockFreePool<PPIOCTX,
			IO_CONTEXT_POOL_SIZE> m_ioContextPool;
	private:
		SocketOption m_sockOpt = {};
	public:
		NetworkSocket() = default;
		virtual ~NetworkSocket();

		NetworkSocket(NetworkSocket&&) noexcept;
		NetworkSocket& operator=(NetworkSocket&&) noexcept;

		NetworkSocket(const NetworkSocket&) = delete;
		NetworkSocket& operator=(const NetworkSocket&) = delete;

		void SetSocketOpt(SocketOption&& _sockOpt) { m_sockOpt = _sockOpt; }

		void Bind(
			_In_ SOCKET _socket, 
			_In_ const SOCKADDR_IN& _localAddr,
			_In_opt_ const SOCKADDR_IN& _remoteAddr
		);
		bool IsValid() const noexcept { return m_sock != INVALID_SOCKET; }

		SOCKET GetHandle() const noexcept { return m_sock; }

		const IPAddress& GetLocal() const { return m_local; }
		const IPAddress& GetRemote() const { return m_remote; }

		IOCTX* AcquireContext() 
		{ return m_ioContextPool.Acquire(); }

		void ReleaseContext(
			_In_ IOCTX* _context
		) { m_ioContextPool.Release(std::move(_context)); }

		virtual DWORD Recv(
			_In_ IOCTX& _context
		) = 0;
		virtual DWORD Send(
			_In_ IOCTX& _context
		) = 0;
		virtual void Close() noexcept = 0;
	};
}
#endif
#include "LT/Network/Socket/NetworkSocket.h"

using namespace LyntraNet::Socket;

void NetworkSocket::Bind(
	SOCKET _socket,
	const SOCKADDR_IN& _localAddr,
	const SOCKADDR_IN& _remoteAddr)
{
	m_sock = _socket;

	m_local.SetAddress(
		reinterpret_cast<const sockaddr*>(&_localAddr),
		sizeof(_localAddr));

	m_remote.SetAddress(
		reinterpret_cast<const sockaddr*>(&_remoteAddr),
		sizeof(_remoteAddr));

	int flag = 1;

	setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, 
		(char*)&m_sockOpt.st_so_rcvbuf_size, sizeof(m_sockOpt.st_so_rcvbuf_size));
	setsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, 
		(char*)&m_sockOpt.st_so_sndbuf_size, sizeof(m_sockOpt.st_so_sndbuf_size));

	if (m_sockOpt.b_tcp_nodelay)
		setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));
	if(m_sockOpt.b_so_reuseaddr)
		setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&flag, sizeof(flag));
	if (m_sockOpt.b_so_keepalive)
	{
		DWORD ret = 0;
		setsockopt(m_sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&flag, sizeof(flag));
		WSAIoctl(m_sock, SIO_KEEPALIVE_VALS,
			&m_sockOpt.ka_vals, sizeof(m_sockOpt.ka_vals),
			nullptr, 0, &ret, nullptr, nullptr);
	}
}

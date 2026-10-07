#ifndef __INCL_LYNTRA_LYNTRANET_SOCKET_H__
#define __INCL_LYNTRA_LYNTRANET_SOCKET_H__

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <mswsock.h>
#include <mstcpip.h>
#include <sal.h>

namespace LT
{
	typedef struct LTSocket
		LTSOCKET, * PLTSOCKET;

	struct LTSocket
	{
#ifdef _WIN32
		SOCKET m_sock;

		LTSocket() :
			m_sock(INVALID_SOCKET) {}
		LTSocket(const SOCKET& _socket) :
			m_sock(_socket) {}

		BOOL IsValid() const { return m_sock != INVALID_SOCKET; }

		static LTSocket INVALID() { return INVALID_SOCKET; }
#elif __linux__
		int m_fd;
		LTSocket(int _fd) :
			m_fd(_fd) {}
#endif
	};
}

#endif
#ifndef __INCL_LYNTRA_LYNTRANET_SOCKET_H__
#define __INCL_LYNTRA_LYNTRANET_SOCKET_H__

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <mswsock.h>
#include <mstcpip.h>
#include <sal.h>

#define CICP CreateIoCompletionPort
#define GQCS GetQueuedCompletionStatus
#define GQCSEx GetQueuedCompletionStatusEx

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
		BOOL operator==(const LTSOCKET& other) const noexcept
		{
			return m_sock == other.m_sock;
		}

		static LTSocket INVALID() { return INVALID_SOCKET; }
#elif __linux__
		int m_fd;
		LTSocket(int _fd) :
			m_fd(_fd) {}
#endif
	};

	struct LTSocketHash
	{
		size_t operator()(const LTSOCKET& socket) const noexcept
		{
			return std::hash<SOCKET>{}(socket.m_sock);
		}
	};
}

#endif
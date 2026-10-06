#ifndef __INCL_LYNTRA_TCP_SOCKET_H__
#define __INCL_LYNTRA_TCP_SOCKET_H__

#include "NetworkSocket.h"

namespace LT::Socket
{
	typedef class TCPSocket
		TCP_SOCK, * PTCP_SOCK;

	class TCPSocket : public NetworkSocket
	{
		alignas(64) std::atomic<DWORD> m_ioCount = 0x80000000;
		alignas(64) DWORD m_ioFlag = 0;

		std::atomic<bool> m_isUseTLS = false;
	public:
		TCPSocket();
		DWORD Recv(_In_ IOCONTEXT& _context) override;
		DWORD Send(_In_ IOCONTEXT& _context) override;
	};
}
#endif

#ifndef __INCL_LYNTRA_ITRANSPORT_H__
#define __INCL_LYNTRA_ITRANSPORT_H__

#include <LyntraNet/Network/Socket/NetworkSocket.h>

namespace LT::Transport
{
	class ITransport
	{
	private:
		std::unique_ptr<SocketSocket> m_sock;
	public:
		void SetSocket(
			_In_ std::unique_ptr<SocketSocket> _socket
		);
		SocketSocket& GetSocket() { return *m_sock; }
		const SocketSocket& GetSocket() const { return *m_sock; }
	};
}
#endif
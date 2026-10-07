#ifndef __INCL_LYNTRA_UDP_SOCKET_H__
#define __INCL_LYNTRA_UDP_SOCKET_H__


#include "NetworkSocket.h"

namespace LT::Socket
{
	typedef class UDPSocket
		UDP_SOCK, * PUDP_SOCK;

	class UDPSocket : public NetworkSocket
	{

	};
}
#endif

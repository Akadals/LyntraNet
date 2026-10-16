#ifndef __INCL_LYNTRA_LYNTRANET_CONNECTION_H__
#define __INCL_LYNTRA_LYNTRANET_CONNECTION_H__

#include <LT/Utility.h>
#include <LT/Windows/Core/Transport.h>

namespace LT
{
	class LTConnection {
	private:
		U_PTR<ITransport> m_transport;
	public:
		LTConnection() :
			m_transport(nullptr) {}

		ITransport* GetTransport() { return m_transport.get(); }
		LTSOCKET GetSock() { return m_transport->Get(); }

		void ShutDown() { m_transport.reset(); }
	};
}

#endif
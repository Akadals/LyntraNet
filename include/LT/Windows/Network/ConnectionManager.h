#ifndef __INCL_LYNTRA_CONNECTION_MANAGER_H__
#define __INCL_LYNTRA_CONNECTION_MANAGER_H__

#include <LT/Utility.h>
#include <LT/Windows/Network/LTConnection.h>


namespace LT
{
	typedef class ConnectionManager
		CONNMGR, * PCONNCMGR;

	class ConnectionManager
	{
	private:
		//LockFreePool<U_PTR<LTConnection>> m_connectionPool;
		VEC<U_PTR<LTConnection>> m_connections;
	public:
		ConnectionManager() = default;
		LTConnection* NewConnection()
		{
			auto ret = std::make_unique<LTConnection>(); //m_connectionPool.Acquire();
			m_connections.emplace_back(std::move(ret));
			return ret.get();
		}

		LTConnection* operator[](LTSOCKET _socket)
		{
			for (auto& key : m_connections)
			{
				if (key.get()->GetSock() == _socket)
					return key.get();
			}
			return nullptr;
		}
		LTConnection* operator[](SOCKET _socket)
		{
			for (auto& key : m_connections)
			{
				if (key.get()->GetSock().m_sock == _socket)
					return m_connections[_socket].get();
			}
			return nullptr;
		}
	};
}
#endif
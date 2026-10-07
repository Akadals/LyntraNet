#ifndef __INCL_LYNTRA_CONNECTION_MANAGER_H__
#define __INCL_LYNTRA_CONNECTION_MANAGER_H__

#include <LT/Utility.h>
#include "NetworkConnection.h"
#include "Socket/NetworkSocket.h"
#include "Session.h"

#include <LT/Network/IOContext.h>

#define U_PTR std::unique_ptr
#define S_PTR std::shared_ptr

namespace LT
{
	class ConnectionManager
	{
	public:
		static const size_t CONNECTION_POOL_SIZE = 1024;
		static const size_t SESSION_POOL_SIZE = 1024;
		static const size_t ACCEPT_CONTEXT_POOL_SIZE = 100;
		static const size_t JOB_POOL_SIZE = 65565;
	private:
		LockFreePool<S_PTR<NET_CONNECTION>>
			m_connectionPool = { CONNECTION_POOL_SIZE };
		LockFreePool<U_PTR<Transport::ITransport>>
			m_transportPool = { CONNECTION_POOL_SIZE };
		LockFreePool<U_PTR<Socket::NET_SOCK>>
			m_socketPool = { CONNECTION_POOL_SIZE };

		LockFreePool<PPIOCTX, ACCEPT_CONTEXT_POOL_SIZE>
			m_acceptCtxPool;

		//LockFreePool<Packet::JOB> m_JobPool = { JOB_POOL_SIZE };

#pragma region Network Session
		ConcurrentHashMap<uint64_t, U_PTR<NET_CONNECTION>, MPSC>
			m_connections;
		ConcurrentHashMap<SOCKET, uint64_t, MPSC>
			m_socketMap;
#pragma endregion

#pragma region Game Session
#ifdef _USE_GAME_SESSION_
		LockFreePool<Game::SESSION> m_sessionPool = { SESSION_POOL_SIZE };
		ConcurrentHashMap<uint64_t, Game::SESSION, MPSC> m_userList;
#endif
#pragma endregion
	public:
		ConnectionManager();
		S_PTR<NET_CONNECTION> AcquireConnection() 
		{ return m_connectionPool.Acquire(); }
		U_PTR<Transport::ITransport> AcquireTransport()
		{ return m_transportPool.Acquire(); }
		U_PTR<Socket::NET_SOCK> AcquireSocket()
		{ return m_socketPool.Acquire(); }
		PPIOCTX AcquireAcceptCtx()
		{ return m_acceptCtxPool.Acquire(); }
	};
}
#endif
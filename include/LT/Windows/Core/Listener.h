#ifndef __INCL_LYNTRA_LISTENER_H__
#define __INCL_LYNTRA_LISTENER_H__

#include <WinSock2.h>
#include <Windows.h>
#include <MSWSock.h>
#include <ws2tcpip.h>
#include <vector>
#include <atomic>
#include <sal.h>

#include <LT/Windows/Network/ConnectionManager.h>
#include <LT/Windows/Network/IOContext.h>
#include <LT/Windows/Network/IPEndPoint.h>
#include <LT/Common/Protocol/TProtocol.h>
#include <LT/Common/Utility/LTReturnObject.h>
#include <LT/Utility.h>
#include <LT/Windows/Network/Socket/LTSocket.h>
#include <LT/Windows/Network/Socket/SocketOption.h>
#include <LT/Windows/Core/ListenerConfig.h>

namespace LT
{
	typedef enum ListenerState
	{
		LISTENER_CREATED,
		LISTENER_BINDING,
		LISTENER_BOUND,
		LISTENER_LISTENING,
		LISTENER_CLOSING,
		LISTENER_CLOSED
	} LSTNRSTATE;

	class IListener {
	protected:
		LTSOCKET						m_listenSock;
		IPEndPoint						m_localEndpoint;
		UINT							m_protocol;

		LSTNRSTATE						m_state;
	public:
		IListener(
			_In_		const IPEndPoint&	_endpoint,
			_In_		UINT				_protocol) :
			m_localEndpoint(_endpoint),
			m_listenSock(LTSOCKET::INVALID()),
			m_protocol(_protocol),
			m_state(LISTENER_CREATED) {}

		IListener operator=(const IListener&)	= delete;
		IListener operator=(IListener&&)		= delete;

		virtual ~IListener() = default;

		virtual LTReturnObject Bind() { return{}; }
		virtual LTReturnObject Bind(_In_ HANDLE _comPort) { return{}; }
		virtual LTReturnObject Listen() { return{}; }
		virtual LTReturnObject Close() { return{}; }

		UINT				Protocol() const { return m_protocol; }
		LTSOCKET*			GetSocket() { return &m_listenSock; }
		const IPEndPoint&	LocalEndPoint() const { return m_localEndpoint; }
	};

	template<TProtocol>
	class Listener;

	template<>
	class Listener<TCP> : public IListener {
	private:
		static const int DEFAULT_ACCEPT_CONTEXT_SIZE = 50;
	private:
		LPFN_ACCEPTEX				m_lpAcceptEx;
		LPFN_GETACCEPTEXSOCKADDRS	m_lpGetAcceptExSockaddrs;
		UINT						m_backlog;

		UINT							m_acceptContextSize;
		LockFreePool<U_PTR<ACPTCTX>>	m_acceptContextPool;
		VEC<U_PTR<ACPTCTX>>				m_pendingAcceptContexts; // Pool Release Àü¿ë
	public:
		explicit Listener<TCP>(
			_In_		const IPEndPoint&	_endpoint,
			_In_opt_	UINT				_backlog			= SOMAXCONN,
			_In_opt_	UINT				_acceptContextSize	= DEFAULT_ACCEPT_CONTEXT_SIZE) :
			IListener(
				_endpoint,
				PROTOCOL_TCP),
			m_lpAcceptEx(nullptr),
			m_lpGetAcceptExSockaddrs(nullptr),
			m_backlog(_backlog),
			m_acceptContextSize(_acceptContextSize),
			m_acceptContextPool(_acceptContextSize),
			m_pendingAcceptContexts(_acceptContextSize) {}

		LTReturnObject Bind()						override;
		LTReturnObject Bind(_In_ HANDLE _comPort)	override;
		LTReturnObject Listen()						override;
		LTReturnObject Close()						override;

		LTReturnValue<BOOL>		PostAccept();					// Non-Blocking AcceptEx Based
		LTReturnObject			ReserveAccept();
		LTReturnValue<LTSOCKET> Accept(_In_ HANDLE _comPort);	// Blocking WSAAccept Based

		VOID GetAcceptExSockAddrs(
			_In_ PACPTCTX _acptCtx,
			_Out_ IPEndPoint* _localAdrPtr,
			_Out_ IPEndPoint* _remoteAdrPtr);
	private:
		LTReturnValue<BOOL> load_accept_extensions();
	};

	template<>
	class Listener<UDP> : public IListener {
	public:
		explicit Listener<UDP>(
			_In_		const IPEndPoint& _endpoint) :
			IListener(
				_endpoint,
				PROTOCOL_UDP) {}

		LTReturnObject Bind()						override;
		LTReturnObject Bind(_In_ HANDLE _comPort)	override;
		LTReturnObject Close()						override;
	};
	template<>
	class Listener<QUIC> : public IListener
	{
	public:
		explicit Listener<QUIC>(
			_In_		const IPEndPoint& _endpoint) :
			IListener(
				_endpoint,
				PROTOCOL_QUIC) {}
		LTReturnObject Bind()						override { return{}; }
		LTReturnObject Bind(_In_ HANDLE _comPort)	override { return{}; }
		LTReturnObject Close()						override { return{}; }
	};
	template<>
	class Listener<KCP> : public IListener
	{
	public:
		explicit Listener<KCP>(
			_In_		const IPEndPoint& _endpoint) :
			IListener(
				_endpoint,
				PROTOCOL_KCP) {}
		LTReturnObject Bind()						override { return{}; }
		LTReturnObject Bind(_In_ HANDLE _comPort)	override { return{}; }
		LTReturnObject Close()						override { return{}; }
	};
}
#endif //__INCL_LYNTRA_LISTENER_H__
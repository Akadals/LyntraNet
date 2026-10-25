#ifndef __INCL_LYNTRA_LISTENER_H__
#define __INCL_LYNTRA_LISTENER_H__

#include <WinSock2.h>
#include <Windows.h>
#include <MSWSock.h>
#include <ws2tcpip.h>
#include <vector>
#include <atomic>
#include <sal.h>

#include <LT/Common/IO/IBackendType.h>
#include <LT/Windows/Network/ConnectionManager.h>
#include <LT/Windows/Network/IOContext.h>
#include <LT/Windows/Network/IPEndPoint.h>
#include <LT/Common/Protocol/TProtocol.h>
#include <LT/Common/Utility/LTReturnObject.h>
#include <LT/Common/Utility/Concurrency/ConcurrentLockFreePool.h>
#include <LT/Common/Security/Security.h>
#include <LT/Windows/Network/Socket/LTSocket.h>
#include <LT/Windows/Core/ListenerConfig.h>

namespace LT
{
	typedef enum ListenerState
	{
		LSTNR_CREATED,
		LSTNR_BINDING,
		LSTNR_BOUND,
		LSTNR_LISTENING,
		LSTNR_CLOSING,
		LSTNR_CLOSED
	} LSTNRSTATE;

	class IListener {
	protected:
		LTSOCKET						m_listenSock;
		IPEndPoint						m_localEndpoint;
		ProtocolType					m_protocol;

		IBackend*						m_backend;

		LSTNRSTATE						m_state;
	public:
		IListener(
			_In_		ProtocolType		_protocol,
			_In_		const IPEndPoint&	_endpoint) :
			m_protocol		(_protocol),
			m_localEndpoint	(_endpoint),
			m_listenSock	(LTSOCKET::INVALID()),
			m_state			(LSTNR_CREATED),
			m_backend		(nullptr) {}

		virtual ~IListener() { m_backend = nullptr; }

		IListener operator=(const IListener&)	= delete;
		IListener operator=(IListener&&)		= delete;

		virtual LTReturnObject Bind()	{ return {}; }
		virtual LTReturnObject Listen()	{ return {}; }
		virtual LTReturnObject Close()	{ return {}; }

		ProtocolType		Protocol()		const	{ return m_protocol; }
		LTSOCKET			LTSocket()		const	{ return m_listenSock; }
		const LTSOCKET&		LTSocket()				{ return m_listenSock; }
		SOCKET				Socket()		const	{ return m_listenSock.m_sock; }
		const SOCKET&		Socket()				{ return m_listenSock.m_sock; }
		HANDLE				SocketHandle()	const	{ return reinterpret_cast<HANDLE>(m_listenSock.m_sock); }
		LSTNRSTATE			State()			const	{ return m_state; }
		const IPEndPoint&	LocalEndPoint() const	{ return m_localEndpoint; }

		void				SetBackend(IBackend* _backend) { m_backend = _backend; }
	};

	template<ProtocolType TProtocol>
	class Listener;

	template<>
	class Listener<ProtocolType::TCP> : public IListener {
	private:
		static constexpr int ACPT_CTX_SIZE = 50;
	private:
		LPFN_ACCEPTEX					m_lpAcceptEx;
		LPFN_GETACCEPTEXSOCKADDRS		m_lpGetAcceptExSockaddrs;
		UINT							m_backlog;

		UINT							m_acceptContextSize;
		LockFreePool<U_PTR<ACPTCTX>>	m_acceptContextPool;
		VEC<U_PTR<ACPTCTX>>				m_pendingAcceptContexts;

		BOOL							m_isUseTLS;
	public:
		explicit Listener<ProtocolType::TCP>(
			_In_		const IPEndPoint&	_endpoint,
			_In_opt_	UINT				_backlog			= SOMAXCONN,
			_In_opt_	UINT				_acceptContextSize	= ACPT_CTX_SIZE) :
			IListener				(ProtocolType::TCP, _endpoint),
			m_backlog				(_backlog),
			m_acceptContextSize		(_acceptContextSize),
			m_acceptContextPool		(_acceptContextSize),
			m_pendingAcceptContexts	(_acceptContextSize),
			m_lpAcceptEx			(nullptr),
			m_lpGetAcceptExSockaddrs(nullptr),
			m_isUseTLS(FALSE) {}

		LTReturnObject Bind()	override;
		LTReturnObject Listen()	override;
		LTReturnObject Close()	override;

		LTReturnValue<BOOL>		PostAccept();
		LTReturnObject			ReserveAccept();
		LTReturnValue<LTSOCKET> Accept();

		void GetAcceptExSockAddrs(
			_In_ PACPTCTX		_acptCtx,
			_Out_ IPEndPoint*	_localAdrPtr,
			_Out_ IPEndPoint*	_remoteAdrPtr);

		void useTLS(BOOL _bool, UINT8 _tlsVersion = TLS_1_3);
	private:
		LTReturnValue<BOOL> load_accept_extensions();
	};

	template<>
	class Listener<ProtocolType::UDP> : public IListener {
	public:
		explicit Listener<ProtocolType::UDP>(
			_In_		const IPEndPoint& _endpoint) :
			IListener(ProtocolType::UDP, _endpoint) {}

		LTReturnObject Bind()	override;
		LTReturnObject Close()	override;
	};
	template<>
	class Listener<ProtocolType::QUIC> : public IListener
	{
	public:
		explicit Listener<ProtocolType::QUIC>(
			_In_		const IPEndPoint& _endpoint) :
			IListener(ProtocolType::QUIC, _endpoint) {}
		LTReturnObject Bind()						override { return{}; }
		LTReturnObject Close()						override { return{}; }
	};
	template<>
	class Listener<ProtocolType::KCP> : public IListener
	{
	public:
		explicit Listener<ProtocolType::KCP>(
			_In_		const IPEndPoint& _endpoint) :
			IListener(ProtocolType::KCP, _endpoint) {}
		LTReturnObject Bind()						override { return{}; }
		LTReturnObject Close()						override { return{}; }
	};
	template<>
	class Listener<ProtocolType::RTP> : public IListener
	{
	public:
		explicit Listener<ProtocolType::RTP>(
			_In_		const IPEndPoint& _endpoint) :
			IListener(ProtocolType::RTP, _endpoint) {}
		LTReturnObject Bind()						override { return{}; }
		LTReturnObject Close()						override { return{}; }
	};
}
#endif //__INCL_LYNTRA_LISTENER_H__
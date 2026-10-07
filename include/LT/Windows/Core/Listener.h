#ifndef __INCL_LYNTRA_LISTENER_H__
#define __INCL_LYNTRA_LISTENER_H__

#include <WinSock2.h>
#include <Windows.h>
#include <MSWSock.h>
#include <ws2tcpip.h>
#include <LT/Network/ConnectionManager.h>
#include <LT/Network/IOContext.h>
#include <LT/Network/IPEndPoint.h>
#include <LT/Network/Protocol.h>
#include <LT/Utility/LTReturnObject.h>
#include <LT/Utility.h>
#include <LT/Network/Socket/LTSocket.h>
#include <LT/Windows/Core/ListenerConfig.h>
#include <vector>
#include <sal.h>

#define DEFAULT_POSTING_ACCEPT_DEPTH 50

namespace LT
{
	class IListener
	{
	protected:
		LTSOCKET						m_listenSock;
		IPEndPoint						m_localEndpoint;
		UINT							m_backlog;
		UINT							m_protocol;

		BOOL							m_isListening;

		LockFreePool<ACPTCTX>			m_acceptContextPool;
		std::vector<PACPTCTX>			m_acceptContexts;
	public:
		explicit IListener(
			_In_		const IPEndPoint&	_endpoint,
			_In_		UINT				_backlog,
			_In_		UINT				_protocol) :
			m_localEndpoint(_endpoint),
			m_listenSock(LTSOCKET::INVALID()),
			m_backlog(_backlog),
			m_protocol(_protocol),
			m_isListening(FALSE),
			m_acceptContextPool(DEFAULT_POSTING_ACCEPT_DEPTH),
			m_acceptContexts(DEFAULT_POSTING_ACCEPT_DEPTH) {}

		explicit IListener(
			_In_		const LISTENCFG& _config) :
			m_acceptContextPool(DEFAULT_POSTING_ACCEPT_DEPTH),
			m_acceptContexts(DEFAULT_POSTING_ACCEPT_DEPTH)
		{
			//config Àû¿ë
		}

		virtual ~IListener() = default;

		virtual LTReturnObject Bind()	= 0;
		virtual LTReturnObject Listen() = 0;
		virtual LTReturnObject Close()	= 0;

		UINT Protocol() const { return m_protocol; }
		const IPEndPoint& LocalEndPoint() const { return m_localEndpoint; }
	};

	template<TProtocol>
	class Listener;

	template<>
	class Listener<TCP> : public IListener
	{
	private:
		LPFN_ACCEPTEX				m_lpAcceptEx;
		LPFN_GETACCEPTEXSOCKADDRS	m_lpGetAcceptExSockaddrs;
		UINT						m_acceptDepth;
	public:
		explicit Listener<TCP>(
			_In_		const IPEndPoint&	_endpoint,
			_In_opt_	UINT				_backlog = SOMAXCONN,
			_In_opt_	UINT				_acceptDepth = DEFAULT_POSTING_ACCEPT_DEPTH) :
			IListener(_endpoint, _backlog, PROTOCOL_TCP),
			m_lpAcceptEx(nullptr),
			m_lpGetAcceptExSockaddrs(nullptr),
			m_acceptDepth(_acceptDepth) {}

		LTReturnObject Bind()	override;
		LTReturnObject Listen() override;
		LTReturnObject Close()	override;

		LTReturnValue<BOOL>		PostAccept();	// Non-Blocking AcceptEx Based
		LTReturnValue<LTSOCKET> Accept();		// Blocking WSAAccept Based
	private:
		LTReturnValue<BOOL> load_accept_extensions();
	};

	template<>
	class Listener<UDP> : public IListener
	{

	};
	template<>
	class Listener<QUIC> : public IListener
	{

	};
	template<>
	class Listener<KCP> : public IListener
	{

	};
}

#include "Listener/TCPListener.inl"
#endif //__INCL_LYNTRA_LISTENER_H__
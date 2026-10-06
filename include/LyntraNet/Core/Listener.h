#ifndef __INCL_LYNTRA_LISTENER_H__
#define __INCL_LYNTRA_LISTENER_H__

#include <WinSock2.h>
#include <Windows.h>
#include <MSWSock.h>
#include <ws2tcpip.h>
#include <LyntraNet/Network/ConnectionManager.h>
#include <LyntraNet/Network/IOContext.h>
#include <LyntraNet/Network/IPEndPoint.h>
#include <LyntraNet/Network/Protocol.h>
#include <LyntraNet/Utility/LTReturnObject.h>
#include <LyntraNet/Network/Socket/LTSocket.h>
#include <sal.h>

namespace LT
{
	class IListener
	{
	protected:
		IPEndPoint m_localEndpoint;
		int m_backlog;
	protected:
		LTSOCKET m_listenSock;
	public:
		explicit IListener(
			_In_		const IPEndPoint& _endpoint,
			_In_		int _backlog) :
			m_localEndpoint(_endpoint),
			m_listenSock(INVALID_SOCKET),
			m_backlog(_backlog) {}

		virtual LTReturnObject Bind() = 0;
		virtual LTReturnObject Listen() = 0;
		virtual LTReturnObject Close() = 0;

		virtual LTSOCKET Accept() = 0;
	};

	template<TProtocol>
	class Listener;

	template<>
	class Listener<TCP> : public IListener
	{
	private:
		LPFN_ACCEPTEX m_lpAcceptEx;
		LPFN_GETACCEPTEXSOCKADDRS m_lpGetAcceptExSockaddrs;
	public:
		explicit Listener<TCP>(
			_In_		const IPEndPoint& _endpoint, 
			_In_opt_	int _backlog = 0) :
			IListener(_endpoint, _backlog) {}

		LTReturnObject Bind() override;
		LTReturnObject Listen() override;
		LTReturnObject Close() override;

		LTSOCKET Accept() override;
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
#endif
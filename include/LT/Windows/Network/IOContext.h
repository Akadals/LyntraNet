#ifndef __INCL_LYNTRA_IOCONTEXT_H__
#define __INCL_LYNTRA_IOCONTEXT_H__

#include <winsock2.h>
#include <atomic>
#include <LT/Windows/Network/Socket/LTSocket.h>
#include <LT/Windows/Network/IPAddress.h>
#include <LT/Windows/Core/Transport.h>
//#include <LT/Windows/Core/Listener.h>

namespace LT
{
	template<typename T>
	using ATOMIC = std::atomic<T>;

	typedef enum class IOType : uint8_t
	{ READING, WRITING } IOTYPE;

	typedef enum ContextState
	{
		CONTEXT_PENDING,
		CONTEXT_AVAILABLE,
		CONTEXT_COMPLETED,
		CONTEXT_PROCESSING
	} CTXSTATE; //State 수정 필요

	typedef enum AcceptMode
	{
		ACCEPT_EX,
		WSA_ACCEPT
	} ACPTMOD;

	class IListener;

	struct alignas(64) IContext 
	{	
		OVERLAPPED			m_overlapped;
		ATOMIC<CTXSTATE>	m_contextState;
		IListener*			m_ownerListener;

		IContext() :
			m_overlapped{},
			m_contextState(CONTEXT_READY),
			m_ownerListener(nullptr) {}
		virtual ~IContext() = default;
	};

	typedef struct AcceptContext
		ACPTCTX, * PACPTCTX;

	typedef struct IOContext
		IOCTX, * PIOCTX;

	struct alignas(64) AcceptContext : public IContext
	{
	private:
		static constexpr DWORD ADDRESS_LENGTH =
			sizeof(SOCKADDR_STORAGE) + 16;

		static constexpr DWORD BUFFER_LENGTH =
			ADDRESS_LENGTH * 2;
	public:
		ACPTMOD		m_acceptMode;

		LTSOCKET	m_acceptSock;
		CHAR		m_buffer[BUFFER_LENGTH];

		IPEndPoint	m_localAddress;		//Accept에서만 사용 PostAccept는 사용하지않음
		IPEndPoint	m_remoteAddress;	//Accept에서만 사용 PostAccept는 사용하지않음

		AcceptContext() :
			m_acceptMode(ACCEPT_EX),
			m_acceptSock(LTSOCKET::INVALID()),
			m_localAddress{},
			m_remoteAddress{} {
			ZeroMemory(&m_overlapped, sizeof(OVERLAPPED));
		}
	};

	struct alignas(64) IOContext : public IContext
	{
		IOTYPE m_ioType;
		WSABUF m_wsaBuf[2];

		ITransport* m_ownerTransport;

		IOContext() : 
			m_ownerTransport(nullptr),
			m_wsaBuf{} {}
	};
}
#endif
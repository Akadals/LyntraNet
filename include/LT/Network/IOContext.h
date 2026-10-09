#ifndef __INCL_LYNTRA_IOCONTEXT_H__
#define __INCL_LYNTRA_IOCONTEXT_H__

#include <winsock2.h>
#include <LT/Network/Socket/LTSocket.h>
#include <LT/Network/IPAddress.h>

namespace LT
{
	typedef enum class IOType : uint8_t
	{ READING, WRITING } IOTYPE;

	typedef enum ContextState
	{
		CONTEXT_PENDING,
		CONTEXT_AVAILABLE,
		CONTEXT_COMPLETED
	} CTXSTATE;

	struct IContext
	{
		IPAddress	m_localAddress;
		IPAddress	m_remoteAddress;
		OVERLAPPED	m_overlapped;
	};

	typedef struct AcceptContext
		ACPTCTX, * PACPTCTX;

	typedef struct IOContext
		IOCTX, * PPIOCTX;

	struct alignas(64) AcceptContext : public IContext
	{
		LTSOCKET	m_acceptSock;
		PCHAR		m_buffer;
		CTXSTATE	m_contextState;

		AcceptContext() :
			m_acceptSock(LTSOCKET::INVALID()) {}
	};

	struct alignas(64) IOContext : public IContext
	{
		WSABUF m_wsaBuf[2];
		IOTYPE m_ioType;
		SOCKET m_ownerSock;

		IOContext();
		void Init();
	};
}
#endif
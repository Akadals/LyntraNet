#ifndef __INCL_LYNTRA_IOCONTEXT_H__
#define __INCL_LYNTRA_IOCONTEXT_H__

#include <winsock2.h>
#include <LT/Network/Socket/LTSocket.h>

namespace LT
{
	typedef enum class IOType : uint8_t
	{ READING, WRITING } IOTYPE;


	typedef struct AcceptContext
		ACPTCTX, * PACPTCTX;

	typedef struct IOContext
		IOCTX, * PPIOCTX;


	struct alignas(64) AcceptContext
	{
		LTSOCKET m_AcceptSock;
		WSABUF m_wsaBuf;
		CHAR* buffer;
		OVERLAPPED m_overlapped;

		AcceptContext() :
			m_AcceptSock(LTSOCKET::INVALID()) {}
	};

	struct alignas(64) IOContext
	{
		OVERLAPPED m_overlapped = {};
		WSABUF m_wsaBuf[2] = {};
		IOTYPE m_ioType = {};
		SOCKET m_ownerSock = { INVALID_SOCKET };

		IOContext();
		void Init();
	};
}
#endif
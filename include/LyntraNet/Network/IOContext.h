#ifndef __INCL_LYNTRA_IOCONTEXT_H__
#define __INCL_LYNTRA_IOCONTEXT_H__

#include <winsock2.h>

namespace LT
{
	typedef enum class IOType : uint8_t
	{ READING, WRITING } IOTYPE;

	typedef class AcceptContext
		ACCEPTCONTEXT, * PACCEPTCONTEXT;

	typedef class IOContext
		IOCONTEXT, * PIOCONTEXT;

	class alignas(64) AcceptContext
	{
	public:
		OVERLAPPED m_overlapped = {};
		WSABUF m_wsaBuf = {};
		SOCKET m_ownerSock = { INVALID_SOCKET };
	public:
		AcceptContext();
		void Init();
	};

	class alignas(64) IOContext
	{
	public:
		OVERLAPPED m_overlapped = {};
		WSABUF m_wsaBuf[2] = {};
		IOTYPE m_ioType = {};
		SOCKET m_ownerSock = { INVALID_SOCKET };
	public:
		IOContext();
		void Init();
	};
}
#endif
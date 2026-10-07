#include <LT/Network/IOContext.h>

using namespace LT;
void IOContext::Init()
{
	ZeroMemory(&m_overlapped, sizeof(OVERLAPPED));
	m_wsaBuf->buf = nullptr;
	m_wsaBuf->len = 0;
	m_ioType = IOType::READING;
	m_ownerSock = INVALID_SOCKET;
}

IOContext::IOContext() { Init(); }
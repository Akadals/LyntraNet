#include "LT/Network/NetworkConnection.h"

using namespace LT;

DWORD NetworkConnection::Read()
{
	auto region =
		m_recvBuf.AcquireWriteRegion();
	if (region.Empty()) return 0;

	PPIOCTX ctx = 
		m_transport->GetSocket().AcquireContext();

	memset(&ctx->m_overlapped, 0, sizeof(OVERLAPPED));
	ctx->m_ioType = IOTYPE::READING;

	ctx->m_wsaBuf[0].buf = 
		reinterpret_cast<char*>(region.firstPtr);
	ctx->m_wsaBuf[0].len = 
		static_cast<ULONG>(region.firstSize);
	ctx->m_wsaBuf[1].buf = 
		reinterpret_cast<char*>(region.secondPtr);
	ctx->m_wsaBuf[1].len =
		static_cast<ULONG>(region.secondSize);

	return m_transport->GetSocket().Recv(*ctx);
}

DWORD NetworkConnection::Write()
{
	return 0;
}

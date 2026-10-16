#include <LT/Windows/Core/IOCompletionPort.h>

using namespace LT;

LTReturnObject IOCompletionPort::Startup()
{

	GetSystemInfo(&m_sysInfo);
	m_processorSize =
		m_sysInfo.dwNumberOfProcessors *
		THREAD_PER_PROCESSOR;

	if (!WSAStartup(MAKEWORD(2, 2), &m_wsaData))
		return {
			RETOBJ_RSN_STARTUP,
			RETOBJ_ERR_TYPE_OS,
			RETOBJ_MSG_INVALID };

	if ((m_comPort = CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,
		NULL, 0, m_processorSize)) == NULL)
		return {
			RETOBJ_RSN_CICP,
			RETOBJ_ERR_TYPE_OS,
			RETOBJ_MSG_INVALID };

	return {};
}

LTReturnObject IOCompletionPort::OnAcceptCompleted(PACPTCTX _acptCtx)
{
	auto conn = m_connectionManager.NewConnection();
	auto transport = dynamic_cast<Transport<TCP>*>(conn->GetTransport());
	if (!transport)
		return {
			RETOBJ_RSN_ACPT,
			RETOBJ_ERR_TYPE_OS,
			RETOBJ_MSG_INVALID };

	transport->Set(_acptCtx->m_acceptSock);

	auto listener = dynamic_cast<Listener<TCP>*>(_acptCtx->m_ownerListener);
	if (!listener)
		return {
			RETOBJ_RSN_ACPT,
			RETOBJ_ERR_TYPE_OS,
			RETOBJ_MSG_INVALID };

	switch (_acptCtx->m_acceptMode) {
	case ACCEPT_EX:
		listener->GetAcceptExSockAddrs(
			_acptCtx,
			transport->GetLocalAddress(),
			transport->GetRemoteAddress());
		break;
	case WSA_ACCEPT:
		transport->GetLocalAddress()->Set(
			_acptCtx->m_localAddress);
		transport->GetRemoteAddress()->Set(
			_acptCtx->m_remoteAddress);
		break;
	}

	transport->StartRecv(); //추가 처리 필요

	_acptCtx->m_contextState.store(CONTEXT_COMPLETED);
}

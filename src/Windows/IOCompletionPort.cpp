#include <LT/Windows/Core/IOCompletionPort.h>

namespace LT {
	IOCompletionPort::IOCompletionPort() {
		GetSystemInfo(&m_sysInfo);
		m_processorSize =
			m_sysInfo.dwNumberOfProcessors *
			THREAD_PER_PROCESSOR;
	}

	LTReturnObject IOCompletionPort::Startup() {
		if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0)
			return {
				RETOBJ_RSN_STARTUP,
				RETOBJ_ERR_TYPE_OS };

		if ((m_comPort = CreateIoCompletionPort(
			INVALID_HANDLE_VALUE,
			NULL, 0, m_processorSize)) == NULL)
			return {
				RETOBJ_RSN_CICP,
				RETOBJ_ERR_TYPE_OS };

		for (int i = 0; i < m_processorSize; i++)
			_beginthreadex(NULL, 0, ThreadEntry, this, 0, NULL);

		std::cout << "Startup Completed\n";
		return {};
	}

	LTReturnObject IOCompletionPort::OnAcceptCompleted(PACPTCTX _acptCtx) {
		auto conn = m_connectionManager.NewConnection();
		auto transport = std::make_unique<Transport<TCP>>();//dynamic_cast<Transport<TCP>*>(conn->GetTransport());
		if (!transport)
			return {
				RETOBJ_RSN_ACPT,
				RETOBJ_ERR_TYPE_OS };

		transport->Set(_acptCtx->m_acceptSock);

		auto listener = dynamic_cast<Listener<TCP>*>(_acptCtx->m_ownerListener);
		if (!listener)
			return {
				RETOBJ_RSN_ACPT,
				RETOBJ_ERR_TYPE_OS };

		switch (_acptCtx->m_acceptMode) {
		case ACCEPT_EX:
			if (setsockopt(_acptCtx->m_acceptSock.m_sock, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
				(char*)listener->GetSocket(), sizeof(listener->GetSocket())) == SOCKET_ERROR)
			{
				closesocket(_acptCtx->m_acceptSock.m_sock);
				//예외처리
			}

			listener->GetAcceptExSockAddrs(
				_acptCtx,
				transport->GetLocalAddress(),
				transport->GetRemoteAddress());
			listener->PostAccept();
			break;
		case WSA_ACCEPT:
			transport->GetLocalAddress()->Set(
				_acptCtx->m_localAddress);
			transport->GetRemoteAddress()->Set(
				_acptCtx->m_remoteAddress);
			break;
		}

		std::cout << transport->GetRemoteAddress()->GetIPAddress().ToString() << " : " <<
			transport->GetRemoteAddress()->Port() << " Protocol: " << listener->Protocol();

		//transport->StartRecv(); //추가 처리 필요

		_acptCtx->m_contextState.store(CONTEXT_COMPLETED);
		return {};
	}
	unsigned __stdcall IOCompletionPort::ThreadEntry(void* _this) {
		auto* self = static_cast<IOCompletionPort*>(_this);

		return self->Worker();
	}
	unsigned WINAPI IOCompletionPort::Worker() { //임시 스레드
		DWORD byteTrans = 0;
		ULONG_PTR completionKey = 0;
		LPOVERLAPPED overlapped = nullptr;
		DWORD64 sessionID = 0;
		BOOL res = FALSE;

		while (true)
		{
			byteTrans = 0;
			completionKey = 0;
			overlapped = nullptr;

			res = GQCS(m_comPort, &byteTrans, &completionKey, &overlapped, INFINITE);
			DWORD error = res ? ERROR_SUCCESS : GetLastError();

			if (res)
			{
				IContext* context = CONTAINING_RECORD(overlapped, IContext, m_overlapped);
				if (auto* accept = dynamic_cast<ACPTCTX*>(context))
					OnAcceptCompleted(accept);
				else if (auto* io = dynamic_cast<IOCTX*>(context))
				{

				}
			}

		}
		return 0;
	}
}
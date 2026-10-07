#include <LT/Core/Listener.h>

using namespace LT;

//void LT::Listener<TCP>::Listen(size_t _size)
//{
//	listen(m_listenSock, _size); //예외처리
//
//	LoadAcceptEx();
//	for (size_t i = 0; i < _size; i++)
//		PostAccept();
//}
//
//void Listener<TCP>::LoadAcceptEx()
//{
//	DWORD bytes = 0;
//	auto loadExtension = [&](GUID guid, void** fnPtr)
//		{ WSAIoctl(m_listenSock, SIO_GET_EXTENSION_FUNCTION_POINTER,
//			&guid, sizeof(guid), fnPtr, sizeof(*fnPtr),
//			&bytes, NULL, NULL); };
//
//	loadExtension(WSAID_ACCEPTEX, 
//		reinterpret_cast<void**>(&m_lpAcceptEx));
//	loadExtension(WSAID_GETACCEPTEXSOCKADDRS, 
//		reinterpret_cast<void**>(&m_lpGetAcceptExSockaddrs));
////}
//
//void Listener<TCP>::PostAccept()
//{
	//PACCEPTCONTEXT ctx = new ACCEPTCONTEXT; //OBJ pool 적용

	//if ((ctx->m_ownerSock = WSASocket(
	//	AF_INET,
	//	SOCK_STREAM,
	//	0, NULL, 0,
	//	WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	//{
	//	delete ctx;
	//	return; //loging
	//}

	//DWORD bytes = 0;
	//DWORD dwLen = sizeof(SOCKADDR_IN) + 16;

	//if (!m_lpAcceptEx(m_listenSock, ctx->m_ownerSock, ctx->m_wsaBuf.buf, 0,
	//	dwLen, dwLen, &bytes, &ctx->m_overlapped)) return; //loging
//}
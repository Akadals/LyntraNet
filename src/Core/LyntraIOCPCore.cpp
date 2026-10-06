#include <LyntraNet/Core/LyntraIOCPCore.h>
using namespace LT;
//Core::Core(std::unique_ptr<Listener> _listener)
//{
//	
//}
//void Core::Startup()
//{
//	SYSTEM_INFO sysInfo;
//	GetSystemInfo(&sysInfo);
//	size_t threadSize = static_cast<size_t>(sysInfo.dwNumberOfProcessors) * 2 + 1;
//
//	WSADATA wsaData;
//
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return;
//	if ((m_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL) return;
//	if ((m_listenSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) return;
//	CreateIoCompletionPort((HANDLE)m_listenSock, m_iocp, (ULONG_PTR)0, 0);
//
//	memset(&m_listenAdr, 0, sizeof(m_listenAdr));
//
//	m_listenAdr.sin_family = AF_INET;
//	m_listenAdr.sin_addr.s_addr = htonl(INADDR_ANY);
//	m_listenAdr.sin_port = htons(_port);
//
//	if (::bind(m_listenSock, (PSOCKADDR)&m_listenAdr, sizeof(m_listenAdr)) == SOCKET_ERROR) return;
//
//	if (listen(m_listenSock, 100) == SOCKET_ERROR) return;
//}

//DWORD __stdcall Core::Worker(PVOID args)
//{
//	DWORD byteTrans = 0;
//	ULONG_PTR completionKey = 0;
//	LPOVERLAPPED overlappedPtr = nullptr;
//	BOOL res = false;
//
//	while (true)
//	{
//		byteTrans = 0;
//		completionKey = 0;
//		overlappedPtr = nullptr;
//
//		res = GetQueuedCompletionStatus(m_iocp, &byteTrans, &completionKey, &overlappedPtr, INFINITE);
//		if (!overlappedPtr)
//		{
//			//shutdown
//			if (!completionKey)
//			{
//			}
//			else
//			{
//				//logging
//			}
//			continue;
//		}
//
//		if (overlappedPtr == (LPOVERLAPPED)0xffffffff)
//		{
//			//disconnect
//			continue;
//		}
//
//		if (res)
//		{
//		}
//	}
//	return 0;
//}
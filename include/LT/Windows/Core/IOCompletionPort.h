#ifndef __INCL_IO_COMPLETION_PORT_H__
#define __INCL_IO_COMPLETION_PORT_H__

#include <LT/Common/IO/IBackendType.h>
#include <LT/Windows/Network/ConnectionManager.h>
#include <LT/Windows/Core/Listener.h>
#include <LT/Windows/Network/IOContext.h>
#include <LT/Common/Utility/LTReturnObject.h>

namespace LT
{
	class IOCompletionPort : public IBackend
	{
	private:
		static const int THREAD_PER_PROCESSOR = 4;
	private:
		HANDLE m_completionPort;
		CONNMGR m_connectionManager;

		WSADATA m_wsaData;
		SYSTEM_INFO m_sysInfo;
		DWORD m_processorSize;
		HANDLE m_comPort;
	public:
		IOCompletionPort();
		LTReturnObject Startup() override;
		HANDLE* GetComPort() { return &m_comPort; }
	private:
		LTReturnObject OnAcceptCompleted(PACPTCTX _acptCtx); //TCP±â¹Ý
		
		static unsigned WINAPI ThreadEntry(void* _this);
		unsigned WINAPI Worker();
	};
}

#endif
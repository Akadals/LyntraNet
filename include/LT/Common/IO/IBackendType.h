#ifndef __INCL_LYNTRA_INTERFACE_BACKEND_TYPE_H__
#define __INCL_LYNTRA_INTERFACE_BACKEND_TYPE_H__

#include <LT/Common/Utility/LTReturnObject.h>
#include <LT/Windows/Network/ConnectionManager.h>

namespace LT
{
	enum class BackendType : uint8_t {
		NONE,
		IOCP,
		EPOLL,
		KQUEUE
	};
	class IBackend {
	protected:
		CONNMGR m_connectionManager;
		BackendType m_backendType;
		std::optional<HANDLE> m_backendHandle;
	public:
		IBackend(BackendType _backendType) :
			m_connectionManager(),
			m_backendType(_backendType) {}
		virtual ~IBackend() = default;
		virtual LTReturnObject Startup() = 0;

		HANDLE Handle() const { return m_backendHandle.value_or(INVALID_HANDLE_VALUE); }
	};
}

#endif
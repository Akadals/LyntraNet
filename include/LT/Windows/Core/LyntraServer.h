#ifndef __INCL_LYNTRA_SERVER_H__
#define __INCL_LYNTRA_SERVER_H__

#include <memory>
#include <vector>
#include <atomic>

#include <LT/Common/IO/IBackendType.h>
#include <LT/Windows/Core/IOCompletionPort.h>
#include <LT/Common/Protocol/TProtocol.h>
#include <LT/Windows/Core/Listener.h>
#include <LT/Common/Utility/LTReturnObject.h>
#include <LT/Common/Preset/ServerPreset.h>

namespace LT
{
	enum class IOBackendType
	{
		None,
		IOCP,
		EPOLL,
		IO_URING,
		KQUEUE
	}; //임시 (플랫폼별로 사용 가능 타입 선언 예정)
	class LyntraServer
	{
	public:
		static const size_t MAX_LISTENER_SIZE = 10;
	private:
		IOBackendType m_ioBackendType;
		U_PTR<IBackend> m_backend;

		std::vector<U_PTR<IListener>> m_listeners;
		std::atomic<bool> m_isRunning = false;
	public:
		LyntraServer() = default;
		LyntraServer(Preset _preset) {};
		LyntraServer(IOBackendType _backendType) :
			m_ioBackendType(_backendType)
		{
			switch (m_ioBackendType) {
			case IOBackendType::IOCP:
				m_backend = std::make_unique<IOCompletionPort>();
				break;
			case IOBackendType::EPOLL: break;
			case IOBackendType::IO_URING: break;
			case IOBackendType::KQUEUE: break;
			}
		}

		LTReturnObject Start();
		LTReturnObject Stop() { return{}; };
		void Wait() {};

		template<TProtocol T>
		LTReturnObject AddListener(
			_In_ const IPEndPoint& _endpoint);
		template<TProtocol T>
		LTReturnObject AddListener(
			_In_ const IPAddress& _address,
			_In_ UINT16 _port);
		template<TProtocol T>
		LTReturnObject AddListener(
			_In_ UINT16 _port);
	};
}

#endif
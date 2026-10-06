#ifndef __INCL_LYNTRA_SERVER_H__
#define __INCL_LYNTRA_SERVER_H__

#include <memory>
#include <vector>
#include <atomic>
#include <LyntraNet/Network/IPEndPoint.h>
#include <LyntraNet/Network/Protocol.h>
#include <LyntraNEt/Core/Listener.h>
#include <LyntraNet/Utility/LTReturnObject.h>

#include <LyntraNet/Preset/ServerPreset.h>

namespace LT
{
	class LyntraServer
	{
	public:
		static const size_t MAX_LISTENER_SIZE = 10;
	private:
		std::vector<std::unique_ptr<IListener>> m_listeners;
		std::atomic<bool> m_isRunning = false;
	public:
		LyntraServer() = default;
		LyntraServer(Preset _preset);

		LTReturnObject Start();
		LTReturnObject Stop();
		void Wait();

		template<TProtocol T> 
		LTReturnObject AddListener(const IPEndPoint& _endpoint);
		template<TProtocol T> 
		LTReturnObject AddListener(const IPAddress& _address, uint16_t _port);
		template<TProtocol T> 
		LTReturnObject AddListener(uint16_t _port);
	};
}

#endif
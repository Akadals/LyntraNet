#include <LT/Windows/Core/LyntraServer.h>

namespace LT {
	template<ProtocolType TProtocol>
	LTReturnObject LyntraServer::AddListener(const IPEndPoint& _endpoint) {
		for (auto& listener : m_listeners) {
			if (_endpoint.Port() == listener->LocalEndPoint().Port())
				return {}; //포트중복
		}
		auto listener = std::make_unique<Listener<TProtocol>>(_endpoint); //일단 기본값
		m_listeners.emplace_back(std::move(listener));
		return {};
	}
	template<ProtocolType TProtocol>
	LTReturnObject LT::LyntraServer::AddListener(UINT16 _port) {
		for (auto& listener : m_listeners) {
			if (_port == listener->LocalEndPoint().Port())
				return {}; //포트중복
		}
		auto listener = std::make_unique<Listener<TProtocol>>(
			IPEndPoint(IPAddress::AnyIPv4(), _port)); //일단 기본값
		m_listeners.emplace_back(std::move(listener));
		return {};
	}
	template<ProtocolType TProtocol>
	LTReturnObject LyntraServer::AddListener(const IPAddress& _address, UINT16 _port) {
		for (auto& listener : m_listeners) {
			if (_port == listener->LocalEndPoint().Port())
				return {}; //포트중복
		}
		auto listener = std::make_unique<Listener<TProtocol>>(
			IPEndPoint(_address, _port)); //일단 기본값
		m_listeners.emplace_back(std::move(listener));
		return {};
	}

	LTReturnObject LyntraServer::Start() {
		auto ret = m_backend->Startup();
		if (ret.StatusCode() != 0)
			return ret;
		for (auto& listener : m_listeners) {
			switch (listener->Protocol()) {
			case ProtocolType::TCP: {
				auto tcpListner = dynamic_cast<Listener<ProtocolType::TCP>*>(listener.get());
				tcpListner->SetBackend(m_backend.get());
				std::cout << tcpListner->Bind().Message(); //출력방식 변경 필요
				std::cout << tcpListner->Listen().Message();
				std::cout << tcpListner->ReserveAccept().Message();
				break;
			}
			case ProtocolType::UDP: {
				auto udpListner = dynamic_cast<Listener<ProtocolType::UDP>*>(listener.get());
				udpListner->SetBackend(m_backend.get());
				std::cout << udpListner->Bind().Message();
				break;
			}
			case ProtocolType::QUIC:
				break;
			case ProtocolType::KCP:
				break;
			case ProtocolType::RTP:
				break;
			case ProtocolType::SRTP:
				break;
			}
		}
		std::cout << "Listener Working\n";
		return {};
	}


	template LTReturnObject LyntraServer::AddListener<ProtocolType::TCP>(const IPEndPoint& _endpoint);
	template LTReturnObject LyntraServer::AddListener<ProtocolType::UDP>(const IPEndPoint& _endpoint);
	template LTReturnObject LyntraServer::AddListener<ProtocolType::QUIC>(const IPEndPoint& _endpoint);
	template LTReturnObject LyntraServer::AddListener<ProtocolType::KCP>(const IPEndPoint& _endpoint);


	template LTReturnObject LyntraServer::AddListener<ProtocolType::TCP>(const IPAddress& _address, UINT16 _port);
	template LTReturnObject LyntraServer::AddListener<ProtocolType::UDP>(const IPAddress& _address, UINT16 _port);
	template LTReturnObject LyntraServer::AddListener<ProtocolType::QUIC>(const IPAddress& _address, UINT16 _port);
	template LTReturnObject LyntraServer::AddListener<ProtocolType::KCP>(const IPAddress& _address, UINT16 _port);

	template LTReturnObject LyntraServer::AddListener<ProtocolType::TCP>(UINT16 _port);
	template LTReturnObject LyntraServer::AddListener<ProtocolType::UDP>(UINT16 _port);
	template LTReturnObject LyntraServer::AddListener<ProtocolType::QUIC>(UINT16 _port);
	template LTReturnObject LyntraServer::AddListener<ProtocolType::KCP>(UINT16 _port);
}
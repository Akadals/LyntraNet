#include <LT/Windows/Core/LyntraServer.h>

using namespace LT;

template<TProtocol T>
LTReturnObject LyntraServer::AddListener(const IPEndPoint& _endpoint) {
	for (auto& listener : m_listeners) {
		if (_endpoint.Port() == listener->LocalEndPoint().Port())
			return {}; //포트중복
	}
	auto listener = std::make_unique<Listener<T>>(_endpoint); //일단 기본값
	m_listeners.emplace_back(std::move(listener));
	return {};
}
template<TProtocol T>
LTReturnObject LT::LyntraServer::AddListener(UINT16 _port) {
	for (auto& listener : m_listeners) {
		if (_port == listener->LocalEndPoint().Port())
			return {}; //포트중복
	}
	auto listener = std::make_unique<Listener<T>>(
		IPEndPoint(IPAddress::AnyIPv4(), _port)); //일단 기본값
	m_listeners.emplace_back(std::move(listener));
	return {};
}
template<TProtocol T>
LTReturnObject LyntraServer::AddListener(const IPAddress& _address, UINT16 _port) {
	for (auto& listener : m_listeners) {
		if (_port == listener->LocalEndPoint().Port())
			return {}; //포트중복
	}
	auto listener = std::make_unique<Listener<T>>(
		IPEndPoint(_address, _port)); //일단 기본값
	m_listeners.emplace_back(std::move(listener));
	return {};
}

LTReturnObject LyntraServer::Start() {
	auto backend = dynamic_cast<IOCompletionPort*>(m_backend.get());
	auto ret = backend->Startup();
	if (ret.StatusCode() != RETOBJ_STATE_NONE)
		return ret;
	for (auto& listener : m_listeners) {
		switch (listener->Protocol()) {
		case PROTOCOL_TCP: {
			auto tcpListner = dynamic_cast<Listener<TCP>*>(listener.get());
			std::cout << tcpListner->Bind(*backend->GetComPort()).Message(); //출력방식 변경 필요
			std::cout << tcpListner->Listen().Message();
			std::cout << tcpListner->ReserveAccept().Message();
			break;
		}
		case PROTOCOL_UDP: {
			auto udpListner = dynamic_cast<Listener<UDP>*>(listener.get());
			std::cout << udpListner->Bind(backend->GetComPort()).Message();
			break;
		}
		case PROTOCOL_QUIC:
			break;
		case PROTOCOL_KCP:
			break;
		}
	}
	std::cout << "Listener Working\n";
	return {};
}


template LTReturnObject LyntraServer::AddListener<TCP>(const IPEndPoint& _endpoint);
template LTReturnObject LyntraServer::AddListener<UDP>(const IPEndPoint& _endpoint);
template LTReturnObject LyntraServer::AddListener<QUIC>(const IPEndPoint& _endpoint);
template LTReturnObject LyntraServer::AddListener<KCP>(const IPEndPoint& _endpoint);


template LTReturnObject LyntraServer::AddListener<TCP>(const IPAddress& _address, UINT16 _port);
template LTReturnObject LyntraServer::AddListener<UDP>(const IPAddress& _address, UINT16 _port);
template LTReturnObject LyntraServer::AddListener<QUIC>(const IPAddress& _address, UINT16 _port);
template LTReturnObject LyntraServer::AddListener<KCP>(const IPAddress& _address, UINT16 _port);

template LTReturnObject LyntraServer::AddListener<TCP>(UINT16 _port);
template LTReturnObject LyntraServer::AddListener<UDP>(UINT16 _port);
template LTReturnObject LyntraServer::AddListener<QUIC>(UINT16 _port);
template LTReturnObject LyntraServer::AddListener<KCP>(UINT16 _port);
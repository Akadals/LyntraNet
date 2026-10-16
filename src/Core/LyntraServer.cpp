#include <LT/Windows/Core/LyntraServer.h>

using namespace LT;

template<TProtocol T>
LTReturnObject LyntraServer::AddListener(const IPEndPoint& _endpoint)
{
	auto listener = std::make_unique<Listener<T>>(_endpoint); //일단 기본값
	m_listeners.emplace_back(std::move(listener));
	return LTReturnObject();
}
template<TProtocol T>
LTReturnObject LyntraServer::AddListener(const IPAddress& _address, uint16_t _port)
{
	auto listener = std::make_unique<Listener<T>>(
		IPEndPoint(_address, _port)); //일단 기본값
	m_listeners.emplace_back(std::move(listener));
	return LTReturnObject();
}
template<TProtocol T>

LTReturnObject LyntraServer::AddListener(uint16_t _port, UINT16 _ipVersion)
{
	auto listener = std::make_unique<Listener<T>>(
		IPEndPoint(
			_ipVersion == IPV4 ? 
			IPAddress::AnyIPv4() : 
			IPAddress::AnyIPv6(), 
			_port)); //일단 기본값
	m_listeners.emplace_back(std::move(listener));
	return LTReturnObject();
}

LTReturnObject LyntraServer::Start()
{
	auto backend = dynamic_cast<IOCompletionPort*>(m_backend.get());
	auto ret = backend->Startup();
	if (ret.StatusCode() != RETOBJ_STATE_NONE)
		return ret;

	for (auto& listener : m_listeners)
	{
		switch (listener->Protocol())
		{
		case PROTOCOL_TCP: {
			auto tcpListner = dynamic_cast<Listener<TCP>*>(listener.get());
			tcpListner->Bind(backend->GetComPort());
			tcpListner->Listen();
			tcpListner->PostAccept(); //Accept() 처리도 필요
			break;
		}
		case PROTOCOL_UDP: {
			auto udpListner = dynamic_cast<Listener<UDP>*>(listener.get());
			udpListner->Bind(backend->GetComPort());
			break;
		}
		case PROTOCOL_QUIC:
			break;
		case PROTOCOL_KCP:
			break;
		}
	}
}


template LTReturnObject LyntraServer::AddListener<TCP>(const IPEndPoint& _endpoint);
template LTReturnObject LyntraServer::AddListener<UDP>(const IPEndPoint& _endpoint);
template LTReturnObject LyntraServer::AddListener<QUIC>(const IPEndPoint& _endpoint);
template LTReturnObject LyntraServer::AddListener<KCP>(const IPEndPoint& _endpoint);


template LTReturnObject LyntraServer::AddListener<TCP>(const IPAddress& _address, uint16_t _port);
template LTReturnObject LyntraServer::AddListener<UDP>(const IPAddress& _address, uint16_t _port);
template LTReturnObject LyntraServer::AddListener<QUIC>(const IPAddress& _address, uint16_t _port);
template LTReturnObject LyntraServer::AddListener<KCP>(const IPAddress& _address, uint16_t _port);

template LTReturnObject LyntraServer::AddListener<TCP>(uint16_t _port, UINT16 _ipVersion);
template LTReturnObject LyntraServer::AddListener<UDP>(uint16_t _port, UINT16 _ipVersion);
template LTReturnObject LyntraServer::AddListener<QUIC>(uint16_t _port, UINT16 _ipVersion);
template LTReturnObject LyntraServer::AddListener<KCP>(uint16_t _port, UINT16 _ipVersion);
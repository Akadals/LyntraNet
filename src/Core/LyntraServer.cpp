#include <LT/Core/LyntraServer.h>

using namespace LT;

template<TProtocol T>
LTReturnObject LyntraServer::AddListener(const IPEndPoint& _endpoint)
{
	auto listener = std::make_unique<Listener<T>>(_endpoint);
	m_listeners.emplace_back(std::move(listener));
	return LTReturnObject();
}
template<TProtocol T>
LTReturnObject LyntraServer::AddListener(const IPAddress& _address, uint16_t _port)
{
	auto listener = std::make_unique<Listener<T>>(
		IPEndPoint(_address, _port));
	m_listeners.emplace_back(std::move(listener));
	return LTReturnObject();
}
template<TProtocol T>

LTReturnObject LyntraServer::AddListener(uint16_t _port)
{
	auto listener = std::make_unique<Listener<T>>(
		IPEndPoint(
			IPAddress::AnyIPv4, _port));
	m_listeners.emplace_back(std::move(listener));
	return LTReturnObject();
}

LTReturnObject LyntraServer::Start()
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return LTReturnObject();

	return LTReturnObject();
}

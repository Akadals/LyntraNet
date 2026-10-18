#include <LT/Windows/Network/IPEndPoint.h>

namespace LT {
    IPEndPoint::IPEndPoint(const IPAddress& _ip, UINT16 _port) :
        m_ipAddress(_ip),
        m_port(_port),
        m_storage{},
        m_length{} {
        if (_ip.IsIPv4())
        {
            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(_port);
            addr.sin_addr = _ip.GetAddress().ipv4.m_address;

            std::memcpy(&m_storage, &addr, sizeof(addr));
            m_length = sizeof(addr);
        }
        else if (_ip.IsIPv6())
        {
            sockaddr_in6 addr{};
            addr.sin6_family = AF_INET6;
            addr.sin6_port = htons(_port);
            addr.sin6_addr = _ip.GetAddress().ipv6.m_address;

            std::memcpy(&m_storage, &addr, sizeof(addr));
            m_length = sizeof(addr);
        }
    }
    void IPEndPoint::Set(const sockaddr* addr, socklen_t len)
    {
        if (addr == nullptr || len > sizeof(m_storage))
            return;

        if (addr->sa_family == AF_INET
            && len < sizeof(sockaddr_in))
            return;

        if (addr->sa_family == AF_INET6
            && len < sizeof(sockaddr_in6))
            return;

        std::memset(&m_storage, 0, sizeof(m_storage));
        std::memcpy(&m_storage, addr, len);
        m_length = len;

        switch (Family())
        {
        case AF_INET:
        {
            const auto* ipv4 =
                reinterpret_cast<const sockaddr_in*>(&m_storage);

            m_port = ntohs(ipv4->sin_port);
            m_ipAddress = IPAddress(ipv4->sin_addr);
            break;
        }

        case AF_INET6:
        {
            const auto* ipv6 =
                reinterpret_cast<const sockaddr_in6*>(&m_storage);

            m_port = ntohs(ipv6->sin6_port);
            m_ipAddress = IPAddress(ipv6->sin6_addr);
            break;
        }

        default:
            m_port = 0;
            m_ipAddress = IPAddress{};
            break;
        }
    }
}
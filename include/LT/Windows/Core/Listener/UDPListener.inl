namespace LT
{

    LTReturnObject Listener<UDP>::Bind() {

        return {};
    }
    LTReturnObject Listener<UDP>::Bind(HANDLE _comPort) {
        m_state = LISTENER_BINDING;

        if ((m_listenSock.m_sock = WSASocket(
            m_localEndpoint.Family(),
            SOCK_DGRAM,
            IPPROTO_UDP,
            NULL,
            SG_UNCONSTRAINED_GROUP,
            WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
            return {
                RETOBJ_RSN_SOCK,
                RETOBJ_ERR_TYPE_OS,
                RETOBJ_MSG_INVALID };

        if (::bind(
            m_listenSock.m_sock,
            m_localEndpoint.Data(),
            *m_localEndpoint.Length()) == SOCKET_ERROR) {
            if (closesocket(m_listenSock.m_sock) == SOCKET_ERROR)
                return {
                    RETOBJ_RSN_SOCK,
                    RETOBJ_ERR_TYPE_OS,
                    RETOBJ_MSG_INVALID };

            return {
                RETOBJ_RSN_BIND,
                RETOBJ_ERR_TYPE_OS,
                RETOBJ_MSG_INVALID };
        }

        if (CICP(
            reinterpret_cast<HANDLE>(m_listenSock.m_sock),
            _comPort,
            reinterpret_cast<ULONG_PTR>(this),
            0) == INVALID_HANDLE_VALUE)
        {
            closesocket(m_listenSock.m_sock);
            m_listenSock.m_sock = INVALID_SOCKET;

            return {
                RETOBJ_RSN_SOCK,
                RETOBJ_ERR_TYPE_OS,
                RETOBJ_MSG_INVALID
            };
        }

        m_state = LISTENER_BOUND;
        return {};
    }

    LTReturnObject Listener<UDP>::Close() {
        if (m_listenSock.m_sock == INVALID_SOCKET)
            return {};

        m_state = LISTENER_CLOSING;

        if (closesocket(m_listenSock.m_sock) == SOCKET_ERROR)
            return {
                RETOBJ_RSN_SOCK,
                RETOBJ_ERR_TYPE_OS,
                RETOBJ_MSG_INVALID };

        m_listenSock.m_sock = INVALID_SOCKET;
        m_state = LISTENER_CLOSED;

        return {};
    }
}
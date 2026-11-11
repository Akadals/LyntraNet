namespace LT {
    LTReturnObject Listener<ProtocolType::UDP>::Bind() {
        m_state = LSTNR_BINDING;

        if ((m_listenSock.m_sock = WSASocket(
            m_localEndpoint.Family(),
            SOCK_DGRAM,
            IPPROTO_UDP,
            NULL,
            SG_UNCONSTRAINED_GROUP,
            WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
            return {
                RETOBJ_RSN_SOCK,
                RETOBJ_ERR_TYPE_OS };

        if (::bind(
            m_listenSock.m_sock,
            m_localEndpoint.Data(),
            *m_localEndpoint.Length()) == SOCKET_ERROR) {
            if (closesocket(m_listenSock.m_sock) == SOCKET_ERROR)
                return {
                    RETOBJ_RSN_SOCK,
                    RETOBJ_ERR_TYPE_OS };

            return {
                RETOBJ_RSN_BIND,
                RETOBJ_ERR_TYPE_OS };
        }

        if (CICP(
            reinterpret_cast<HANDLE>(m_listenSock.m_sock),
            m_backend->Handle(),
            reinterpret_cast<ULONG_PTR>(this),
            0) == INVALID_HANDLE_VALUE)
        {
            closesocket(m_listenSock.m_sock);
            m_listenSock.m_sock = INVALID_SOCKET;

            return {
                RETOBJ_RSN_SOCK,
                RETOBJ_ERR_TYPE_OS };
        }

        m_state = LSTNR_BOUND;
        return {};
    }

    LTReturnObject Listener<ProtocolType::UDP>::Close() {
        if (m_listenSock.m_sock == INVALID_SOCKET)
            return {};

        m_state = LSTNR_CLOSING;

        if (closesocket(m_listenSock.m_sock) == SOCKET_ERROR)
            return {
                RETOBJ_RSN_SOCK,
                RETOBJ_ERR_TYPE_OS };

        m_listenSock.m_sock = INVALID_SOCKET;
        m_state = LSTNR_CLOSED;

        return {};
    }
}
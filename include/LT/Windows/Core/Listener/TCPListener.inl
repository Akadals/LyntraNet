namespace LT
{
    LTReturnObject Listener<TCP>::Bind() {
        return {};
    }
    LTReturnObject Listener<TCP>::Bind(HANDLE _comPort) {
        m_state = LISTENER_BINDING;

        if ((m_listenSock.m_sock = WSASocket(
            m_localEndpoint.Family(),
            SOCK_STREAM,
            IPPROTO_TCP,
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

    LTReturnObject Listener<TCP>::Listen() {
        if (listen(
            m_listenSock.m_sock,
            m_backlog) == SOCKET_ERROR) {
            if (m_listenSock.m_sock != INVALID_SOCKET)
                closesocket(m_listenSock.m_sock);
            return {
                RETOBJ_RSN_LISTEN,
                RETOBJ_ERR_TYPE_OS,
                RETOBJ_MSG_INVALID };
        }

        auto res = load_accept_extensions();

        if (!res.Value())
            return { res };

        m_state = LISTENER_LISTENING;
        return {};
    }

    LTReturnObject Listener<TCP>::Close() {
        if (m_listenSock.m_sock == INVALID_SOCKET)
            return {};

        m_state = LISTENER_CLOSING;

        for (auto& ctx : m_acceptContexts)
            CancelIoEx(
                reinterpret_cast<HANDLE>(m_listenSock.m_sock),
                &ctx->m_overlapped);

        if (closesocket(m_listenSock.m_sock) == SOCKET_ERROR)
            return {
                RETOBJ_RSN_SOCK,
                RETOBJ_ERR_TYPE_OS,
                RETOBJ_MSG_INVALID };

        m_listenSock.m_sock = INVALID_SOCKET;
        m_state = LISTENER_CLOSED;

        return {};
    }

    LTReturnValue<BOOL> Listener<TCP>::PostAccept() {
        if (m_state != LISTENER_LISTENING)
            return {
                RETOBJ_RSN_LISTEN,
                RETOBJ_ERR_TYPE_OS,
                RETOBJ_MSG_INVALID };

        auto remoteCtx = m_acceptContextPool.Acquire();
        remoteCtx->m_contextState.store(CONTEXT_AVAILABLE);

        if ((remoteCtx->m_acceptSock.m_sock = WSASocket(
            m_localEndpoint.Family(),
            SOCK_STREAM,
            IPPROTO_TCP,
            NULL,
            SG_UNCONSTRAINED_GROUP,
            WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
            if (remoteCtx->m_acceptSock.m_sock != INVALID_SOCKET)
                closesocket(remoteCtx->m_acceptSock.m_sock);

            m_acceptContextPool.Release(std::move(remoteCtx));
            remoteCtx = nullptr;

            return {
                RETOBJ_RSN_SOCK,
                RETOBJ_ERR_TYPE_OS,
                RETOBJ_MSG_INVALID };
        }

        DWORD bytes = 0;
        DWORD dwLen = sizeof(SOCKADDR_STORAGE) + 16;

        if (!m_lpAcceptEx(
            m_listenSock.m_sock,
            remoteCtx->m_acceptSock.m_sock,
            remoteCtx->m_buffer,
            0,
            dwLen,
            dwLen,
            &bytes,
            &remoteCtx->m_overlapped) &&
            WSAGetLastError() != WSA_IO_PENDING) {
            if (remoteCtx->m_acceptSock.m_sock != INVALID_SOCKET)
                closesocket(remoteCtx->m_acceptSock.m_sock);

            m_acceptContextPool.Release(std::move(remoteCtx));
            remoteCtx = nullptr;

            return {
                RETOBJ_RSN_ACPT,
                RETOBJ_ERR_TYPE_OS,
                RETOBJ_MSG_INVALID };
        }

        remoteCtx->m_acceptMode = ACCEPT_EX;
        remoteCtx->m_contextState.store(CONTEXT_PENDING);
        remoteCtx->m_ownerListener = this;
        m_acceptContexts.emplace_back(std::move(remoteCtx));
        remoteCtx = nullptr;

        return { TRUE };
    }

    LTReturnValue<LTSOCKET> Listener<TCP>::Accept(HANDLE _comPort) {
        if (m_state != LISTENER_LISTENING)
            return {
                RETOBJ_RSN_LISTEN,
                RETOBJ_ERR_TYPE_OS,
                RETOBJ_MSG_INVALID };

        auto remoteCtx = m_acceptContextPool.Acquire();
        remoteCtx->m_contextState.store(CONTEXT_AVAILABLE);

        remoteCtx->m_localAddress.SetAddress(m_localEndpoint);

        if ((remoteCtx->m_acceptSock.m_sock = WSAAccept(
            m_listenSock.m_sock,
            remoteCtx->m_remoteAddress.Data(),
            remoteCtx->m_remoteAddress.Length(),
            NULL,
            NULL)) == INVALID_SOCKET) {
            m_acceptContextPool.Release(std::move(remoteCtx));
            remoteCtx = nullptr;

            return {
                RETOBJ_RSN_ACPT,
                RETOBJ_ERR_TYPE_OS,
                RETOBJ_MSG_INVALID };
        }

        remoteCtx->m_acceptMode = WSA_ACCEPT;
        remoteCtx->m_contextState.store(CONTEXT_PENDING);
        remoteCtx->m_ownerListener = this;

        if (CICP(
            (HANDLE)remoteCtx->m_acceptSock.m_sock,
            _comPort,
            (ULONG_PTR)remoteCtx.get(),
            0) == INVALID_HANDLE_VALUE) {
            m_acceptContextPool.Release(std::move(remoteCtx));
            remoteCtx = nullptr;

            return {
                RETOBJ_RSN_ACPT,
                RETOBJ_ERR_TYPE_OS,
                RETOBJ_MSG_INVALID };
        }

        return { remoteCtx->m_acceptSock };
    }

    VOID Listener<TCP>::GetAcceptExSockAddrs(
        PACPTCTX _acptCtx,
        IPAddress * _localAdrPtr,
        IPAddress * _remoteAdrPtr) {
        PSOCKADDR localAdr = nullptr;
        PSOCKADDR remoteAdr = nullptr;
        int localAdrLen = 0;
        int remoteAdrLen = 0;

        DWORD dwLen = sizeof(SOCKADDR_STORAGE) + 16;

        m_lpGetAcceptExSockaddrs(
            _acptCtx->m_buffer,
            0,
            dwLen,
            dwLen,
            &localAdr,
            &localAdrLen,
            &remoteAdr,
            &remoteAdrLen);

        _localAdrPtr->SetAddress(localAdr, localAdrLen);
        _remoteAdrPtr->SetAddress(remoteAdr, remoteAdrLen);
    }

    LTReturnValue<BOOL> Listener<TCP>::load_accept_extensions() {
        if (m_lpAcceptEx && m_lpGetAcceptExSockaddrs) [[unlikely]]
            return { TRUE };

        DWORD bytes = 0;

        auto load = [&](GUID guid, auto& function) -> bool {
            return WSAIoctl(
                m_listenSock.m_sock,
                SIO_GET_EXTENSION_FUNCTION_POINTER,
                &guid,
                sizeof(guid),
                reinterpret_cast<PVOID*>(&function),
                sizeof(function),
                &bytes,
                nullptr,
                nullptr
            ) != SOCKET_ERROR;
            };

        if (!m_lpAcceptEx &&
            !load(
                WSAID_ACCEPTEX,
                m_lpAcceptEx))
            return {
                RETOBJ_RSN_ACPTEX,
                RETOBJ_ERR_TYPE_OS,
                RETOBJ_MSG_INVALID,
                FALSE };

        if (!m_lpGetAcceptExSockaddrs &&
            !load(
                WSAID_GETACCEPTEXSOCKADDRS,
                m_lpGetAcceptExSockaddrs))
            return {
                RETOBJ_RSN_ACPTEX,
                RETOBJ_ERR_TYPE_OS,
                RETOBJ_MSG_INVALID,
                FALSE };

        return { TRUE };
    }
}

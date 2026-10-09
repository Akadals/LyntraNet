using namespace LT;


LTReturnObject Listener<TCP>::Bind()
{
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
        m_localEndpoint.Length()) == SOCKET_ERROR)
        return {
            RETOBJ_RSN_BIND,
            RETOBJ_ERR_TYPE_OS,
            RETOBJ_MSG_INVALID }; 

    m_state = LISTENER_BOUND;
    return {};
}

LTReturnObject Listener<TCP>::Listen()
{
    if (listen(
        m_listenSock.m_sock,
        m_backlog) == SOCKET_ERROR)
    {
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

LTReturnObject Listener<TCP>::Close()
{
    if (m_listenSock.m_sock == INVALID_SOCKET)
        return {};

    m_state = LISTENER_CLOSING;

    for (auto* ctx : m_acceptContexts)
        CancelIoEx(
            reinterpret_cast<HANDLE>(m_listenSock.m_sock),
            &ctx->m_overlapped);

    //shutdown 추가 고려

    if (closesocket(m_listenSock.m_sock) == SOCKET_ERROR)
        return {
            RETOBJ_RSN_SOCK,
            RETOBJ_ERR_TYPE_OS,
            RETOBJ_MSG_INVALID };

    m_listenSock.m_sock = INVALID_SOCKET;
    m_state = LISTENER_CLOSED;

    return {};
}

LTReturnValue<BOOL> Listener<TCP>::PostAccept()
{
    if (m_state != LISTENER_LISTENING)
        return {
            RETOBJ_RSN_LISTEN,
            RETOBJ_ERR_TYPE_OS,
            RETOBJ_MSG_INVALID };

    ACPTCTX remoteCtx = std::move(m_acceptContextPool.Acquire());
    remoteCtx.m_contextState = CONTEXT_AVAILABLE;

    if ((remoteCtx.m_acceptSock.m_sock = WSASocket(
        m_localEndpoint.Family(),
        SOCK_STREAM,
        IPPROTO_TCP,
        NULL,
        SG_UNCONSTRAINED_GROUP,
        WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        if (remoteCtx.m_acceptSock.m_sock != INVALID_SOCKET)
            closesocket(remoteCtx.m_acceptSock.m_sock);

        return {
            RETOBJ_RSN_SOCK,
            RETOBJ_ERR_TYPE_OS,
            RETOBJ_MSG_INVALID };
    }

	DWORD bytes = 0;
    DWORD dwLen = sizeof(SOCKADDR_STORAGE) + 16;

    if (!m_lpAcceptEx(
        m_listenSock.m_sock,
        remoteCtx.m_acceptSock.m_sock,
        remoteCtx.m_buffer,
        0,
        dwLen,
        dwLen,
        &bytes,
        &remoteCtx.m_overlapped) &&
        WSAGetLastError() != WSA_IO_PENDING)
    {
        if (remoteCtx.m_acceptSock.m_sock != INVALID_SOCKET)
            closesocket(remoteCtx.m_acceptSock.m_sock);

        m_acceptContextPool.Release(std::move(remoteCtx));

        return {
            RETOBJ_RSN_SOCK,
            RETOBJ_ERR_TYPE_OS,
            RETOBJ_MSG_INVALID };
    }

    remoteCtx.m_contextState = CONTEXT_PENDING;
    m_acceptContexts.emplace_back(remoteCtx);

    return { TRUE };

    // Accept 완료 후 IOCP에서 GetAcceptSockAddrs를 통해 
    // local과 remote 주소 기록 필요
}

LTReturnValue<LTSOCKET> Listener<TCP>::Accept()
{
    if (m_state != LISTENER_LISTENING)
        return {
            RETOBJ_RSN_LISTEN,
            RETOBJ_ERR_TYPE_OS,
            RETOBJ_MSG_INVALID };

    ACPTCTX remoteCtx = std::move(m_acceptContextPool.Acquire());
    remoteCtx.m_contextState = CONTEXT_AVAILABLE;
    int remoteAddressLength = static_cast<int>(remoteCtx.m_remoteAddress.Length());

    if ((remoteCtx.m_acceptSock.m_sock = WSAAccept(
        m_listenSock.m_sock,
        remoteCtx.m_remoteAddress.Data(),
        &remoteAddressLength,
        NULL,
        NULL)) == INVALID_SOCKET)
        return {
            RETOBJ_RSN_ACPT,
            RETOBJ_ERR_TYPE_OS,
            RETOBJ_MSG_INVALID };

    remoteCtx.m_contextState = CONTEXT_PENDING;

    return { remoteCtx.m_acceptSock };
}

LTReturnValue<BOOL> Listener<TCP>::load_accept_extensions()
{
    if (m_lpAcceptEx && m_lpGetAcceptExSockaddrs) [[unlikely]]
        return { TRUE };

    DWORD bytes = 0;

    auto load = [&](GUID guid, auto& function) -> bool
        {
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

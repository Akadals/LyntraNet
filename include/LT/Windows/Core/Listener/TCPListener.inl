using namespace LT;


LTReturnObject Listener<TCP>::Bind()
{
    if ((m_listenSock.m_sock = WSASocket(
        m_localEndpoint.GetAddress().Family(),
        SOCK_STREAM,
        0,
        NULL,
        0,
        WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
        return {
            RETOBJ_RSN_SOCK,
            RETOBJ_ERR_TYPE_OS,
            RETOBJ_MSG_INVALID };

    auto& listenAdr = m_localEndpoint.GetAddress();

    if (::bind(
        m_listenSock.m_sock,
        listenAdr.Data(),
        listenAdr.Length()) == SOCKET_ERROR)
        return {
            RETOBJ_RSN_BIND,
            RETOBJ_ERR_TYPE_OS,
            RETOBJ_MSG_INVALID };

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

    m_isListening = TRUE;
    return {};
}

LTReturnObject Listener<TCP>::Close()
{
    if (m_listenSock.m_sock == INVALID_SOCKET)
        return {};

    m_isListening = FALSE;

    for (auto* ctx : m_acceptContexts)
        CancelIoEx(
            reinterpret_cast<HANDLE>(m_listenSock.m_sock),
            &ctx->m_overlapped);

    if (closesocket(m_listenSock.m_sock) == SOCKET_ERROR)
        return {
            RETOBJ_RSN_SOCK,
            RETOBJ_ERR_TYPE_OS,
            RETOBJ_MSG_INVALID };

    m_listenSock.m_sock = INVALID_SOCKET;

    return {};
}

LTReturnValue<BOOL> Listener<TCP>::PostAccept()
{
    if(!m_isListening)
        return {
            RETOBJ_RSN_LISTEN,
            RETOBJ_ERR_TYPE_OS,
            RETOBJ_MSG_INVALID };

    ACPTCTX clntCtx = std::move(m_acceptContextPool.Acquire());

    if ((clntCtx.m_AcceptSock.m_sock = WSASocket(
        m_localEndpoint.GetAddress().Family(),
        SOCK_STREAM,
        0,
        NULL,
        0,
        WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        if (clntCtx.m_AcceptSock.m_sock != INVALID_SOCKET)
            closesocket(clntCtx.m_AcceptSock.m_sock);

        return {
            RETOBJ_RSN_SOCK,
            RETOBJ_ERR_TYPE_OS,
            RETOBJ_MSG_INVALID };
    }

	DWORD bytes = 0;
    DWORD dwLen = sizeof(SOCKADDR_STORAGE) + 16;

    if (!m_lpAcceptEx(
        m_listenSock.m_sock,
        clntCtx.m_AcceptSock.m_sock,
        clntCtx.m_wsaBuf.buf,
        0,
        dwLen,
        dwLen,
        &bytes,
        &clntCtx.m_overlapped) &&
        WSAGetLastError() != WSA_IO_PENDING)
    {
        if (clntCtx.m_AcceptSock.m_sock != INVALID_SOCKET)
            closesocket(clntCtx.m_AcceptSock.m_sock);

        m_acceptContextPool.Release(std::move(clntCtx));

        return {
            RETOBJ_RSN_SOCK,
            RETOBJ_ERR_TYPE_OS,
            RETOBJ_MSG_INVALID };
    }

    m_acceptContexts.emplace_back(clntCtx);

    return { TRUE };
}

//이후 m_lpGetAcceptExSockaddrs 등록 필요

LTReturnValue<LTSOCKET> Listener<TCP>::Accept()
{
    if (!m_isListening)
        return {
            RETOBJ_RSN_LISTEN,
            RETOBJ_ERR_TYPE_OS,
            RETOBJ_MSG_INVALID };

    LTSOCKET clntSock; //Pooling
    SOCKADDR_STORAGE clntAdr{};
    int adrLen = sizeof(clntAdr);

    if ((clntSock.m_sock = WSAAccept( //소켓 정책 확인 필요
        m_listenSock.m_sock,
        (PSOCKADDR)&clntAdr,
        &adrLen,
        NULL,
        NULL)) == INVALID_SOCKET)
        return {
            RETOBJ_RSN_ACPT,
            RETOBJ_ERR_TYPE_OS,
            RETOBJ_MSG_INVALID };

    return { clntSock };
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

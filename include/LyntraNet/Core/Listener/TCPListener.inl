using namespace LT;

LTReturnObject Listener<TCP>::Bind()
{
	return LTReturnObject();
}

LTReturnObject Listener<TCP>::Listen()
{
	return LTReturnObject();
}

LTReturnObject Listener<TCP>::Close()
{
	return LTReturnObject();
}

LTSOCKET Listener<TCP>::Accept()
{
	return LTSOCKET(INVALID_SOCKET);
}
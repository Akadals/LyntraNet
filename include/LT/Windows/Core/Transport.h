#ifndef __INCL_LYNTRA_TRANSPORT_H__
#define __INCL_LYNTRA_TRANSPORT_H__

#include <LT/Windows/Network/Socket/LTSocket.h>
#include <LT/Common/Protocol/TProtocol.h>
#include <LT/Windows/Network/IPAddress.h>
#include <LT/Utility.h>

namespace LT
{
	class ITransport
	{
	protected:
		LTSOCKET m_ownerSock;

		RingBuffer<ZeroCopy> m_recvBuffer;
		RingBuffer<ZeroCopy> m_sendBuffer;
	public:
		ITransport(size_t _bufferSize) :
			m_ownerSock(LTSOCKET::INVALID()),
			m_recvBuffer(_bufferSize),
			m_sendBuffer(_bufferSize) {}

		LTSOCKET Get() const { return m_ownerSock; }
		void Set(LTSOCKET _sock) { m_ownerSock = _sock; }

		virtual DWORD StartRecv() = 0;
		virtual DWORD StartSend() = 0;
	};

	template<TProtocol>
	class Transport;

	template<>
	class Transport<TCP> : public ITransport
	{
	private:
		IPAddress m_localAddress;
		IPAddress m_remoteAddress;
	public:
		Transport<TCP>(size_t _bufferSize = 0x2000) : // 8KiB
			ITransport(_bufferSize),
			m_localAddress{},
			m_remoteAddress{} {}

		~Transport<TCP>() {}

		IPAddress* GetLocalAddress() { return &m_localAddress; }
		IPAddress* GetRemoteAddress() { return &m_remoteAddress; }
	};
}
#endif
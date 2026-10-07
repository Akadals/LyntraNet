#ifndef __INCL_LYNTRA_SESSION_H__
#define __INCL_LYNTRA_SESSION_H__

#include "NetworkConnection.h"

namespace LT::Game
{
	typedef class Session
		SESSION, * PSESSION;

	class alignas(64) Session
	{
	public:
		static const size_t MAX_NAME_SIZE = 30;
	private:
		DWORD m_uid = {};
		char m_uuid[37] = {};

		char m_nickname[MAX_NAME_SIZE] = {};
	public:
		Session(SOCKET _socket, SOCKADDR_IN _addr);
		~Session();
		DWORD GetUID() const { return m_uid; }
		const char* GetUUID() const { return m_uuid; }
		const char* GetNickname() const { return m_nickname; }
	};
}
#endif
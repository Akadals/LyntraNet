#include <LT/Common/Utility/LTReturnObject.h>

namespace LT {
	LTReturnObject::LTReturnObject() {
		m_statusCode = RETOBJ_STATE_NONE;
	}
	LTReturnObject::LTReturnObject(
		int _returnObjectReason, 
		int _returnObjectErrorType) {
		m_reason = _returnObjectReason;
		m_statusCode == RETOBJ_STATE_NONE;
		// m_statusCode는 reason에 따라 결정 (뒤끝 sdk 참조)
		switch (_returnObjectErrorType) {
		case RETOBJ_ERR_TYPE_OS: {
			WORD errorCode = WSAGetLastError();

			char* buffer = nullptr;

			DWORD length = FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr,
				errorCode,
				MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
				reinterpret_cast<char*>(&buffer),
				0,
				nullptr
			);

			if (length > 0 && buffer != nullptr)
				m_message.append(buffer, length);
			break;
		}
		case RETOBJ_ERR_TYPE_LT:
			break;
		}
	}
}
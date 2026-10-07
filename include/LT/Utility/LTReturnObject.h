#ifndef __INCL_LYNTRA_ERROR_HANDLER_H__
#define __INCL_LYNTRA_ERROR_HANDLER_H__

#include <string>
#include <optional>

#define RETOBJ_STATE_NONE				0

#define RETOBJ_RSN_SOCK					0x00000001
#define RETOBJ_RSN_ACPT					0x00000002
#define RETOBJ_RSN_ACPTEX				0x00000003
#define RETOBJ_RSN_ACPTEXSOCKADDRS		0x00000004
#define RETOBJ_RSN_LOAD					0x00000005
#define RETOBJ_RSN_BIND					0x00000006
#define RETOBJ_RSN_LISTEN				0x00000006

#define RETOBJ_ERR_TYPE_LT				0
#define RETOBJ_ERR_TYPE_OS				1 //윈도우인경우 message는 WSAGetLastError출력

#define RETOBJ_MSG_NONE					0x00000001
#define RETOBJ_MSG_INVALID				0x00000002

//상수값 수정필요

namespace LT
{
	class ReturnObject
	{
	protected:
		UINT16		m_statusCode;
		UINT16		m_reason;
		std::string m_message;
	public:
		ReturnObject() = default;
		ReturnObject(UINT16 _code) :
			m_statusCode(_code) {}

		const UINT16 StatusCode() const noexcept { return m_statusCode; }
		const std::string_view Message() const { return m_message; }
	};
	class LTReturnObject : public ReturnObject
	{
	public:
		LTReturnObject() :
			ReturnObject(RETOBJ_STATE_NONE) {}
		LTReturnObject(
			int _returnObjectReason,
			int _returnObjectErrorType,
			int _returnObjectMessage = RETOBJ_MSG_NONE) {}
	};

	template<typename T>
	class LTReturnValue : public LTReturnObject
	{
	private:
		T m_value;
	public:
		LTReturnValue() :
			m_value(T{}),
			ReturnObject(RETOBJ_STATE_NONE) {}
		LTReturnValue(T value) :
			m_value(value) {}
		LTReturnValue(
			int _returnObjectReason,
			int _returnObjectErrorType,
			int _returnObjectMessage = RETOBJ_MSG_NONE,
			T _defaultValue = T{}) :
			m_value(_defaultValue) {}
		LTReturnValue(
			const LTReturnObject& _ReturnObject,
			T _defaultValue = T{}) :
			m_value(_defaultValue) {}
		const T& Value() const { return m_value; }
	};
}

#endif
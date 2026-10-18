#ifndef __INCL_LYNTRA_RETURN_OBJECT_H__
#define __INCL_LYNTRA_RETURN_OBJECT_H__

#include <string>
#include <optional>

#define RETOBJ_STATE_NONE				0

#define RETOBJ_RSN_SOCK					0x00000001
#define RETOBJ_RSN_ACPT					0x00000002
#define RETOBJ_RSN_ACPTEX				0x00000003
#define RETOBJ_RSN_ACPTEXSOCKADDRS		0x00000004
#define RETOBJ_RSN_LOAD					0x00000005
#define RETOBJ_RSN_BIND					0x00000006
#define RETOBJ_RSN_LISTEN				0x00000007
#define RETOBJ_RSN_STARTUP				0x00000008

#define RETOBJ_RSN_CICP					0x00000009

#define RETOBJ_ERR_TYPE_LT				0
#define RETOBJ_ERR_TYPE_OS				1 //윈도우인경우 message는 WSAGetLastError출력

//상수값 수정필요

namespace LT
{
	class ReturnObject
	{
	protected:
		int16_t		m_statusCode;
		int16_t		m_reason;
		std::string m_message;
	public:
		ReturnObject() = default;
		const int16_t StatusCode() const noexcept { return m_statusCode; }
		const std::string_view Message() const { return m_message; }
	};

	class LTReturnObject : public ReturnObject
	{
	public:
		LTReturnObject();
		LTReturnObject(
			_In_ int _returnObjectReason,
			_In_ int _returnObjectErrorType);
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
			_In_ int _returnObjectReason,
			_In_ int _returnObjectErrorType,
			_In_ T _defaultValue = T{}) :
			m_value(_defaultValue) {}
		LTReturnValue(
			_In_ const LTReturnObject& _ReturnObject,
			_In_ T _defaultValue = T{}) :
			m_value(_defaultValue) {}
		const T& Value() const { return m_value; }
	};
}

#endif
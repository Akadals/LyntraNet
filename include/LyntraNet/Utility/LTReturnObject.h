#ifndef __INCL_LYNTRA_ERROR_HANDLER_H__
#define __INCL_LYNTRA_ERROR_HANDLER_H__

#ifdef _WIN32
#define TO_WINERR(x) LTReturnObject::ParseToWinError(x)


#elif __linux__
#define TO_LINUXERR(x) LTReturnObject::ParseToLinuxError(x)


#endif

#include <string>

namespace LT
{
	class LTReturnObject
	{
	private:
		uint16_t m_result = 0;
		std::string m_message;
	public:
		LTReturnObject() = default;

		uint16_t StatusCode() const noexcept { return m_result; }
		std::string_view Message() const { return m_message; }
	public:
#ifdef _WIN32
		static int ParseToWinError(const LTReturnObject& _obj);
#elif __linux__
		static int ParseToLinuxError(const LTReturnObject& _obj);
#endif
	};
}

#endif
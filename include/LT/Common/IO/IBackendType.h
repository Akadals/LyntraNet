#ifndef __INCL_LYNTRA_INTERFACE_BACKEND_TYPE_H__
#define __INCL_LYNTRA_INTERFACE_BACKEND_TYPE_H__

#include <LT/Common/Utility/LTReturnObject.h>

namespace LT
{
	class IBackend {
	public:
		IBackend() = default;
		virtual ~IBackend() = default;
		virtual LTReturnObject Startup() = 0;
	};
}

#endif
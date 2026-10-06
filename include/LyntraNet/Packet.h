#ifndef __INCL_LYNTRA_PACKET_H__
#define __INCL_LYNTRA_PACKET_H__

#include "Utility/ConcurrentPolicy.h"
#include <winsock2.h>
#include <atomic>
#include <vector>

#define	LT_PKT_BASE 0x00010000

namespace LT::Packet
{

	typedef struct Header
		HEADER, * PHEADER;
	typedef class Job
		JOB, * PJOB;

	#pragma pack(push, 1)
	struct Header
	{
		uint16_t size = 0;
		uint32_t header = 0;
	};
	#pragma pack(pop)

	class alignas(64) Job
	{
	private:
		std::vector<char> m_data;
		uint32_t m_header;
	};
}
#endif
//Local
#include "IDemuxer.h"

namespace SJCMMLib
{
	class CRawMPEGVideoDemuxer : public IDemuxer
	{
	public:
		//Functions
		void ReadHeader(CInputContext &refCtx);
		EResult ReadPacket(SPacket &refPacket, CInputContext &refCtx);
		void Release();
	};
}
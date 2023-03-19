//Local
#include "IDemuxer.h"

namespace SJCMMLib
{
	class CRawAudioAC3Demuxer : public IDemuxer
	{
	public:
		//Methods
		void ReadHeader(CInputContext &refCtx);
		EResult ReadPacket(SPacket &refPacket, CInputContext &refCtx);
		void Release();
	};
}
//Local
#include "IDemuxer.h"

namespace SJCMMLib
{
	class CMPEGAudioDemuxer : public IDemuxer
	{
	private:
		//Methods
		void ReadID3V1Tags(CInputContext &refCtx);
		void ReadVBRTags(CInputContext &refCtx);
	public:
		//Functions
		void ReadHeader(CInputContext &refCtx);
		EResult ReadPacket(SPacket &refPacket, CInputContext &refCtx);
		void Release();
	};
}
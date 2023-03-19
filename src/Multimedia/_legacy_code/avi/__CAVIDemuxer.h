//Local
#include "AStream.h"
#include "__CIndex.h"
#include "IDemuxer.h"

namespace SJCMMLib
{
	struct SAVIStreamInfo
	{
		uint32 currentFrame;
		uint32 start;
		uint32 sampleSize;
		uint32 remainingSize;
		uint32 packetSize;
		uint32 currentTimeStamp;
	};

	class CAVIDemuxer : public IDemuxer
	{
	private:
		//Variables
		bool isInterleaved;
		bool indexLoaded;
		int32 streamIndex;
		uint32 moviListOffset;
		SJCLib::CMap<uint32, SAVIStreamInfo> streamInfo;
		CIndex index;
		//Functions
		void ReadIndxChunk(SJCLib::ASeekableInputStream *pInput, uint32 frameNumber);
		void GuessInterleave(CInputContext &refCtx);
		bool Sync(CInputContext &refCtx);
	public:
		//Constructor
		CAVIDemuxer();
		//Functions
		void ReadHeader(CInputContext &refCtx);
		EResult ReadPacket(SPacket &refPacket, CInputContext &refCtx);
		void Release();
	};
}
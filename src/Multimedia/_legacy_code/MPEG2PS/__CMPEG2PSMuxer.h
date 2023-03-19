/*//Local
#include "IMuxer.h"

namespace SJCMMLib
{
	struct SStreamInfo
	{
		byte streamId; //mpeg stream id
		SJCLib::CFIFOBuffer buffer;
	};

	class CMPEG2PSMuxer : public IMuxer
	{
	private:
		//Variables
		uint32 packetSize;
		uint16 nVideoStreams;
		SJCLib::CMap<uint32, SStreamInfo> streamInfo;
		//Functions
		bool OutputPacket(COutputContext &refCtx);
		uint16 PutPackHeader(SJCLib::ASeekableOutputStream *pOutput);
		uint16 PutSystemHeader(SJCLib::ASeekableOutputStream *pOutput);
	public:
		//Constructor
		CMPEG2PSMuxer();
		//Functions
		void Finalize(COutputContext &refCtx) const;
		void WriteHeader(COutputContext &refCtx);
		void WritePacket(COutputContext &refCtx, const SPacket *pPacket);
	};
}*/
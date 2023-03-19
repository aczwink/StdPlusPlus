//Local
#include "IDemuxer.h"

/*
This implementation is followed by:
	ISO IEC 13818-1

Some info refering MPEG-1 is taken from:
	http://stnsoft.com/DVD
*/

namespace SJCMMLib
{
	class CMPEG2PSDemuxer : public IDemuxer
	{
		struct SPESHeaderInfo
		{
			uint8 streamId;
			uint16 packetLength;
			bool isMpeg1; //true if an mpeg1 decoder should be used
			uint64 pts;
		};
	private:
		//Functions
		void ReadPESHeader(SJCLib::ASeekableInputStream &refInput, SPESHeaderInfo &refInfo) const;
		void Resync(SJCLib::ASeekableInputStream &refInput) const;
	public:
		//Functions
		EResult ReadPacket(SPacket &refPacket, CInputContext &refCtx);
	};
}
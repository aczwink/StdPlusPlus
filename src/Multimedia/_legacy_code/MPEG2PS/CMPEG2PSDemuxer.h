#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

/*
Specifications:
	for MPEG-1: ISO IEC 11172-1
*/

class CMPEG2PSDemuxer : public ADemuxer
{
private:
	//Members
	CMap<uint8, uint32> streamMap; //mpeg stream id to stream index
	
	//Methods
	AStream *MapDVDPrivateStream1(byte subStreamNumber);
	AStream *MapStream(uint8 streamId);
	void ReadPackHeader();
	uint8 ReadPESExtendedHeader(uint64 &refPts);
	void ReadSystemHeader();
	void Resync(byte &refStartCode);

public:
	//Constructor
	inline CMPEG2PSDemuxer(const IFormat &refFormat, ASeekableInputStream &refInput) : ADemuxer(refFormat, refInput)
	{
	}

	//Methods
	void ReadHeader();
	bool ReadPacket(SPacket &refPacket);
};
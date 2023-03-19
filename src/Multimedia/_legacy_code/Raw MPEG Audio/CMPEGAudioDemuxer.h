#include <ACStdLib.h>
//Local
#include "../../../Codecs/MPEG/MPEGAudio.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

class CMPEGAudioDemuxer : public ADemuxer
{
private:
	//Methods
	void ReadVBRTags(const SMPEGAudioFrameHeader &refFrameHeader);

public:
	//Constructor
	inline CMPEGAudioDemuxer(const IFormat &refFormat, ASeekableInputStream &refInput) : ADemuxer(refFormat, refInput)
	{
	}

	//Methods
	void ReadHeader();
	bool ReadPacket(SPacket &refPacket);
};
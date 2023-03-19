#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

class CDDSDemuxer : public ADemuxer
{
public:
	//Constructor
	inline CDDSDemuxer(const IFormat &refFormat, ASeekableInputStream &refInput) : ADemuxer(refFormat, refInput)
	{
	}

	//Methods
	void ReadHeader();
	bool ReadPacket(SPacket &refPacket);
};
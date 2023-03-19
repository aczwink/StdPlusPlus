#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

class CTGADemuxer : public ADemuxer
{
private:
	//Members
	bool isRunLengthEncoded;
	bool isTopDown;

public:
	//Constructor
	inline CTGADemuxer(const IFormat &refFormat, ASeekableInputStream &refInput) : ADemuxer(refFormat, refInput)
	{
	}

	//Methods
	void ReadHeader();
	bool ReadPacket(SPacket &refPacket);
};
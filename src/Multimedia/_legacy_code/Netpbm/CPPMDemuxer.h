#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

class CPPMDemuxer : public ADemuxer
{
private:
	//Methods
	uint16 ReadUInt();

public:
	//Constructor
	inline CPPMDemuxer(const IFormat &refFormat, ASeekableInputStream &refInput) : ADemuxer(refFormat, refInput)
	{
	}

	//Methods
	void ReadHeader();
	bool ReadPacket(SPacket &refPacket);
};
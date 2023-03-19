#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

class CBMPDemuxer : public ADemuxer
{
private:
	//Members
	bool isBottomUp;

public:
	//Constructor
	inline CBMPDemuxer(const IFormat &refFormat, ASeekableInputStream &refInput) : ADemuxer(refFormat, refInput)
	{
	}
	
	//Methods
	void ReadHeader();
	bool ReadPacket(SPacket &refPacket);
};
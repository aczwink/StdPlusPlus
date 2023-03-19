#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

class CBMPMuxer : public AMuxer
{
private:
	//Members
	uint64 startOffset;
	uint32 imageSize;

public:
	//Constructor
	inline CBMPMuxer(const IFormat &refFormat, ASeekableOutputStream &refOutput) : AMuxer(refFormat, refOutput)
	{
		this->imageSize = 0;
	}

	//Methods
	void Finalize();
	void WriteHeader();
	void WritePacket(const SPacket &refPacket);
};
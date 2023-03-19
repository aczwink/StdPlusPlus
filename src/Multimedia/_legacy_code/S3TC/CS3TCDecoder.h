#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

class CS3TCDecoder : public AVideoDecoder
{
private:
	//Members
	uint8 dxtNum;

public:
	//Constructor
	inline CS3TCDecoder(uint8 dxtNum, AStream &refStream) : AVideoDecoder(refStream)
	{
		this->dxtNum = dxtNum;
	}

	//Methods
	void Decode(const SPacket &refPacket);
	EPixelFormat GetPixelFormat() const;
};
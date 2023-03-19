#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

class CRGB24Decoder : public AVideoDecoder
{
public:
	//Constructor
	inline CRGB24Decoder(AStream &refStream) : AVideoDecoder(refStream)
	{
	}
	
	//Methods
	void Decode(const SPacket &refPacket);
	EPixelFormat GetPixelFormat() const;
};
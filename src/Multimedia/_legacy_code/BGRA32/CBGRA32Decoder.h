#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

class CBGRA32Decoder : public AVideoDecoder
{
public:
	//Constructor
	CBGRA32Decoder(AStream &refStream);

	//Methods
	void Decode(const SPacket &refPacket);
	EPixelFormat GetPixelFormat() const;
};
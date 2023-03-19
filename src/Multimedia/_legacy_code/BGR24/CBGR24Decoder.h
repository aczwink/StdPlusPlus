#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

class CBGR24Decoder : public AVideoDecoder
{
public:
	//Constructor
	CBGR24Decoder(AStream &refStream);

	//Methods
	void Decode(const SPacket &refPacket);
	EPixelFormat GetPixelFormat() const;
};
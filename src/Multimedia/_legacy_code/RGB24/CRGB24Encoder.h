#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

class CRGB24Encoder : public IEncoder
{
public:
	//Methods
	void Encode(const AFrame &refFrame, SPacket &refPacket) const;
};
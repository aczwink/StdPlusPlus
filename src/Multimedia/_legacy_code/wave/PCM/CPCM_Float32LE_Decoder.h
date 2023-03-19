#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

class CPCM_Float32LE_Decoder : public AAudioDecoder
{
public:
	//Constructor
	inline CPCM_Float32LE_Decoder(AStream &refStream) : AAudioDecoder(refStream)
	{
		this->refStream.bitRate = 32 * this->refStream.sampleRate;
	}
	
	//Methods
	void Decode(const SPacket &refPacket);
};
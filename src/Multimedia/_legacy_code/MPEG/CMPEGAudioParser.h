#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

class CMPEGAudioParser : public AParser
{
private:
	//Members
	uint32 state;
	uint8 layer;
	uint32 leftFrameSize;

public:
	//Constructor
	CMPEGAudioParser();

	//Methods
	ECodecId GetCodecId() const;
	void Parse(const SPacket &refPacket);
	void Reset();
};
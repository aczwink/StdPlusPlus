#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

class CMPEGVideoParser : public AParser
{
private:
	//Members
	uint32 state;
	bool foundFrameStart;
	ECodecId codecId;
	bool foundSequenceHeader;

	//Methods
	bool FindFrameEnd(const byte *pBuffer, uint32 bufferSize, int32 &refFrameEndOffset);

public:
	//Constructor
	CMPEGVideoParser();

	//Methods
	ECodecId GetCodecId() const;
	void Parse(const SPacket &refPacket);
	void Reset();
};
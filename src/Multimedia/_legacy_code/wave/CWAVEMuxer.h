#pragma once
#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

class CWAVEMuxer : public AMuxer
{
private:
	//Members
	uint32 riffTagSizeOffset;
	uint32 dataChunkSizeOffset;

	//Methods
	uint16 GetBitsPerSample(ECodecId codecId) const;

public:
	//Constructor
	CWAVEMuxer(const IFormat &refFormat, ASeekableOutputStream &refOutput);
	
	//Methods
	void Finalize();
	void WriteHeader();
	void WritePacket(const SPacket &refPacket);
};
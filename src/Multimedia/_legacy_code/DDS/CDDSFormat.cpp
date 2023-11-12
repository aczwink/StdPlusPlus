//Class Header
#include "CDDSFormat.h"
//Local
#include "../../FourCC.h"
#include "CDDSDemuxer.h"

//Public methods
ADemuxer *CDDSFormat::CreateDemuxer(ASeekableInputStream &refInput) const
{
	return new CDDSDemuxer(*this, refInput);
}

void CDDSFormat::GetFormatInfo(SFormatInfo &refFormatInfo) const
{
	//no seeking at all possible
	refFormatInfo.supportsByteSeeking = false;
}

float32 CDDSFormat::Matches(CBufferInputStream &refBuffer) const
{
	uint32 tag;
	
	if(refBuffer.HitEnd())
		return FORMAT_MATCH_BUFFER_TOO_SMALL;
	
	tag = refBuffer.ReadUInt32LE();
	if(tag == FOURCC("DDS "))
		return 1;
	
	return 0;
}
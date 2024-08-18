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
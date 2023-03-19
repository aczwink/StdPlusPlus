/*//Class Header
#include "__CMPEG2PSFormat.h"
//Local
#include "__CMPEG2PSDemuxer.h"
#include "__CMPEG2PSMuxer.h"
#include "__MPEG.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;

//Public Functions
IDemuxer *CMPEG2PSFormat::GetDemuxer() const
{
	return new CMPEG2PSDemuxer;
}

void CMPEG2PSFormat::GetFormatInfo(SFormatInfo *pFormatInfo) const
{
	MemZero(pFormatInfo, sizeof(*pFormatInfo));

	pFormatInfo->hasHeader = false;

	//DO THIS CORRECTY
	ASSERT(0);
	//pFormatInfo->audioStreamCountDescriptor = STREAMCOUNTDESCRIPTOR_EXACT;
	//pFormatInfo->audioStreams = -1;
	//pFormatInfo->videoStreamCountDescriptor = STREAMCOUNTDESCRIPTOR_EXACT;
	//pFormatInfo->videoStreams = -1;
}

EContainerFormatId CMPEG2PSFormat::GetId() const
{
	return CONTAINERFORMAT_ID_MPEG2PS;
}

IMuxer *CMPEG2PSFormat::GetMuxer() const
{
	return new CMPEG2PSMuxer;
}

EFormatMatch CMPEG2PSFormat::MatchesFormat(byte *pBuffer, uint32 bufferSize) const
{
	byte startCode[4];
	
	if(bufferSize < 17)
		return FORMATMATCH_BUFFERTOOSMALL;
	
	startCode[0] = 0;
	startCode[1] = 0;
	startCode[2] = MPEG2PS_STARTCODE_PREFIX;
	startCode[3] = MPEG2PS_STARTCODE_PACKHEADER;
	
	if(MemCmp(&startCode, pBuffer, 4) == 0) //first must be the pack header
	{
		if(MemCmp(&startCode, pBuffer+14, 3) == 0) //mpeg 2 pack header is 10 bytes wide, next start code prefix must be right after
			return FORMATMATCH_PERFECT;
	}
	return FORMATMATCH_NO;
}*/
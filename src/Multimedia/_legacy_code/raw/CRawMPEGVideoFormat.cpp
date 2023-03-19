//Class Header
#include "__CRawMPEGVideoFormat.h"
//Local
#include "__CRawMPEGVideoDemuxer.h"
#include "__MPEG.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;

//Public Methods
CString CRawMPEGVideoFormat::GetFileExtension() const
{
	return "m1v"; //mpv
}

void CRawMPEGVideoFormat::GetFormatInfo(SFormatInfo &refFormatInfo) const
{
	MemZero(&refFormatInfo, sizeof(refFormatInfo));

	refFormatInfo.videoStreamCountDescriptor = STREAMCOUNTDESCRIPTOR_EXACT;
	refFormatInfo.nVideoStreams = 1;
}

EContainerFormatId CRawMPEGVideoFormat::GetId() const
{
	return CONTAINERFORMAT_ID_RAWMPEGVIDEO;
}

CString CRawMPEGVideoFormat::GetName() const
{
	return "Raw MPEG Video";
}

EFormatMatch CRawMPEGVideoFormat::MatchesFormat(byte *pBuffer, uint32 bufferSize) const
{
	byte startCode[4];
	
	if(bufferSize < 4)
		return FORMATMATCH_BUFFERTOOSMALL;
	
	startCode[0] = 0;
	startCode[1] = 0;
	startCode[2] = MPEG1SYSTEMS_STARTCODEPREFIX;
	startCode[3] = MPEG1VIDEO_STARTCODE_SEQUENCE_HEADER;

	//first header must be a pack header
	if(MemCmp(&startCode, pBuffer, 4) == 0)
		return FORMATMATCH_PERFECT;
	return FORMATMATCH_NO;
}

IDemuxer *CRawMPEGVideoFormat::RequestDemuxerInstance() const
{
	return new CRawMPEGVideoDemuxer;
}

IMuxer *CRawMPEGVideoFormat::RequestMuxerInstance() const
{
	return NULL;
}
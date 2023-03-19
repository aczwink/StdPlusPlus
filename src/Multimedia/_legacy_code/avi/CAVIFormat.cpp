//Class Header
#include "__CAVIFormat.h"
//Local
#include "__CAVIDemuxer.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;
//Definitions
#define RIFFCHUNK_CHUNKID "RIFF"
#define RIFFCHUNK_RIFFTYPE "AVI "

//Public Methods
CString CAVIFormat::GetFileExtension() const
{
	return "avi";
}

void CAVIFormat::GetFormatInfo(SFormatInfo &refFormatInfo) const
{
	MemZero(&refFormatInfo, sizeof(refFormatInfo));
	
	refFormatInfo.hasHeader = true;
	refFormatInfo.audioStreamCountDescriptor = STREAMCOUNTDESCRIPTOR_UNLIMITED;
	refFormatInfo.videoStreamCountDescriptor = STREAMCOUNTDESCRIPTOR_UNLIMITED;
}

EContainerFormatId CAVIFormat::GetId() const
{
	return CONTAINERFORMAT_ID_AVI;
}

CString CAVIFormat::GetName() const
{
	return "Audio Video Interleave (AVI)";
}

EFormatMatch CAVIFormat::MatchesFormat(byte *pBuffer, uint32 bufferSize) const
{
	if(bufferSize < 12)
		return FORMATMATCH_BUFFERTOOSMALL;

	if(!MemCmp(pBuffer, RIFFCHUNK_CHUNKID, 4))
	{
		if(!MemCmp(pBuffer+8, RIFFCHUNK_RIFFTYPE, 4))
			return FORMATMATCH_PERFECT;
	}

	return FORMATMATCH_NO;
}

IDemuxer *CAVIFormat::RequestDemuxerInstance() const
{
	return new CAVIDemuxer;
}

IMuxer *CAVIFormat::RequestMuxerInstance() const
{
	return NULL;
}
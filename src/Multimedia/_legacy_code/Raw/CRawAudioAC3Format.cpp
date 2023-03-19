//Class Header
#include "__CRawAudioAC3Format.h"
//Local
#include "__CRawAudioAC3Demuxer.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;

//Public methods
CString CRawAudioAC3Format::GetFileExtension() const
{
	return "ac3";
}

void CRawAudioAC3Format::GetFormatInfo(SFormatInfo &refFormatInfo) const
{
	MemZero(&refFormatInfo, sizeof(refFormatInfo));

	refFormatInfo.maxProbeSize = UINT32_MAX;

	refFormatInfo.audioStreamCountDescriptor = STREAMCOUNTDESCRIPTOR_EXACT;
	refFormatInfo.nAudioStreams = 1;
}

EContainerFormatId CRawAudioAC3Format::GetId() const
{
	return CONTAINERFORMAT_ID_RAWAC3;
}

CString CRawAudioAC3Format::GetName() const
{
	return "Raw AC-3 Audio";
}

EFormatMatch CRawAudioAC3Format::MatchesFormat(byte *pBuffer, uint32 bufferSize) const
{
	if(bufferSize < 2)
		return FORMATMATCH_BUFFERTOOSMALL;

	if(*pBuffer == 0xB && *(pBuffer+1) == 0x77)
		return FORMATMATCH_PERFECT; //check for single syncword
	return FORMATMATCH_NO;
}

IDemuxer *CRawAudioAC3Format::RequestDemuxerInstance() const
{
	return new CRawAudioAC3Demuxer;
}

IMuxer *CRawAudioAC3Format::RequestMuxerInstance() const
{
	return NULL;
}
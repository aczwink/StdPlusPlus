//Class Header
#include "__CBMPFormat.h"
//Local
#include "__Bmp.h"
#include "__CBMPDemuxer.h"
#include "__CBMPMuxer.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;

//Public Methods
void CBMPFormat::GetFormatInfo(SFormatInfo &refFormatInfo) const
{
	refFormatInfo.hasHeader = true;
	
	refFormatInfo.videoStreamCountDescriptor = STREAMCOUNTDESCRIPTOR_EXACT;
	refFormatInfo.nVideoStreams = 1;
	refFormatInfo.audioStreamCountDescriptor = STREAMCOUNTDESCRIPTOR_EXACT;
	refFormatInfo.nAudioStreams = 0;
}

EContainerFormatId CBMPFormat::GetId() const
{
	return CONTAINERFORMAT_ID_BMP;
}


IMuxer *CBMPFormat::RequestMuxerInstance() const
{
	return new CBMPMuxer;
}
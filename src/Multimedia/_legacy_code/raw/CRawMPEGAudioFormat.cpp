//Class Header
#include "__CRawMPEGAudioFormat.h"
//Local
#include "__CMPEGAudioDemuxer.h"
#include "__MPEGAudio.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;

//Public Methods
void CRawMPEGAudioFormat::GetFormatInfo(SFormatInfo &refFormatInfo) const
{
	MemZero(&refFormatInfo, sizeof(refFormatInfo));
	
	refFormatInfo.hasHeader = false;
	refFormatInfo.maxProbeSize = 1; //read one packet/or complete frame should be enough
	//exactly one audio stream
	refFormatInfo.audioStreamCountDescriptor = STREAMCOUNTDESCRIPTOR_EXACT;
	refFormatInfo.nAudioStreams = 1;
	//no video stream
	refFormatInfo.videoStreamCountDescriptor = STREAMCOUNTDESCRIPTOR_EXACT;
	refFormatInfo.nVideoStreams = 0;
}

EContainerFormatId CRawMPEGAudioFormat::GetId() const
{
	return CONTAINERFORMAT_ID_RAWMPEGAUDIO;
}

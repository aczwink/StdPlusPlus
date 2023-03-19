//Class Header
#include "__CMPEG1SystemsFormat.h"
//Local
#include "__MPEG.h"
#include "__CMPEG2PSDemuxer.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;

//Public Methods
void CMPEG1SystemsFormat::GetFormatInfo(SFormatInfo &refFormatInfo) const
{
	MemZero(&refFormatInfo, sizeof(refFormatInfo));
	
	refFormatInfo.hasHeader = false;
	refFormatInfo.maxProbeSize = 50000;
	refFormatInfo.estimateDurationByReadingLastPacket = true;
	
	//HOW TO ACCOUNT PRIVATE STREAMS?????
	refFormatInfo.audioStreamCountDescriptor = STREAMCOUNTDESCRIPTOR_LESSOREQUAL;
	refFormatInfo.nAudioStreams = 32;
	refFormatInfo.videoStreamCountDescriptor = STREAMCOUNTDESCRIPTOR_LESSOREQUAL;
	refFormatInfo.nVideoStreams = 16;
}

EContainerFormatId CMPEG1SystemsFormat::GetId() const
{
	return CONTAINERFORMAT_ID_MPEG1SYSTEMS;
}

IDemuxer *CMPEG1SystemsFormat::RequestDemuxerInstance() const
{
	//The program stream demuxer also works for mpeg1
	//MatchesFormat() makes sure that this is an mpeg1 file
	return new CMPEG2PSDemuxer;
}
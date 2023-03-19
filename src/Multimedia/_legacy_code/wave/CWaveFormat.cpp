//Class Header
#include "__CWaveFormat.h"
//Local
#include "__CPCM_S16LE_Encoder.h"
#include "__CWaveDemuxer.h"
#include "__CWaveMuxer.h"
#include "__WAVE.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;
//Definitions

//Public Methods
void CWaveFormat::GetFormatInfo(SFormatInfo &refFormatInfo) const
{
	MemZero(&refFormatInfo, sizeof(refFormatInfo));
	
	refFormatInfo.hasHeader = true;
	//exactly one audio stream
	refFormatInfo.audioStreamCountDescriptor = STREAMCOUNTDESCRIPTOR_EXACT;
	refFormatInfo.nAudioStreams = 1;
	//no video stream
	refFormatInfo.videoStreamCountDescriptor = STREAMCOUNTDESCRIPTOR_EXACT;
	refFormatInfo.nVideoStreams = 0;
}

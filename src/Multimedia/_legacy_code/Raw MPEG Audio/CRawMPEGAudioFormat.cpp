//Class header
#include "CRawMPEGAudioFormat.h"
//Local
#include "../../../Codecs/MPEG/MPEGAudio.h"
#include "CMPEGAudioDemuxer.h"

//Public methods
ADemuxer *CRawMPEGAudioFormat::CreateDemuxer(ASeekableInputStream &refInput) const
{
	return new CMPEGAudioDemuxer(*this, refInput);
}

AMuxer *CRawMPEGAudioFormat::CreateMuxer(ASeekableOutputStream &refOutput) const
{
	return nullptr;
}

ECodecId CRawMPEGAudioFormat::GetDefaultCodec(EDataType dataType) const
{
	ASSERT(0);
	
	return ECodecId::Unknown;
}

CString CRawMPEGAudioFormat::GetExtension() const
{
	return "mpa";
}

void CRawMPEGAudioFormat::GetFormatInfo(SFormatInfo &refFormatInfo) const
{
	refFormatInfo.supportsByteSeeking = true;
}

CString CRawMPEGAudioFormat::GetName() const
{
	return "Raw MPEG-1/2/2.5 Audio stream";
}

CFiniteSet<ECodecId> CRawMPEGAudioFormat::GetSupportedCodecs(EDataType dataType) const
{
	CFiniteSet<ECodecId> result;

	NOT_IMPLEMENTED_ERROR;

	return result;
}

float32 CRawMPEGAudioFormat::Matches(CBufferInputStream &refBuffer) const
{
	uint32 state;

	state = 0;

	if(refBuffer.GetSize() < 2)
		return FORMAT_MATCH_BUFFER_TOO_SMALL;

	if(FindNextFrameHeader(state, refBuffer))
		return 1;

	return 0;
}
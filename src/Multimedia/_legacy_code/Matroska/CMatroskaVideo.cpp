//Class Header
#include "CMatroskaVideo.h"
//Local
#include "../BMP/BMP.h"
#include "../WAVE/WAVE.h"
#include "CMatroskaDemuxer.h"
#include "CMatroskaMuxer.h"

//Public methods
ADemuxer *CMatroskaVideo::CreateDemuxer(ASeekableInputStream &refInput) const
{
	return new CMatroskaDemuxer(*this, refInput);
}

AMuxer *CMatroskaVideo::CreateMuxer(ASeekableOutputStream &refOutput) const
{
	return new CMatroskaMuxer(*this, refOutput);
}

ECodecId CMatroskaVideo::GetDefaultCodec(EDataType dataType) const
{
	NOT_IMPLEMENTED_ERROR;
	
	return ECodecId::Unknown;
}

CString CMatroskaVideo::GetExtension() const
{
	return "mkv";
}

void CMatroskaVideo::GetFormatInfo(SFormatInfo &refFormatInfo) const
{
	refFormatInfo.supportsByteSeeking = false;
}

CString CMatroskaVideo::GetName() const
{
	return "Matroska Video";
}

CFiniteSet<ECodecId> CMatroskaVideo::GetSupportedCodecs(EDataType dataType) const
{
	CFiniteSet<ECodecId> result;

	switch(dataType)
	{
	case EDataType::Audio:
		{
			AddMS_TwoCC_AudioCodecs(result);
		}
		break;
	case EDataType::Subtitle:
		{
			NOT_IMPLEMENTED_ERROR;
		}
		break;
	case EDataType::Video:
		{
			AddMS_FourCC_VideoCodecs(result);

			result.Insert(ECodecId::MPEG2Video);
		}
		break;
	}
	
	return result;
}

float32 CMatroskaVideo::Matches(CBufferInputStream &refBuffer) const
{
	if(refBuffer.ReadUInt32BE() == 0x1A45DFA3)
	{
		if(refBuffer.HitEnd())
			return FORMAT_MATCH_BUFFER_TOO_SMALL;
		
		return 1;
	}
	
	return 0;
}
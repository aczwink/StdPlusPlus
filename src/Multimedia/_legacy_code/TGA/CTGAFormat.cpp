//Class header
#include "CTGAFormat.h"
//Local
#include "CTGADemuxer.h"

//Public methods
ADemuxer *CTGAFormat::CreateDemuxer(ASeekableInputStream &refInput) const
{
	return new CTGADemuxer(*this, refInput);
}

AMuxer *CTGAFormat::CreateMuxer(ASeekableOutputStream &refOutput) const
{
	return nullptr;
}

ECodecId CTGAFormat::GetDefaultCodec(EDataType dataType) const
{
	if(dataType == EDataType::Video)
		return ECodecId::BGR24;
	
	return ECodecId::Unknown;
}

CString CTGAFormat::GetExtension() const
{
	return "tga";
}

void CTGAFormat::GetFormatInfo(SFormatInfo &refFormatInfo) const
{
	refFormatInfo.supportsByteSeeking = false;
}

CString CTGAFormat::GetName() const
{
	return "Truevision TGA";
}

CFiniteSet<ECodecId> CTGAFormat::GetSupportedCodecs(EDataType dataType) const
{
	CFiniteSet<ECodecId> result;

	NOT_IMPLEMENTED_ERROR;

	return result;
}

float32 CTGAFormat::Matches(CBufferInputStream &refBuffer) const
{
	return 0; //we never know this :S
}
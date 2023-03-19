//Class Header
#include "CDDSFormat.h"
//Local
#include "../../FourCC.h"
#include "CDDSDemuxer.h"

//Public methods
ADemuxer *CDDSFormat::CreateDemuxer(ASeekableInputStream &refInput) const
{
	return new CDDSDemuxer(*this, refInput);
}

AMuxer *CDDSFormat::CreateMuxer(ASeekableOutputStream &refOutput) const
{
	return nullptr;
}

ECodecId CDDSFormat::GetDefaultCodec(EDataType dataType) const
{
	if(dataType == EDataType::Video)
		NOT_IMPLEMENTED_ERROR;
	
	return ECodecId::Unknown;
}

CString CDDSFormat::GetExtension() const
{
	return "dds";
}

void CDDSFormat::GetFormatInfo(SFormatInfo &refFormatInfo) const
{
	//no seeking at all possible
	refFormatInfo.supportsByteSeeking = false;
}

CString CDDSFormat::GetName() const
{
	return "DirectDraw Surface";
}

CFiniteSet<ECodecId> CDDSFormat::GetSupportedCodecs(EDataType dataType) const
{
	CFiniteSet<ECodecId> result;

	NOT_IMPLEMENTED_ERROR;

	return result;
}

float32 CDDSFormat::Matches(CBufferInputStream &refBuffer) const
{
	uint32 tag;
	
	if(refBuffer.HitEnd())
		return FORMAT_MATCH_BUFFER_TOO_SMALL;
	
	tag = refBuffer.ReadUInt32LE();
	if(tag == FOURCC("DDS "))
		return 1;
	
	return 0;
}
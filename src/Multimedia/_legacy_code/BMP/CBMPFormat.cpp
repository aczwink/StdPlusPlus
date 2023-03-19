//Class Header
#include "CBMPFormat.h"
//Local
#include "BMP.h"
#include "CBMPDemuxer.h"
#include "CBMPMuxer.h"

//Public methods
ADemuxer *CBMPFormat::CreateDemuxer(ASeekableInputStream &refInput) const
{
	return new CBMPDemuxer(*this, refInput);
}

AMuxer *CBMPFormat::CreateMuxer(ASeekableOutputStream &refOutput) const
{
	return new CBMPMuxer(*this, refOutput);
}

ECodecId CBMPFormat::GetDefaultCodec(EDataType dataType) const
{
	if(dataType == EDataType::Video)
		return ECodecId::BGR24;
	
	return ECodecId::Unknown;
}

CString CBMPFormat::GetExtension() const
{
	return "bmp";
}

void CBMPFormat::GetFormatInfo(SFormatInfo &refFormatInfo) const
{
	ASSERT(0);
}

CString CBMPFormat::GetName() const
{
	return "BMP Bitmap image";
}

CFiniteSet<ECodecId> CBMPFormat::GetSupportedCodecs(EDataType dataType) const
{
	CFiniteSet<ECodecId> result;
	
	NOT_IMPLEMENTED_ERROR;
	
	return result;
}

float32 CBMPFormat::Matches(CBufferInputStream &refBuffer) const
{
	byte readHeader[BMP_HEADER_TYPE_SIZE];
	
	refBuffer.ReadBytes(readHeader, sizeof(readHeader));
	if(MemCmp(BMP_HEADER_TYPE, readHeader, sizeof(readHeader)) == 0)
		return 1;

	if(refBuffer.HitEnd())
		return FORMAT_MATCH_BUFFER_TOO_SMALL;

	return 0;
}
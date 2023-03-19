//Class header
#include "CPPMFormat.h"
//Local
#include "CPPMDemuxer.h"
//Definitions
#define PPM_HEADER_TYPE "P6"
#define PPM_HEADER_TYPE_SIZE 2

//Public methods
ADemuxer *CPPMFormat::CreateDemuxer(ASeekableInputStream &refInput) const
{
	return new CPPMDemuxer(*this, refInput);
}

AMuxer *CPPMFormat::CreateMuxer(ASeekableOutputStream &refOutput) const
{
	return nullptr;
}

ECodecId CPPMFormat::GetDefaultCodec(EDataType dataType) const
{
	if(dataType == EDataType::Video)
		return ECodecId::RGB24;
	
	return ECodecId::Unknown;
}

CString CPPMFormat::GetExtension() const
{
	return "ppm";
}

void CPPMFormat::GetFormatInfo(SFormatInfo &refFormatInfo) const
{
	ASSERT(0);
}

CString CPPMFormat::GetName() const
{
	return "Netpbm Portable Pixmap (PPM)";
}

CFiniteSet<ECodecId> CPPMFormat::GetSupportedCodecs(EDataType dataType) const
{
	CFiniteSet<ECodecId> result;

	NOT_IMPLEMENTED_ERROR;

	return result;
}

float32 CPPMFormat::Matches(CBufferInputStream &refBuffer) const
{
	byte readHeader[PPM_HEADER_TYPE_SIZE];

	refBuffer.ReadBytes(readHeader, sizeof(readHeader));
	if(MemCmp(PPM_HEADER_TYPE, readHeader, sizeof(readHeader)) == 0)
		return 1;

	if(refBuffer.HitEnd())
		return FORMAT_MATCH_BUFFER_TOO_SMALL;

	return 0;
}
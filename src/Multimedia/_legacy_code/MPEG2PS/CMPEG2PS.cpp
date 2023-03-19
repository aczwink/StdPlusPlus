//Class header
#include "CMPEG2PS.h"
//Local
#include "../../../Codecs/MPEG/MPEG1.h"
#include "CMPEG2PSDemuxer.h"

//Public methods
ADemuxer *CMPEG2PS::CreateDemuxer(ASeekableInputStream &refInput) const
{
	return new CMPEG2PSDemuxer(*this, refInput);
}

AMuxer *CMPEG2PS::CreateMuxer(ASeekableOutputStream &refOutput) const
{
	return nullptr;
}

ECodecId CMPEG2PS::GetDefaultCodec(EDataType dataType) const
{
	//IMPLEMENT ME
	ASSERT(0);
	return ECodecId::Unknown;
}

CString CMPEG2PS::GetExtension() const
{
	return "mpg";
}

void CMPEG2PS::GetFormatInfo(SFormatInfo &refFormatInfo) const
{
	refFormatInfo.supportsByteSeeking = true;
}

CString CMPEG2PS::GetName() const
{
	return "MPEG-2 Program Stream";
}

CFiniteSet<ECodecId> CMPEG2PS::GetSupportedCodecs(EDataType dataType) const
{
	CFiniteSet<ECodecId> result;

	NOT_IMPLEMENTED_ERROR;

	return result;
}

float32 CMPEG2PS::Matches(CBufferInputStream &refBuffer) const
{
	//mpeg program stream starts with a pack header
	if(refBuffer.ReadUInt32BE() == ((MPEG1_STARTCODE_PREFIX << 8) | MPEG1_STARTCODE_PACKHEADER))
	{
		if(refBuffer.HitEnd())
			return FORMAT_MATCH_BUFFER_TOO_SMALL;

		return 1;
	}
	
	return 0;
}
//Class header
#include "CDVD_SubtitleCodec.h"

//Public methods
ADecoder *CDVD_SubtitleCodec::CreateDecoder(AStream &refStream) const
{
	return nullptr;
}

IEncoder *CDVD_SubtitleCodec::CreateEncoder() const
{
	return nullptr;
}

AParser *CDVD_SubtitleCodec::CreateParser() const
{
	return nullptr;
}

ECodecId CDVD_SubtitleCodec::GetId() const
{
	return ECodecId::DVD_Subtitle;
}

CString CDVD_SubtitleCodec::GetName() const
{
	return "DVD Subtitle";
}
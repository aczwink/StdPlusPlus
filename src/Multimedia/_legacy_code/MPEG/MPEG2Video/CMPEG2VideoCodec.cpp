//Class header
#include "CMPEG2VideoCodec.h"

//Public methods
ADecoder *CMPEG2VideoCodec::CreateDecoder(AStream &refStream) const
{
	return nullptr;
}

IEncoder *CMPEG2VideoCodec::CreateEncoder() const
{
	return nullptr;
}

AParser *CMPEG2VideoCodec::CreateParser() const
{
	return nullptr;
}

ECodecId CMPEG2VideoCodec::GetId() const
{
	return ECodecId::MPEG2Video;
}

CString CMPEG2VideoCodec::GetName() const
{
	return "MPEG-2 Video";
}
//Class header
#include "CH264Codec.h"
//Local

//Public methods
ADecoder *CH264Codec::CreateDecoder(AStream &refStream) const
{
	return nullptr;
}

IEncoder *CH264Codec::CreateEncoder() const
{
	return nullptr;
}

AParser *CH264Codec::CreateParser() const
{
	return nullptr;
}

ECodecId CH264Codec::GetId() const
{
	return ECodecId::H264;
}

CString CH264Codec::GetName() const
{
	return "H.264 / Advanced Video Coding (AVC) / MPEG-4 Part 10";
}
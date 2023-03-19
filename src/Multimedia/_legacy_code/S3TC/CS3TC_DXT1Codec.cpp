//Class header
#include "CS3TC_DXT1Codec.h"
//Local
#include "CS3TCDecoder.h"
#include "CS3TCEncoder.h"

//Public methods
ADecoder *CS3TC_DXT1Codec::CreateDecoder(AStream &refStream) const
{
	return new CS3TCDecoder(1, refStream);
}

IEncoder *CS3TC_DXT1Codec::CreateEncoder() const
{
	return new CS3TCEncoder(1);
}

AParser *CS3TC_DXT1Codec::CreateParser() const
{
	return nullptr;
}

ECodecId CS3TC_DXT1Codec::GetId() const
{
	return ECodecId::S3TC_DXT1;
}

CString CS3TC_DXT1Codec::GetName() const
{
	return "S3TC DXT1 - S3 Texture Compression";
}
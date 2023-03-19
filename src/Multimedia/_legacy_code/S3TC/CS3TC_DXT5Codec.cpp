//Class header
#include "CS3TC_DXT5Codec.h"
//Local
#include "CS3TCDecoder.h"
#include "CS3TCEncoder.h"

//Public methods
ADecoder *CS3TC_DXT5Codec::CreateDecoder(AStream &refStream) const
{
	return new CS3TCDecoder(5, refStream);
}

IEncoder *CS3TC_DXT5Codec::CreateEncoder() const
{
	return new CS3TCEncoder(5);
}

AParser *CS3TC_DXT5Codec::CreateParser() const
{
	return nullptr;
}

ECodecId CS3TC_DXT5Codec::GetId() const
{
	return ECodecId::S3TC_DXT5;
}

CString CS3TC_DXT5Codec::GetName() const
{
	return "S3TC DXT5 - S3 Texture Compression";
}
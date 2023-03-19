//Class header
#include "CRGB24Codec.h"
//Local
#include "CRGB24Decoder.h"
#include "CRGB24Encoder.h"

//Public methods
ADecoder *CRGB24Codec::CreateDecoder(AStream &refStream) const
{
	return new CRGB24Decoder((CVideoStream &)refStream);
}

IEncoder *CRGB24Codec::CreateEncoder() const
{
	return new CRGB24Encoder;
}

AParser *CRGB24Codec::CreateParser() const
{
	return nullptr;
}

ECodecId CRGB24Codec::GetId() const
{
	return ECodecId::RGB24;
}

CString CRGB24Codec::GetName() const
{
	return "24-bit R8G8B8 Interleaved (RGB24)";
}
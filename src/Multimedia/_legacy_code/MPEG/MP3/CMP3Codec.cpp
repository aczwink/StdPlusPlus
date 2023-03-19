//Class header
#include "CMP3Codec.h"
//Local
#include "../CMPEGAudioParser.h"
#include "CMP3Decoder.h"

//Public methods
ADecoder *CMP3Codec::CreateDecoder(AStream &refStream) const
{
	return new CMP3Decoder((CAudioStream &)refStream);
}

IEncoder *CMP3Codec::CreateEncoder() const
{
	return nullptr;
}

AParser *CMP3Codec::CreateParser() const
{
	return new CMPEGAudioParser;
}

ECodecId CMP3Codec::GetId() const
{
	return ECodecId::MP3;
}

CString CMP3Codec::GetName() const
{
	return "MPEG-1/2/2.5 Audio Layer III (MP3)";
}
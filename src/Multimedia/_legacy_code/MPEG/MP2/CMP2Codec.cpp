//Class header
#include "CMP2Codec.h"
//Local
#include "../CMPEGAudioParser.h"
#include "CMP2Decoder.h"

//Public methods
ADecoder *CMP2Codec::CreateDecoder(AStream &refStream) const
{
	return new CMP2Decoder((CAudioStream &)refStream);
}

IEncoder *CMP2Codec::CreateEncoder() const
{
	return nullptr;
}

AParser *CMP2Codec::CreateParser() const
{
	return new CMPEGAudioParser;
}

ECodecId CMP2Codec::GetId() const
{
	return ECodecId::MP2;
}

CString CMP2Codec::GetName() const
{
	return "MPEG-1/2 Audio Layer II (MP2)";
}
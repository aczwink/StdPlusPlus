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
AParser *CMP3Codec::CreateParser() const
{
	return new CMPEGAudioParser;
}
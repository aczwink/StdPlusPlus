//Class header
#include "CBGRA32Codec.h"
//Local
#include "CBGRA32Decoder.h"

//Public methods
ADecoder *CBGRA32Codec::CreateDecoder(AStream &refStream) const
{
	return new CBGRA32Decoder((CVideoStream &)refStream);
}

IEncoder *CBGRA32Codec::CreateEncoder() const
{
	return nullptr;
}

AParser *CBGRA32Codec::CreateParser() const
{
	return nullptr;
}

ECodecId CBGRA32Codec::GetId() const
{
	return ECodecId::BGRA32;
}

CString CBGRA32Codec::GetName() const
{
	return "32-bit B8G8R8A8 Interleaved (BGRA32)";
}
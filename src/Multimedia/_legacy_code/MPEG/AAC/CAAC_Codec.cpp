//Class header
#include "CAAC_Codec.h"
//Local

//Public methods
ADecoder *CAAC_Codec::CreateDecoder(AStream &refStream) const
{
	return nullptr;
}

IEncoder *CAAC_Codec::CreateEncoder() const
{
	return nullptr;
}

AParser *CAAC_Codec::CreateParser() const
{
	return nullptr;
}

ECodecId CAAC_Codec::GetId() const
{
	return ECodecId::AAC;
}

CString CAAC_Codec::GetName() const
{
	return "Advanced Audio Coding (AAC)";
}
//Class header
#include "CMS_MPEG4_Part2Codec.h"
//Local

//Public methods
ADecoder *CMS_MPEG4_Part2Codec::CreateDecoder(AStream &refStream) const
{
	return nullptr;
}

IEncoder *CMS_MPEG4_Part2Codec::CreateEncoder() const
{
	return nullptr;
}

AParser *CMS_MPEG4_Part2Codec::CreateParser() const
{
	return nullptr;
}

ECodecId CMS_MPEG4_Part2Codec::GetId() const
{
	return ECodecId::MS_MPEG4Part2;
}

CString CMS_MPEG4_Part2Codec::GetName() const
{
	return "Microsoft MPEG-4 Part2";
}
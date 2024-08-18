//Class header
#include "CMS_MPEG4_Part2Codec.h"
//Local

//Public methods
ECodecId CMS_MPEG4_Part2Codec::GetId() const
{
	return ECodecId::MS_MPEG4Part2;
}

CString CMS_MPEG4_Part2Codec::GetName() const
{
	return "Microsoft MPEG-4 Part2";
}
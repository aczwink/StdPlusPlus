//Class Header
#include "__CMPEG4Part2Codec.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;

//Public methods
ECodecId CMPEG4Part2Codec::GetId() const
{
	return CODEC_ID_MPEG4PART2;
}

CString CMPEG4Part2Codec::GetName() const
{
	return "MPEG-4 Part 2";
}

ADecoder *CMPEG4Part2Codec::RequestDecoderInstance() const
{
	return NULL;
}

IEncoder *CMPEG4Part2Codec::RequestEncoderInstance() const
{
	return NULL;
}
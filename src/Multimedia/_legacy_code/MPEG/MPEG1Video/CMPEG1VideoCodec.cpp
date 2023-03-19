//Class header
#include "CMPEG1VideoCodec.h"
//Local
#include "../CMPEGVideoParser.h"
#include "CMPEG1VideoDecoder.h"

//Public methods
ADecoder *CMPEG1VideoCodec::CreateDecoder(AStream &refStream) const
{
	return new CMPEG1VideoDecoder((CVideoStream &)refStream);
}

IEncoder *CMPEG1VideoCodec::CreateEncoder() const
{
	return nullptr;
}

AParser *CMPEG1VideoCodec::CreateParser() const
{
	return new CMPEGVideoParser;
}

ECodecId CMPEG1VideoCodec::GetId() const
{
	return ECodecId::MPEG1Video;
}

CString CMPEG1VideoCodec::GetName() const
{
	return "MPEG-1 Video";
}
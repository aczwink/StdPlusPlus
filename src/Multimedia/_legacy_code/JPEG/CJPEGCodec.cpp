//Class header
#include "CJPEGCodec.h"
//Local
#include "CJPEGDecoder.h"

//Public methods
ADecoder *CJPEGCodec::CreateDecoder(AStream &refStream) const
{
	return new CJPEGDecoder((CVideoStream &)refStream);
}

IEncoder *CJPEGCodec::CreateEncoder() const
{
	return nullptr;
}

AParser *CJPEGCodec::CreateParser() const
{
	return nullptr;
}

ECodecId CJPEGCodec::GetId() const
{
	return ECodecId::JPEG;
}

CString CJPEGCodec::GetName() const
{
	return "JPEG";
}
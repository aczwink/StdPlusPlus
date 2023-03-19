//Class header
#include "CBGR24Codec.h"
//Local
#include "CBGR24Decoder.h"
#include "CBGR24Encoder.h"

//Public methods
ADecoder *CBGR24Codec::CreateDecoder(AStream &refStream) const
{
	return new CBGR24Decoder(refStream);
}

IEncoder *CBGR24Codec::CreateEncoder() const
{
	return new CBGR24Encoder;
}

AParser *CBGR24Codec::CreateParser() const
{
	return nullptr;
}

ECodecId CBGR24Codec::GetId() const
{
	return ECodecId::BGR24;
}

CString CBGR24Codec::GetName() const
{
	return "24-bit B8G8R8 Interleaved (BGR24)";
}
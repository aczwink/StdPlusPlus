//Class Header
#include "__CMS_ADPCMCodec.h"
//Local
#include "__CMS_ADPCMDecoder.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;

ECodecId CMS_ADPCMCodec::GetId() const
{
	return CODEC_ID_MS_ADPCM;
}

CString CMS_ADPCMCodec::GetName() const
{
	return "Microsoft ADPCM";
}

ADecoder *CMS_ADPCMCodec::RequestDecoderInstance() const
{
	return new CMS_ADPCMDecoder;
}

IEncoder *CMS_ADPCMCodec::RequestEncoderInstance() const
{
	return NULL;
}
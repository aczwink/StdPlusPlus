//Local
#include "AAudioCodec.h"

namespace SJCMMLib
{
	class CMS_ADPCMCodec : public AAudioCodec
	{
	public:
		//Methods
		ECodecId GetId() const;
		SJCLib::CString GetName() const;
		ADecoder *RequestDecoderInstance() const;
		IEncoder *RequestEncoderInstance() const;
	};
}
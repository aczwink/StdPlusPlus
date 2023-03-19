//Local
#include "AVideoCodec.h"

namespace SJCMMLib
{
	class CMPEG4Part2Codec : public AVideoCodec
	{
	public:
		//Methods
		ECodecId GetId() const;
		SJCLib::CString GetName() const;
		ADecoder *RequestDecoderInstance() const;
		IEncoder *RequestEncoderInstance() const;
	};
}
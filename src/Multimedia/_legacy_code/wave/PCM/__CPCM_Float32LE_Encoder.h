/*//Local
#include "AAudioEncoder.h"

namespace SJCMMLib
{
	class CPCM_Float32LE_Encoder : public AAudioEncoder
	{
	public:
		//Constructor
		CPCM_Float32LE_Encoder();
		//Functions
		void Encode(SPacket *pPacket, const IFrame *pFrame) const;
		uint16 GetBitsPerSample() const;
		ECodecId GetId() const;
		SJCLib::CString GetName() const;
		void Release();
	};
}*/
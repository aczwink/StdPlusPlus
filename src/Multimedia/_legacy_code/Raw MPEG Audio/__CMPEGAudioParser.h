//Local
#include "AParser.h"

namespace SJCMMLib
{
	class CMPEGAudioParser : public AParser
	{
	private:
		//Variables
		uint8 nSyncBytes;
		byte syncBytes[6];
	public:
		//Constructor
		CMPEGAudioParser();
		//Functions
		ECodecId GetCodecId() const;
		void Parse(const byte *pBuffer, uint32 bufferSize);
	};
}
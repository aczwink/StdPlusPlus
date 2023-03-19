#pragma once
//Local
#include "CIOContext.h"
#include "EResult.h"

namespace SJCMMLib
{
	class SJCMMLIB_API CInputContext : public CIOContext
	{
	private:
		//Variables
		IDemuxer *pDemux;
		SJCLib::ASeekableInputStream *pInput;
		//Functions
		bool EstimateTimingInfoFromBitrate();
		bool HasFullTimingInfo(bool allStreams = true);
		bool HasFullTimingInfo(uint32 streamIndex);
		SJCLib::CWString ReadID3V2String(uint32 dataSize, SJCLib::ASeekableInputStream &refInput);
		SJCLib::CWString ReadID3V2String(uint8 textEncoding, uint32 dataSize, SJCLib::ASeekableInputStream &refInput);
		uint32 ReadID3V2Frame(SJCLib::ASeekableInputStream &refInput);
		uint32 ReadID3V2Tags(SJCLib::ASeekableInputStream &refInput);
	public:
		//Constructor
		CInputContext();
		//Destructor
		~CInputContext();
		//Functions
		bool FindStreamInfo();
		bool Init(SJCLib::ASeekableInputStream &refInput, const EContainerFormatId *pAllowedFormats, uint32 nAllowedFormats);
		//Inline		
		inline IDemuxer *GetDemuxer()
		{
			return this->pDemux;
		}

		inline SJCLib::ASeekableInputStream *GetInputStream()
		{
			return this->pInput;
		}

		inline bool Init(SJCLib::ASeekableInputStream &refInput)
		{
			return this->Init(refInput, NULL, 0);
		}
		
		inline bool Init(SJCLib::ASeekableInputStream &refInput, EContainerFormatId allowedFormat)
		{
			return this->Init(refInput, &allowedFormat, 1);
		}

		inline void SetDemuxer(IDemuxer *pDemux)
		{
			this->pDemux = pDemux;
		}

		inline void SetInputStream(SJCLib::ASeekableInputStream *pStream)
		{
			this->pInput = pStream;
		}
	};
}
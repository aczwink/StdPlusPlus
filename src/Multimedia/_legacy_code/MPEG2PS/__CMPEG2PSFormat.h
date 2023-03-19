/*//Local
#include "IFormat.h"

namespace SJCMMLib
{
	class CMPEG2PSFormat : public IFormat
	{
	public:
		//Functions
		AEncoder *GetBestEncoder(EDataType streamType) const;
		IDemuxer *GetDemuxer() const;
		SJCLib::CString GetFileExtension() const;
		uint32 GetMaxNoOfStreams(EDataType streamType) const;
		uint32 GetMinNoOfStreams(EDataType streamType) const;
		void GetFormatInfo(SFormatInfo *pFormatInfo) const;
		EContainerFormatId GetId() const;
		IMuxer *GetMuxer() const;
		SJCLib::CString GetName() const;
		EFormatMatch MatchesFormat(byte *pBuffer, uint32 bufferSize) const;
	};
}*/
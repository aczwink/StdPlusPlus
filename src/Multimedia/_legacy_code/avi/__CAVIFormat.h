//Local
#include "IFormat.h"

namespace SJCMMLib
{
	class CAVIFormat : public IFormat
	{
	public:
		//Methods
		SJCLib::CString GetFileExtension() const;
		void GetFormatInfo(SFormatInfo &refFormatInfo) const;
		EContainerFormatId GetId() const;
		SJCLib::CString GetName() const;
		EFormatMatch MatchesFormat(byte *pBuffer, uint32 bufferSize) const;
		IDemuxer *RequestDemuxerInstance() const;
		IMuxer *RequestMuxerInstance() const;
	};
}
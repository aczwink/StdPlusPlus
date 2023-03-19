//Local
#include "IFormat.h"

namespace SJCMMLib
{
	class CMPEG1SystemsFormat : public IFormat
	{
	public:
		//Functions
		SJCLib::CString GetFileExtension() const;
		void GetFormatInfo(SFormatInfo &refFormatInfo) const;
		EContainerFormatId GetId() const;
		SJCLib::CString GetName() const;
		EFormatMatch MatchesFormat(byte *pBuffer, uint32 bufferSize) const;
		IDemuxer *RequestDemuxerInstance() const;
		IMuxer *RequestMuxerInstance() const;
	};
}
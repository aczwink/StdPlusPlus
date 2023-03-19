//Local
#include "IFormat.h"

namespace SJCMMLib
{
	class CBMPFormat : public IFormat
	{
	public:
		//Methods
		SJCLib::CString GetFileExtension() const;
		void GetFormatInfo(SFormatInfo &refFormatInfo) const;
		EContainerFormatId GetId() const;
		IMuxer *RequestMuxerInstance() const;
	};
}
//Local
#include "ARawImageFormat.h"

//JPEG
class CJPEGFormat : public ARawImageFormat
{
public:
	CJPEGFormat()
	{
		this->codecId = ECodecId::JPEG;
	}

	CString GetExtension() const
	{
		return "jpeg";
	}

	CFiniteSet<ECodecId> GetSupportedCodecs(EDataType dataType) const
	{
		CFiniteSet<ECodecId> result;

		NOT_IMPLEMENTED_ERROR;

		return result;
	}
	
	float32 Matches(CBufferInputStream &refBuffer) const
	{
		bool match;
		
		match = refBuffer.ReadByte() == 0xFF && refBuffer.ReadByte() == 0xD8 && refBuffer.ReadByte() == 0xFF;
		if(refBuffer.HitEnd())
			return FORMAT_MATCH_BUFFER_TOO_SMALL;
		
		if(match)
			return 1;

		return 0;
	}
};
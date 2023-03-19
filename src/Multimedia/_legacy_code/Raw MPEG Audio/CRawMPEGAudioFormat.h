#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

class CRawMPEGAudioFormat : public IFormat
{
public:
	//Methods
	ADemuxer *CreateDemuxer(ASeekableInputStream &refInput) const;
	AMuxer *CreateMuxer(ASeekableOutputStream &refOutput) const;
	ECodecId GetDefaultCodec(EDataType dataType) const;
	CString GetExtension() const;
	void GetFormatInfo(SFormatInfo &refFormatInfo) const;
	CString GetName() const;
	CFiniteSet<ECodecId> GetSupportedCodecs(EDataType dataType) const;
	float32 Matches(CBufferInputStream &refBuffer) const;
};
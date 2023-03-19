#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

class CMP3Codec : public ICodec
{
public:
	//Methods
	ADecoder *CreateDecoder(AStream &refStream) const;
	IEncoder *CreateEncoder() const;
	AParser *CreateParser() const;
	ECodecId GetId() const;
	CString GetName() const;
};

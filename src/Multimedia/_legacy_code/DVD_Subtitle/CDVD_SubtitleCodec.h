#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

/*
Sources:
http://dvd.sourceforge.net/spu_notes
http://stnsoft.com/DVD/spu.html

TODO:
https://github.com/FFmpeg/FFmpeg/blob/master/libavcodec/dvdsub_parser.c
https://github.com/FFmpeg/FFmpeg/blob/master/libavcodec/dvdsubdec.c
*/

class CDVD_SubtitleCodec : public ICodec
{
public:
	//Methods
	ADecoder *CreateDecoder(AStream &refStream) const;
	IEncoder *CreateEncoder() const;
	AParser *CreateParser() const;
	ECodecId GetId() const;
	CString GetName() const;
};

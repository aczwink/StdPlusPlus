#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Compression;
using namespace ACStdLib::Multimedia;

class CPNGDecoder : public AVideoDecoder
{
private:
	//Members
	uint8 bitDepth;
	uint8 colorType;
	byte *pPreviousRow;
	byte *pCurrentRow;
	AImage *pImage;
	
	//Methods
	void DecodeData(AInputStream &refInput);
	void DeFilter(byte filterType, uint16 x, uint8 nComponents);
	uint32 GetRowSize() const;
	void ReadImageHeader(AInputStream &refInput);

	//Inline
	inline byte PaethPredictor(byte a, byte b, byte c) const
	{
		int16 p;
		uint16 pa, pb, pc;
		
		p = a + b - c;
		pa = ABS(p - a);
		pb = ABS(p - b);
		pc = ABS(p - c);

		if(pa <= pb && pa <= pc)
			return a;
		else if(pb <= pc)
			return b;
		return c;
	}

public:
	//Constructor
	CPNGDecoder(AStream &refStream);

	//Destructor
	~CPNGDecoder();

	//Methods
	void Decode(const SPacket &refPacket);
	EPixelFormat GetPixelFormat() const;
};
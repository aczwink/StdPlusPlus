#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

class CS3TCEncoder : public IEncoder
{
private:
	//Members
	uint8 dxtNum;

	//Methods
	void EncodeColorBlock(const vec4i32(&refBlock)[16], AOutputStream &refOutput) const;
	uint32 EncodeColorMask(const vec4i32(&refBlock)[16], const vec4i32(&refPalette)[4]) const;
	void EncodeDXT1Block(const vec4i32(&refBlock)[16], AOutputStream &refOutput) const;
	void EncodeDXT5AlphaBlock(const byte (&refBlock)[16], AOutputStream &refOutput) const;
	void FindMinMaxAlpha(byte &refMin, byte &refMax, const byte(&refBlock)[16]) const;
	void FindMinMaxColorsByEuclideanDistance(vec4i32 &refMin, vec4i32 &refMax, const vec4i32(&refBlock)[16]) const;

	//Inline
	inline uint32 ComputeColorDistance(const vec4i32 &refColor0, const vec4i32 &refColor1) const
	{
		vec4i32 tmp;
		
		tmp = (refColor0 - refColor1) * (refColor0 - refColor1);
		
		return tmp.Sum();
	}

	inline uint16 ToRGB565(const vec4i32 &refColor) const
	{
		//r and b are scaled by 31/255, g by 63/255.
		//approx. with shift
		
		return (uint16)((refColor.a >> 3) << 11 | (refColor.b >> 2) << 5 | (refColor.c >> 3));
	}

public:
	//Constructor
	inline CS3TCEncoder(uint8 dxtNum)
	{
		this->dxtNum = dxtNum;
	}
	
	//Methods
	void Encode(const AFrame &refFrame, SPacket &refPacket) const;
};
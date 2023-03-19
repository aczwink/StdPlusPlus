#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;
//Definitions
#define BLOCK_WIDTH 8 //defined in 2.4.1 at 'block'
#define BLOCK_HEIGHT 8 //both in Pels
#define BLOCK_NUMBEROFPELS (BLOCK_WIDTH * BLOCK_HEIGHT) //number of pels in block
#define MACROBLOCK_HEIGHT 16 //both are in Pels (Picture Elements)
#define MACROBLOCK_WIDTH 16 //defined in 2.4.1 at 'macroblock'

/*
Implementation follows:
ISO IEC 11172-2
*/

class CMPEG1VideoDecoder : public AVideoDecoder
{
	//Enums
	enum EPictureType
	{
		PICTURETYPE_I,
		PICTURETYPE_P,
		PICTURETYPE_B,
	};

	enum EState
	{
		STATE_EXPECT_SEQUENCE,
		STATE_EXPECT_GROUPOFPICTURES,
		STATE_EXPECT_PICTURE,
		STATE_EXPECT_SLICE,
		STATE_DECODE_SLICES,
	};

	//Structs
	struct SMacroblockTypeInfo
	{
		bool quant;
		bool motion_forward;
		bool motion_backward;
		bool pattern;
		bool intra;
	};

private:
	//Members
	EState currentState;
	uint16 mb_width; //defined in 2.4.3.2
	uint16 temporal_reference;
	EPictureType currentPicType;
	//Forward Motion Vector
	bool full_pel_forward_vector;
	uint8 forward_f_code;
	//Backward Motion Vector
	bool full_pel_backward_vector;
	uint8 backward_f_code;
	uint8 intra_quantizer_matrix[BLOCK_HEIGHT][BLOCK_WIDTH];
	uint8 non_intra_quantizer_matrix[BLOCK_WIDTH][BLOCK_HEIGHT];
	
	//Frames
	CYCbCr420Image *pFirstPredictionFrame;
	CYCbCr420Image *pSecondPredictionFrame;
	CYCbCr420Image *pCurrentFrame;

	//Methods
	void ApplyInverseDCT8x8(int16(&refDCTCoeffs)[BLOCK_HEIGHT][BLOCK_WIDTH]) const;
	void ConstructBidirectionalPredictor(uint32 bufOffsetY, uint32 bufOffsetC, const int16(&refMotionVectorForward)[2], const int16(&refMotionVectorBackward)[2]);
	void ConstructMonoPredictor(CYCbCr420Image &refPredictorFrame, uint32 bufOffsetY, uint32 bufOffsetC, const int16(&refMotionVector)[2]);
	void CopyIntraBlockToFrame(byte *pBuffer, uint32 lineSize, const int16(&refDCTCoeffs)[BLOCK_HEIGHT][BLOCK_WIDTH]);
	void CopyNonIntraBlockToFrame(byte *pBuffer, uint32 lineSize, const int16(&refDCTCoeffs)[BLOCK_HEIGHT][BLOCK_WIDTH]);
	void DecodeBlock(bool isIntraCoded, bool isLumaBlock, int16(&refdct_zz)[BLOCK_NUMBEROFPELS], CBitInputStream &refBitInput) const;
	void DecodeMacroblock(uint16 macroblock_address, bool skip, SMacroblockTypeInfo &refmacroblock_type, uint8 &refQuantizerScale, int16(&refPredictiveMotionVectorForward)[2], int16(&refPredictiveMotionVectorBackward)[2], int16(&refMotionVectorForward)[2], int16(&refMotionVectorBackward)[2], uint16 &refdct_dc_y_past, uint16 &refdct_dc_cb_past, uint16 &refdct_dc_cr_past, CBitInputStream &refBitInput);
	void DecodeMotionVector(uint8 f_code, int16(&refMotionVector)[2], int16(&refPredictiveMotionVector)[2], bool full_pel_vector, CBitInputStream &refBitInput) const;
	void DecodePicture(ASeekableInputStream &refInput);
	void DecodeSequenceHeader(ASeekableInputStream &refInput);
	void DecodeSlice(uint8 slice_vertical_position, ASeekableInputStream &refInput);
	uint8 *GetBlockOffset(uint8 blockNumber, CYCbCr420Image &refFrame, uint32 bufOffsetY, uint32 bufOffsetC) const;
	byte GetPel(byte *pPel_past, int16 right_half_for, int16 down_half_for, uint16 lineSize) const;
	void InverseQuantizeIntraBlock(uint8 quantizerScale, uint16 &refdct_dc_past, int16(&refdct_recon)[BLOCK_HEIGHT][BLOCK_WIDTH], const int16(&refdct_zz)[BLOCK_NUMBEROFPELS]) const;
	void InverseQuantizeNonIntraBlock(uint8 quantizerScale, int16(&refdct_recon)[BLOCK_HEIGHT][BLOCK_WIDTH], const int16(&refdct_zz)[BLOCK_NUMBEROFPELS]) const;
	bool ReadDCTCoeff(bool first, uint8 &refRun, int16 &refLevel, CBitInputStream &refBitInput) const;
	uint8 ReadDCTDcSizeChrominance(CBitInputStream &refBitInput) const;
	uint8 ReadDCTDcSizeLuminance(CBitInputStream &refBitInput) const;
	uint8 ReadMacroblockAddressIncrement(CBitInputStream &refBitInput) const;
	uint8 ReadMacroblockPattern(CBitInputStream &refBitInput) const;
	void ReadMacroblockType(SMacroblockTypeInfo &refMBTypeInfo, CBitInputStream &refBitInput) const;
	int8 ReadMotionVectorCode(CBitInputStream &refBitInput) const;

	//Inline
	inline uint32 GetLineSize(uint8 blockNumber) const
	{
		if(blockNumber < 4)
			return MACROBLOCK_WIDTH * this->mb_width;
		return BLOCK_WIDTH * this->mb_width;
	}

public:
	//Constructor
	CMPEG1VideoDecoder(AStream &refStream);

	//Destructor
	~CMPEG1VideoDecoder();

	//Methods
	void Decode(const SPacket &refPacket);
	EPixelFormat GetPixelFormat() const;
};
//Class header
#include "CMPEG1VideoDecoder.h"
//Local
#include "../MPEG1.h"

//Tables
static const uint8 scan[8][8] = //defined in 2.4.4.1
{
	{0,  1,  5,  6, 14, 15, 27, 28},
	{2,  4,  7, 13, 16, 26, 29, 42},
	{3,  8, 12, 17, 25, 30, 41, 43},
	{9, 11, 18, 24, 31, 40, 44, 53},
	{10, 19, 23, 32, 39, 45, 52, 54},
	{20, 22, 33, 38, 46, 51, 55, 60},
	{21, 34, 37, 47, 50, 56, 59, 61},
	{35, 36, 48, 49, 57, 58, 62, 63}
};


//Constructor
CMPEG1VideoDecoder::CMPEG1VideoDecoder(AStream &refStream) : AVideoDecoder(refStream)
{
	uint8 m, n;

	this->currentState = STATE_EXPECT_SEQUENCE;

	this->pFirstPredictionFrame = nullptr;
	this->pSecondPredictionFrame = nullptr;

	//Specified in 2.4.3.2 under 'load_intra_quantizer_matrix'
	static uint8 default_intra_quantizer_matrix[BLOCK_WIDTH][BLOCK_HEIGHT] =
	{
		{8, 16, 19, 22, 26, 27, 29, 34},
		{16, 16, 22, 23, 27, 29, 34, 37},
		{19, 22, 26, 27, 29, 34, 34, 38},
		{22, 22, 26, 27, 29, 34, 37, 40},
		{22, 26, 27, 29, 32, 35, 40, 48},
		{26, 27, 29, 32, 35, 40, 48, 58},
		{26, 27, 29, 34, 38, 46, 56, 69},
		{27, 29, 35, 38, 46, 56, 69, 83}
	};

	MemCopy(this->intra_quantizer_matrix, default_intra_quantizer_matrix, sizeof(this->intra_quantizer_matrix));

	//2.4.3.2 says that for default non_intra_quantizer_matrix all values are 16
	for(m = 0; m < BLOCK_HEIGHT; m++)
	{
		for(n = 0; n < BLOCK_WIDTH; n++)
		{
			this->non_intra_quantizer_matrix[m][n] = 16;
		}
	}
}

//Destructor
CMPEG1VideoDecoder::~CMPEG1VideoDecoder()
{
	this->pCurrentFrame = nullptr;
	if(this->pFirstPredictionFrame)
		delete this->pFirstPredictionFrame;
	if(this->pSecondPredictionFrame)
		delete this->pSecondPredictionFrame;
}

//Private methods
void CMPEG1VideoDecoder::ApplyInverseDCT8x8(int16(&refDCTCoeffs)[BLOCK_HEIGHT][BLOCK_WIDTH]) const
{
	uint8 i, m, n;
	float tmp[8][8], tmp2[8][8];

	static const double twiddleInv[8][8] =
	{
		{0.353553390593273790, 0.490392640201615220, 0.461939766255643370, 0.415734806151272620, 0.353553390593273790, 0.277785116509801140, 0.191341716182544920, 0.097545161008064166},
		{0.353553390593273790, 0.415734806151272620, 0.191341716182544920, -0.097545161008064096, -0.353553390593273730, -0.490392640201615220, -0.461939766255643420, -0.277785116509801090},
		{0.353553390593273790, 0.277785116509801140, -0.191341716182544860, -0.490392640201615220, -0.353553390593273840, 0.097545161008064152, 0.461939766255643260, 0.415734806151272730},
		{0.353553390593273790, 0.097545161008064166, -0.461939766255643370, -0.277785116509801090, 0.353553390593273680, 0.415734806151272730, -0.191341716182544950, -0.490392640201615330},
		{0.353553390593273790, -0.097545161008064096, -0.461939766255643420, 0.277785116509800920, 0.353553390593273840, -0.415734806151272560, -0.191341716182545280, 0.490392640201615220},
		{0.353553390593273790, -0.277785116509800980, -0.191341716182545170, 0.490392640201615220, -0.353553390593273340, -0.097545161008064013, 0.461939766255643370, -0.415734806151272510},
		{0.353553390593273790, -0.415734806151272670, 0.191341716182545000, 0.097545161008064388, -0.353553390593273560, 0.490392640201615330, -0.461939766255643200, 0.277785116509800760},
		{0.353553390593273790, -0.490392640201615220, 0.461939766255643260, -0.415734806151272560, 0.353553390593273290, -0.277785116509800760, 0.191341716182544780, -0.097545161008064291}
	};

	for(i = 0; i < 8; i++)
	{
		for(m = 0; m < 8; m++)
		{
			tmp[i][m] = 0;

			for(n = 0; n < 8; n++)
			{
				tmp[i][m] += float(refDCTCoeffs[i][n] * twiddleInv[m][n]);
			}
		}
	}

	for(i = 0; i < 8; i++)
	{
		for(m = 0; m < 8; m++)
		{
			tmp2[m][i] = 0;

			for(n = 0; n < 8; n++)
			{
				tmp2[m][i] += float(tmp[n][i] * twiddleInv[m][n]);
			}
		}
	}

	for(m = 0; m < 8; m++)
	{
		for(n = 0; n < 8; n++)
		{
			refDCTCoeffs[m][n] = int16((tmp2[m][n] > 0) ? (tmp2[m][n] + 0.5) : (tmp2[m][n] - 0.5));
		}
	}
}

void CMPEG1VideoDecoder::ConstructBidirectionalPredictor(uint32 bufOffsetY, uint32 bufOffsetC, const int16(&refMotionVectorForward)[2], const int16(&refMotionVectorBackward)[2])
{
	byte b1, b2;
	byte *pBufferCurrent, *pBufferFirst, *pBufferSecond;
	uint8 blockNumber, m, n;
	int16 right_for, down_for, right_half_for, down_half_for, right_back, down_back, right_half_back, down_half_back, currentIndex, lineSize;
	int32 offset1, offset2;

	//2.4.4.3

	right_for = refMotionVectorForward[0] >> 1;
	down_for = refMotionVectorForward[1] >> 1;
	right_half_for = refMotionVectorForward[0] - (2 * right_for);
	down_half_for = refMotionVectorForward[1] - (2 * down_for);
	offset1 = (down_for * 16 * this->mb_width) + right_for;

	right_back = refMotionVectorBackward[0] >> 1;
	down_back = refMotionVectorBackward[1] >> 1;
	right_half_back = refMotionVectorBackward[0] - (2 * right_back);
	down_half_back = refMotionVectorBackward[1] - (2 * down_back);
	offset2 = (down_back * 16 * this->mb_width) + right_back;

	//4 Y' blocks
	for(blockNumber = 0; blockNumber < 4; blockNumber++)
	{
		pBufferCurrent = this->GetBlockOffset(blockNumber, *this->pCurrentFrame, bufOffsetY, 0);
		pBufferFirst = this->GetBlockOffset(blockNumber, *this->pFirstPredictionFrame, bufOffsetY + offset1, 0);
		pBufferSecond = this->GetBlockOffset(blockNumber, *this->pSecondPredictionFrame, bufOffsetY + offset2, 0);
		currentIndex = 0;
		lineSize = this->GetLineSize(blockNumber);

		for(m = 0; m < BLOCK_HEIGHT; m++)
		{
			for(n = 0; n < BLOCK_WIDTH; n++)
			{
				b1 = this->GetPel(pBufferFirst + currentIndex, right_half_for, down_half_for, lineSize);
				b2 = this->GetPel(pBufferSecond + currentIndex, right_half_back, down_half_back, lineSize);
				*(pBufferCurrent + currentIndex) = ((int32)b1 + (int32)b2) / 2;

				currentIndex++;
			}

			currentIndex += lineSize - BLOCK_WIDTH;
		}
	}

	right_for = (refMotionVectorForward[0] / 2) >> 1;
	down_for = (refMotionVectorForward[1] / 2) >> 1;
	right_half_for = (refMotionVectorForward[0] / 2) - (2 * right_for);
	down_half_for = (refMotionVectorForward[1] / 2) - (2 * down_for);
	offset1 = (down_for * 16 * this->mb_width) + right_for;

	right_back = (refMotionVectorBackward[0] / 2) >> 1;
	down_back = (refMotionVectorBackward[1] / 2) >> 1;
	right_half_back = (refMotionVectorBackward[0] / 2) - (2 * right_back);
	down_half_back = (refMotionVectorBackward[1] / 2) - (2 * down_back);
	offset2 = (down_back * 16 * this->mb_width) + right_back;

	lineSize = this->GetLineSize(4); //same for 5

	for(m = 0; m < BLOCK_HEIGHT; m++)
	{
		for(n = 0; n < BLOCK_WIDTH; n++)
		{
			//cb block
			b1 = this->GetPel(this->pFirstPredictionFrame->GetChromaBlueChannel() + bufOffsetC + offset1, right_half_for, down_half_for, lineSize);
			b2 = this->GetPel(this->pSecondPredictionFrame->GetChromaBlueChannel() + bufOffsetC + offset2, right_half_back, down_half_back, lineSize);
			this->pCurrentFrame->GetChromaBlueChannel()[bufOffsetC] = ((int32)b1 + (int32)b2) / 2;
			//cr block
			b1 = this->GetPel(this->pFirstPredictionFrame->GetChromaRedChannel() + bufOffsetC + offset1, right_half_for, down_half_for, lineSize);
			b2 = this->GetPel(this->pSecondPredictionFrame->GetChromaRedChannel() + bufOffsetC + offset2, right_half_back, down_half_back, lineSize);
			this->pCurrentFrame->GetChromaRedChannel()[bufOffsetC] = ((int32)b1 + (int32)b2) / 2;

			bufOffsetC++;
		}

		bufOffsetC += lineSize - BLOCK_WIDTH;
	}
}

void CMPEG1VideoDecoder::ConstructMonoPredictor(CYCbCr420Image &refPredictorFrame, uint32 bufOffsetY, uint32 bufOffsetC, const int16(&refMotionVector)[2])
{
	byte *pBuffer, *pPredictorBuffer;
	uint8 blockNumber, m, n;
	int16 right_for, down_for, right_half_for, down_half_for;
	uint16 lineSize;
	int32 offset;

	//2.4.4.2

	right_for = refMotionVector[0] >> 1;
	down_for = refMotionVector[1] >> 1;
	right_half_for = refMotionVector[0] - (2 * right_for);
	down_half_for = refMotionVector[1] - (2 * down_for);
	offset = (down_for * 16 * this->mb_width) + right_for;

	lineSize = this->GetLineSize(0); //same for all 4

									 //4 Y' blocks
	for(blockNumber = 0; blockNumber < 4; blockNumber++)
	{
		pBuffer = this->GetBlockOffset(blockNumber, *this->pCurrentFrame, bufOffsetY, 0);
		pPredictorBuffer = this->GetBlockOffset(blockNumber, refPredictorFrame, bufOffsetY + offset, 0);

		for(m = 0; m < BLOCK_HEIGHT; m++)
		{
			for(n = 0; n < BLOCK_WIDTH; n++)
			{
				*pBuffer++ = this->GetPel(pPredictorBuffer++, right_half_for, down_half_for, lineSize);
			}

			pBuffer += lineSize - BLOCK_WIDTH;
			pPredictorBuffer += lineSize - BLOCK_WIDTH;
		}
	}

	right_for = (refMotionVector[0] / 2) >> 1;
	down_for = (refMotionVector[1] / 2) >> 1;
	right_half_for = (refMotionVector[0] / 2) - (2 * right_for);
	down_half_for = (refMotionVector[1] / 2) - (2 * down_for);
	offset = (down_for * 16 * this->mb_width) + right_for;

	lineSize = this->GetLineSize(4); //same for 5

	for(m = 0; m < BLOCK_HEIGHT; m++)
	{
		for(n = 0; n < BLOCK_WIDTH; n++)
		{
			this->pCurrentFrame->GetChromaBlueChannel()[bufOffsetC] = this->GetPel(refPredictorFrame.GetChromaBlueChannel() + bufOffsetC + offset, right_half_for, down_half_for, lineSize);
			this->pCurrentFrame->GetChromaRedChannel()[bufOffsetC] = this->GetPel(refPredictorFrame.GetChromaRedChannel() + bufOffsetC + offset, right_half_for, down_half_for, lineSize);

			bufOffsetC++;
		}

		bufOffsetC += lineSize - BLOCK_WIDTH;
	}
}

void CMPEG1VideoDecoder::CopyIntraBlockToFrame(uint8 *pBuffer, uint32 lineSize, const int16(&refDCTCoeffs)[BLOCK_HEIGHT][BLOCK_WIDTH])
{
	uint8 m, n;

	for(m = 0; m < BLOCK_HEIGHT; m++)
	{
		for(n = 0; n < BLOCK_WIDTH; n++)
		{
			*pBuffer++ = CLAMP(refDCTCoeffs[m][n], 0, 255);
		}

		pBuffer += lineSize - BLOCK_WIDTH; //next line
	}
}

void CMPEG1VideoDecoder::CopyNonIntraBlockToFrame(byte *pBuffer, uint32 lineSize, const int16(&refDCTCoeffs)[BLOCK_HEIGHT][BLOCK_WIDTH])
{
	uint8 m, n;
	int32 tmp;

	for(m = 0; m < BLOCK_HEIGHT; m++)
	{
		for(n = 0; n < BLOCK_WIDTH; n++)
		{
			tmp = *pBuffer + refDCTCoeffs[m][n];

			*pBuffer++ = CLAMP(tmp, 0, 255);
		}

		pBuffer += lineSize - BLOCK_WIDTH; //next line
	}
}

void CMPEG1VideoDecoder::DecodeBlock(bool isIntraCoded, bool isLumaBlock, int16(&refdct_zz)[BLOCK_NUMBEROFPELS], CBitInputStream &refBitInput) const
{
	uint8 dct_dc_size, dct_dc_differential, currentIndex, run;
	int16 level;

	//2.4.2.8

	currentIndex = 0;

	if(isIntraCoded)
	{
		if(isLumaBlock)
		{
			//luminance
			dct_dc_size = this->ReadDCTDcSizeLuminance(refBitInput);
		}
		else
		{
			//chrominance
			dct_dc_size = this->ReadDCTDcSizeChrominance(refBitInput);
		}

		if(dct_dc_size > 0)
		{
			dct_dc_differential = (uint8)refBitInput.Read(dct_dc_size);

			//2.4.3.7
			if(dct_dc_differential & (1 << (dct_dc_size - 1)))
				refdct_zz[0] = dct_dc_differential;
			else
				refdct_zz[0] = (-1 << dct_dc_size) | (dct_dc_differential + 1);
		}
		else
			refdct_zz[0] = 0; //2.4.3.7

		currentIndex++;
	}

	while(true)
	{
		//if pictureType != 4
		{
			if(!this->ReadDCTCoeff(currentIndex == 0, run, level, refBitInput))
				break; //End of block

			currentIndex += run;
			ASSERT(currentIndex < BLOCK_NUMBEROFPELS);
			refdct_zz[currentIndex++] = level;
		}
	}
}

void CMPEG1VideoDecoder::DecodeMacroblock(uint16 macroblock_address, bool skip, SMacroblockTypeInfo &refmacroblock_type, uint8 &refQuantizerScale, int16(&refPredictiveMotionVectorForward)[2], int16(&refPredictiveMotionVectorBackward)[2], int16(&refMotionVectorForward)[2], int16(&refMotionVectorBackward)[2], uint16 &refdct_dc_y_past, uint16 &refdct_dc_cb_past, uint16 &refdct_dc_cr_past, CBitInputStream &refBitInput)
{
	uint8 coded_block_pattern;
	uint8 *pBuffer;
	int16 dct_zz[BLOCK_NUMBEROFPELS];
	int16 dct_recon[BLOCK_HEIGHT][BLOCK_WIDTH];
	uint16 mb_row, mb_col; //defined in 2.4.3.6
	uint32 bufOffsetY, bufOffsetC;

	//2.4.2.7

	//formulas from 2.4.3.6
	mb_row = macroblock_address / this->mb_width;
	mb_col = macroblock_address % this->mb_width;

	bufOffsetY = mb_row * MACROBLOCK_HEIGHT * (16 * this->mb_width) + mb_col * MACROBLOCK_WIDTH;
	bufOffsetC = mb_row * BLOCK_HEIGHT * (8 * this->mb_width) + mb_col * BLOCK_WIDTH;

	if(!skip) //2.4.4.4
	{
		this->ReadMacroblockType(refmacroblock_type, refBitInput);

		if(refmacroblock_type.quant)
			refQuantizerScale = (uint8)refBitInput.Read(5);

		//If macroblock is intra coded, reset the motion vectors (2.4.4.2)
		if(refmacroblock_type.intra)
		{
			refPredictiveMotionVectorForward[0] = 0;
			refPredictiveMotionVectorForward[1] = 0;
			refPredictiveMotionVectorBackward[0] = 0;
			refPredictiveMotionVectorBackward[1] = 0;
		}

		if(refmacroblock_type.motion_forward)
		{
			this->DecodeMotionVector(forward_f_code, refMotionVectorForward, refPredictiveMotionVectorForward, full_pel_forward_vector, refBitInput);
		}

		if(refmacroblock_type.motion_backward)
		{
			this->DecodeMotionVector(backward_f_code, refMotionVectorBackward, refPredictiveMotionVectorBackward, full_pel_backward_vector, refBitInput);
		}
	}

	if(!refmacroblock_type.intra || skip)
	{
		//Macroblock is not intra-coded

		//2.4.4.1 says: reset the dct_recon[0][0] predictors
		refdct_dc_y_past = 1024;
		refdct_dc_cb_past = 1024;
		refdct_dc_cr_past = 1024;

		//Very important clause in 2.4.4.2
		if(this->currentPicType == PICTURETYPE_P && (skip || !refmacroblock_type.motion_forward))
		{
			//2.4.4.4 says that the reconstructed motion vector should be zero... means that the predictive ones will be two
			refMotionVectorForward[0] = 0;
			refMotionVectorForward[1] = 0;
			refPredictiveMotionVectorForward[0] = 0;
			refPredictiveMotionVectorForward[1] = 0;

			this->ConstructMonoPredictor(*this->pFirstPredictionFrame, bufOffsetY, bufOffsetC, refMotionVectorForward);
		}
		else
		{
			if(refmacroblock_type.motion_forward && refmacroblock_type.motion_backward)
			{
				this->ConstructBidirectionalPredictor(bufOffsetY, bufOffsetC, refMotionVectorForward, refMotionVectorBackward);
			}
			else if(refmacroblock_type.motion_forward)
			{
				this->ConstructMonoPredictor(*this->pFirstPredictionFrame, bufOffsetY, bufOffsetC, refMotionVectorForward);
			}
			else if(refmacroblock_type.motion_backward)
			{
				this->ConstructMonoPredictor(*this->pSecondPredictionFrame, bufOffsetY, bufOffsetC, refMotionVectorBackward);
			}
		}
	}

	if(!skip)
	{
		//2.4.3.6 section 'coded_block_pattern'
		if(refmacroblock_type.intra)
		{
			coded_block_pattern = 0x3F; //0011 1111 -> all blocks
		}
		else if(refmacroblock_type.pattern)
		{
			coded_block_pattern = this->ReadMacroblockPattern(refBitInput);
		}
		else
		{
			coded_block_pattern = 0;
		}

		/*
		4 Y' Blocks ordered:
		Y0 | Y1
		Y2 | Y3
		followed by one Cb and one Cr block
		*/
		for(uint8 blockNumber = 0; blockNumber < 6; blockNumber++)
		{
			//The coded_block_pattern says which block needs work
			if(coded_block_pattern & (32 >> blockNumber))
			{
				//2.4.3.7 says that dct_zz should be zero initially
				for(uint8 i = 0; i < BLOCK_NUMBEROFPELS; i++)
					dct_zz[i] = 0;

				this->DecodeBlock(refmacroblock_type.intra, blockNumber < 4, dct_zz, refBitInput);

				if(refmacroblock_type.intra)
				{
					switch(blockNumber)
					{
					case 4:
						this->InverseQuantizeIntraBlock(refQuantizerScale, refdct_dc_cb_past, dct_recon, dct_zz);
						break;
					case 5:
						this->InverseQuantizeIntraBlock(refQuantizerScale, refdct_dc_cr_past, dct_recon, dct_zz);
						break;
					default:
						this->InverseQuantizeIntraBlock(refQuantizerScale, refdct_dc_y_past, dct_recon, dct_zz);
					}
				}
				else
				{
					this->InverseQuantizeNonIntraBlock(refQuantizerScale, dct_recon, dct_zz);
				}

				this->ApplyInverseDCT8x8(dct_recon);

				pBuffer = this->GetBlockOffset(blockNumber, *this->pCurrentFrame, bufOffsetY, bufOffsetC);

				if(refmacroblock_type.intra)
				{
					this->CopyIntraBlockToFrame(pBuffer, this->GetLineSize(blockNumber), dct_recon);
				}
				else
				{
					this->CopyNonIntraBlockToFrame(pBuffer, this->GetLineSize(blockNumber), dct_recon);
				}
			}
		}
	}
}

void CMPEG1VideoDecoder::DecodeMotionVector(uint8 f_code, int16(&refMotionVector)[2], int16(&refPredictiveMotionVector)[2], bool full_pel_vector, CBitInputStream &refBitInput) const
{
	int8 motionCode;
	uint8 r_size, f, r, complement_r;
	int16 little, big, min, new_vector;
	uint16 max;

	//2.4.4.2

	r_size = f_code - 1;
	f = 1 << r_size; //forward_f

	for(uint8 i = 0; i < 2; i++)
	{
		motionCode = this->ReadMotionVectorCode(refBitInput); //motion_horizontal_forward_code

		if((f_code != 1) && (motionCode != 0)) //2.4.2.7
		{
			r = (uint8)refBitInput.Read(r_size); //motion_horizontal_forward_r
		}

		if((f == 1) || (motionCode == 0))
		{
			complement_r = 0;
		}
		else
		{
			complement_r = f - 1 - r;
		}

		little = motionCode * f;
		if(little == 0)
		{
			big = 0;
		}
		else
		{
			if(little > 0)
			{
				little -= complement_r;
				big = little - (32 * f);
			}
			else
			{
				little += complement_r;
				big = little + (32 * f);
			}
		}

		max = (16 * f) - 1;
		min = (-16 * f);

		new_vector = refPredictiveMotionVector[i] + little;
		if((new_vector <= max) && (new_vector >= min))
			refMotionVector[i] = refPredictiveMotionVector[i] + little;
		else
			refMotionVector[i] = refPredictiveMotionVector[i] + big;
		refPredictiveMotionVector[i] = refMotionVector[i];

		if(full_pel_vector)
			refMotionVector[i] <<= 1;
	}
}

void CMPEG1VideoDecoder::DecodePicture(ASeekableInputStream &refInput)
{
	CBitInputStream &refBitStream = CBitInputStream(refInput);

	//2.4.2.5

	this->temporal_reference = (uint16)refBitStream.Read(10);

	switch(refBitStream.Read(3))
	{
	case 1:
		this->currentPicType = PICTURETYPE_I;
		break;
	case 2:
		this->currentPicType = PICTURETYPE_P;
		break;
	case 3:
		this->currentPicType = PICTURETYPE_B;
		break;
	default:
		ASSERT(0);
	}

	refBitStream.Skip(16); //vbv_delay

	if(this->currentPicType == PICTURETYPE_P || this->currentPicType == PICTURETYPE_B)
	{
		this->full_pel_forward_vector = refBitStream.Read(1) != 0;
		this->forward_f_code = (uint8)refBitStream.Read(3);
	}

	if(this->currentPicType == PICTURETYPE_B)
	{
		this->full_pel_backward_vector = refBitStream.Read(1) != 0;
		this->backward_f_code = (uint8)refBitStream.Read(3);
	}

	while(refBitStream.Read(1))
	{
		refBitStream.Skip(8); //extra_information_picture
	}

	//Allocate prediction buffers
	if(!this->pFirstPredictionFrame || this->pFirstPredictionFrame->GetWidth() != this->refStream.width || this->pFirstPredictionFrame->GetHeight() != this->refStream.height)
	{
		this->pFirstPredictionFrame = new CYCbCr420Image(this->refStream.width, this->refStream.height, false);
	}

	if(!this->pSecondPredictionFrame || this->pSecondPredictionFrame->GetWidth() != this->refStream.width || this->pSecondPredictionFrame->GetHeight() != this->refStream.height)
	{
		this->pSecondPredictionFrame = new CYCbCr420Image(this->refStream.width, this->refStream.height, false);
	}

	if(this->currentPicType == PICTURETYPE_B)
	{
		this->pCurrentFrame = new CYCbCr420Image(this->refStream.width, this->refStream.height, false);
	}
	else
	{
		this->pCurrentFrame = this->pFirstPredictionFrame;

		//Rotate the prediction buffers
		this->pFirstPredictionFrame = this->pSecondPredictionFrame;
		this->pSecondPredictionFrame = this->pCurrentFrame;
	}
}

void CMPEG1VideoDecoder::DecodeSequenceHeader(ASeekableInputStream &refInput)
{
	uint32 bitRate;
	
	CBitInputStream &refBitStream = CBitInputStream(refInput);

	//2.4.2.3

	this->refStream.width = (uint16)refBitStream.Read(12);
	this->refStream.height = (uint16)refBitStream.Read(12);

	refBitStream.Skip(4); //pel_aspect_ratio
	refBitStream.Skip(4); //picture_rate
	
	bitRate = (uint32)refBitStream.Read(18);
	if(bitRate == 0x3FFFF)
		this->refStream.vbr = true;
	else
		this->refStream.bitRate = bitRate * 400;
	
	ASSERT(refBitStream.Read(1)); //marker_bit

	refBitStream.Skip(10); //vbv_buffer_size
	refBitStream.Skip(1); //constrained_parameters_flag

	if(refBitStream.Read(1))
	{
		NOT_IMPLEMENTED_ERROR;
		refBitStream.Skip(8 * 64); //intra_quantizer_matrix
	}

	if(refBitStream.Read(1))
	{
		NOT_IMPLEMENTED_ERROR;
		refBitStream.Skip(8 * 64); //non_intra_quantizer_matrix
	}

	//Recalc some values
	this->mb_width = (this->refStream.width + 15) / 16; //formula from 2.4.3.2
										
	//a new sequence of pictures arrives
	this->ResetFrameCounter();
}

void CMPEG1VideoDecoder::DecodeSlice(uint8 slice_vertical_position, ASeekableInputStream &refInput)
{
	bool skip;
	uint8 quantizerScale; //defined in 2.4.3.5
	uint8 macroblock_address_increment; //defined in 2.4.3.6
	uint16 dct_dc_y_past, dct_dc_cb_past, dct_dc_cr_past, macroblock_address;
	int16 predictiveMotionVectorForward[2]; //recon_right_for_prev, recon_down_for_prev
	int16 predictiveMotionVectorBackward[2];
	int16 motionVectorForward[2];
	int16 motionVectorBackward[2];
	SMacroblockTypeInfo macroblock_type;

	CBitInputStream &refBitStream = CBitInputStream(refInput);

	//Mixing 2.4.2.6 & 2.4.2.7 a bit here

	quantizerScale = (uint8)refBitStream.Read(5);

	while(refBitStream.Read(1)) //extra_bit_slice
	{
		refBitStream.Skip(8); //extra_information_slice
	}

	//2.4.4.1 says: reset predictors at start of a slice
	dct_dc_y_past = 1024;
	dct_dc_cb_past = 1024;
	dct_dc_cr_past = 1024;
	//2.4.4.2 & 2.4.4.3 say: reset previous motion vectors on start of a slice
	predictiveMotionVectorForward[0] = 0;
	predictiveMotionVectorForward[1] = 0;
	predictiveMotionVectorBackward[0] = 0;
	predictiveMotionVectorBackward[1] = 0;

	macroblock_address = (slice_vertical_position - 1) * this->mb_width - 1; //formula from 2.4.3.6
	macroblock_address_increment = this->ReadMacroblockAddressIncrement(refBitStream);
	macroblock_address += macroblock_address_increment; //add horizontal offset

	while(true)
	{
		if(macroblock_address_increment == 0)
		{
			if(refBitStream.Get(23) == 0 || refBitStream.HitEnd())
				break;
			macroblock_address_increment = this->ReadMacroblockAddressIncrement(refBitStream);
		}

		skip = macroblock_address_increment > 1; //according to 2.4.4.4
												 
		//stdOut << "mba " << macroblock_address << ": " << ToHexString(refBitStream.Get(32), 8) << endl;
		
		this->DecodeMacroblock(macroblock_address, skip, macroblock_type, quantizerScale, predictiveMotionVectorForward, predictiveMotionVectorBackward, motionVectorForward, motionVectorBackward, dct_dc_y_past, dct_dc_cb_past, dct_dc_cr_past, refBitStream);

		macroblock_address_increment--;
		macroblock_address++;
	}

	refInput.Rewind(3); //revert the start code!
}

uint8 *CMPEG1VideoDecoder::GetBlockOffset(uint8 blockNumber, CYCbCr420Image &refFrame, uint32 bufOffsetY, uint32 bufOffsetC) const
{
	switch(blockNumber)
	{
	case 0: //first Y' block
		return refFrame.GetLumaChannel() + bufOffsetY;
	case 1: //second Y' block is right to the first
		return refFrame.GetLumaChannel() + bufOffsetY + BLOCK_WIDTH;
	case 2: //third Y' block is under the first
		return refFrame.GetLumaChannel() + bufOffsetY + (BLOCK_HEIGHT * MACROBLOCK_WIDTH * this->mb_width);
	case 3: //third Y' block is right to the third
		return refFrame.GetLumaChannel() + bufOffsetY + (BLOCK_HEIGHT * MACROBLOCK_WIDTH * this->mb_width) + BLOCK_WIDTH;
	case 4: //single Cb block
		return refFrame.GetChromaBlueChannel() + bufOffsetC;
	case 5: //single Cr block
		return refFrame.GetChromaRedChannel() + bufOffsetC;
	}

	//this should never happen
	ASSERT(0);
	return NULL;
}

byte CMPEG1VideoDecoder::GetPel(byte *pPel_past, int16 right_half_for, int16 down_half_for, uint16 lineSize) const
{
	if(!right_half_for && !down_half_for)
		return *pPel_past;

	if(!right_half_for && down_half_for)
		return ((int32)*pPel_past + (int32)*(pPel_past + lineSize)) / 2;

	if(right_half_for && !down_half_for)
		return ((int32)*pPel_past + (int32)*(pPel_past + 1)) / 2;

	//if(right_half_for && down_half_for)
	return ((int32)*pPel_past + (int32)*(pPel_past + lineSize) + (int32)*(pPel_past + 1) + (int32)*(pPel_past + lineSize + 1)) / 4;
}

void CMPEG1VideoDecoder::InverseQuantizeIntraBlock(uint8 quantizerScale, uint16 &refdct_dc_past, int16(&refdct_recon)[BLOCK_HEIGHT][BLOCK_WIDTH], const int16(&refdct_zz)[BLOCK_NUMBEROFPELS]) const
{
	uint8 m, n, i;
	int16 tmp;

	//2.4.4.1

	n = 1;

	for(m = 0; m < BLOCK_HEIGHT; m++)
	{
		for(; n < BLOCK_WIDTH; n++)
		{
			i = scan[m][n];
			tmp = (2 * (refdct_zz[i]) * quantizerScale * this->intra_quantizer_matrix[m][n]) / 16;

			if(!(tmp & 1))
			{
				if(tmp < 0)
					tmp++;
				else if(tmp > 0)
					tmp--;
			}

			refdct_recon[m][n] = CLAMP(tmp, -2048, 2047);
		}

		n = 0;
	}

	//equivalent for all blocks, even if standard describes it another way
	refdct_recon[0][0] = refdct_dc_past + refdct_zz[0] * 8;
	refdct_dc_past = refdct_recon[0][0];
}

void CMPEG1VideoDecoder::InverseQuantizeNonIntraBlock(uint8 quantizerScale, int16(&refdct_recon)[BLOCK_HEIGHT][BLOCK_WIDTH], const int16(&refdct_zz)[BLOCK_NUMBEROFPELS]) const
{
	uint8 m, n, i;
	int16 tmp;

	//2.4.4.2

	for(m = 0; m < BLOCK_HEIGHT; m++)
	{
		for(n = 0; n < BLOCK_WIDTH; n++)
		{
			i = scan[m][n];

			if(refdct_zz[i] == 0)
			{
				refdct_recon[m][n] = 0;
				continue;
			}

			tmp = ((2 * refdct_zz[i] + (refdct_zz[i] > 0 ? 1 : -1)) * quantizerScale * this->non_intra_quantizer_matrix[m][n]) / 16;

			if(!(tmp & 1))
			{
				if(tmp < 0)
					tmp++;
				else if(tmp > 0)
					tmp--;
			}

			refdct_recon[m][n] = CLAMP(tmp, -2048, 2047);
		}
	}
}

//Public methods
void CMPEG1VideoDecoder::Decode(const SPacket &refPacket)
{
	byte nextStartCode;
	uint32 startCode;
	CVideoFrame *pFrame;
	
	CBufferInputStream input(refPacket.pData, refPacket.size);
	
	startCode = UINT32_MAX;

	while(FindNextStartCode(startCode, input))
	{
		nextStartCode = startCode & 0xFF; //get rid of the prefix
		startCode = UINT32_MAX; //reset state

		switch(nextStartCode)
		{
		case MPEG1VIDEO_STARTCODE_PICTURE:
			{
				ASSERT(this->currentState == STATE_EXPECT_PICTURE);
				this->currentState = STATE_EXPECT_SLICE;

				this->DecodePicture(input);
			}
			break;
		case MPEG1VIDEO_STARTCODE_SEQUENCE_HEADER:
			{
				ASSERT(this->currentState == STATE_EXPECT_SEQUENCE || this->currentState == STATE_EXPECT_PICTURE);
				this->currentState = STATE_EXPECT_GROUPOFPICTURES;

				this->DecodeSequenceHeader(input);
			}
			break;
		case MPEG1VIDEO_STARTCODE_GROUPOFPICTURES:
			{
				ASSERT(this->currentState == STATE_EXPECT_GROUPOFPICTURES);
				this->currentState = STATE_EXPECT_PICTURE;

				//Contains no interesting info... simply skip
			}
			break;
		default:
			if(nextStartCode >= MPEG1VIDEO_STARTCODE_SLICE_FIRST && nextStartCode <= MPEG1VIDEO_STARTCODE_SLICE_LAST)
			{
				ASSERT(this->currentState == STATE_EXPECT_SLICE || this->currentState == STATE_DECODE_SLICES);
				this->currentState = STATE_DECODE_SLICES;
				
				this->DecodeSlice(nextStartCode, input);
			}
			else
			{
				ASSERT(0);
			}
		}
	}

	ASSERT(this->currentState == STATE_DECODE_SLICES);
	this->currentState = STATE_EXPECT_PICTURE;

	if(this->currentPicType == PICTURETYPE_B)
	{
		pFrame = new CVideoFrame(this->pCurrentFrame);
	}
	else
	{
		CYCbCr420Image *pClone;
		
		pClone = new CYCbCr420Image(*this->pCurrentFrame);
		pFrame = new CVideoFrame(pClone);
	}
	
	pFrame->pts = refPacket.pts;
	
	this->AddFrame(pFrame, this->temporal_reference);
	
	//TODO: DEBUG
	/*
	CFileOutputStream fos(CPath("C:/Users/Amir/Desktop/1.pbm"));
	fos << "P6" << "\xA" << "320" << "\xA" << "240" << "\xA" << "255\xA";
	CYCbCrToRGBConverter conv;
	pFrame = (CVideoFrame *)conv.Apply(*pFrame);
	CR8G8B8Image *pimg = (CR8G8B8Image *)pFrame->GetImage();
	for(uint32 i = 0; i < pimg->GetNumberOfPixels(); i++)
	{
		fos.WriteByte(pimg->GetRedChannel()[i]);
		fos.WriteByte(pimg->GetGreenChannel()[i]);
		fos.WriteByte(pimg->GetBlueChannel()[i]);
	}

	stdOut << "done";
	*/
}

EPixelFormat CMPEG1VideoDecoder::GetPixelFormat() const
{
	return EPixelFormat::YCbCr420;
}
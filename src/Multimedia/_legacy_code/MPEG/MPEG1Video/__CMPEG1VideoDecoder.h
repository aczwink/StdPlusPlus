//Local
#include "AVideoDecoder.h"
#include "CYCbCr420Frame.h"

namespace SJCMMLib
{
	class CMPEG1VideoDecoder : public AVideoDecoder
	{
	public:
		//Constructor
		CMPEG1VideoDecoder();
		//Functions
		void Decode(const SPacket &refPacket);
		EPixelFormat GetFramePixelFormat() const;
		void Release();
	};
}

/*
	private:
		CYCbCr420Frame *pFirstPredictionFrame;
		CYCbCr420Frame *pSecondPredictionFrame;
		CYCbCr420Frame *pCurrentFrame;
		//Functions
		void DecodeBlock(uint8 blockNumber, uint8 coded_block_pattern, bool intra, uint16 &refdct_dc_past, int16 *pdct_recon, SJCLib::CBitInputStream &refBitInput);
		void DecodeExtensionAndUserData(SJCLib::CBufferInputStream &refInput);
		void DecodeMacroblock(uint8 slice_vertical_position, uint16 macroblock_address, SMacroblockTypeInfo &refmacroblock_type, bool isSkipped, SJCLib::CBitInputStream &refBitInput);
		void InverseQuantizeIntraBlock(uint8 blockNumber, int16 *pdct_zz, int16 *pdct_recon, uint8 (&refQuantizerMatrix)[BLOCK_WIDTH][BLOCK_HEIGHT]);
		void InverseQuantizeNonIntraBlock(uint8 blockNumber, int16 *pdct_zz, int16 *pdct_recon, uint8 (&refQuantizerMatrix)[BLOCK_WIDTH][BLOCK_HEIGHT]);
}*/
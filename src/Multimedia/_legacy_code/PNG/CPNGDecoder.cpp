//Class header
#include "CPNGDecoder.h"

/*
source:
https://www.w3.org/TR/PNG/
*/

//Constructor
CPNGDecoder::CPNGDecoder(AStream &refStream) : AVideoDecoder(refStream)
{
	this->pPreviousRow = nullptr;
	this->pCurrentRow = nullptr;

	this->colorType = -1;
}

//Destructor
CPNGDecoder::~CPNGDecoder()
{
	if(this->pPreviousRow)
		MemFree(this->pPreviousRow);
	if(this->pCurrentRow)
		MemFree(this->pCurrentRow);
}

//Private methods
void CPNGDecoder::DecodeData(AInputStream &refInput)
{
	byte *pTmp, nComponents;
	byte *pR, *pG, *pB, *pA;
	byte filterType;
	uint32 rowSize;
	ADecompressor *pInflater;
	
	//
	pR = nullptr;
	pG = nullptr;
	pB = nullptr;
	pA = nullptr;
	switch(this->colorType)
	{
	case 2: //truecolor
		{
			CR8G8B8Image *const& refpRGBImage = (CR8G8B8Image *)this->pImage;
			
			pR = refpRGBImage->GetRedChannel();
			pG = refpRGBImage->GetGreenChannel();
			pB = refpRGBImage->GetBlueChannel();
			nComponents = 3;
		}
		break;
	case 6: //truecolor with alpha
		{
			CR8G8B8A8Image *const& refpRGBAImage = (CR8G8B8A8Image *)this->pImage;
			
			pR = refpRGBAImage->GetRedChannel();
			pG = refpRGBAImage->GetGreenChannel();
			pB = refpRGBAImage->GetBlueChannel();
			pA = refpRGBAImage->GetAlphaChannel();
			nComponents = 4;
		}
		break;
	default:
		ASSERT(0);
	}
	
	rowSize = this->GetRowSize();
	pInflater = ADecompressor::Create(ECompressionAlgorithm::ZLIB, refInput);
	
	//Figure 4.9: for each row there is a filter type byte and then a scanline
	for(uint16 y = 0; y < this->refStream.height; y++)
	{
		pInflater->Decompress(CBufferOutputStream(&filterType, 1), 1);
		pInflater->Decompress(CBufferOutputStream(this->pCurrentRow, rowSize), rowSize);
		
		for(uint16 x = 0; x < rowSize; x += nComponents)
		{
			this->DeFilter(filterType, x, nComponents);

			byte *const& refpFiltered = &this->pCurrentRow[x];
			
			*pR++ = refpFiltered[0];
			*pG++ = refpFiltered[1];
			*pB++ = refpFiltered[2];
			if(nComponents == 4)
				*pA++ = refpFiltered[3];
		}
		
		//swap row pointers
		pTmp = this->pPreviousRow;
		this->pPreviousRow = this->pCurrentRow;
		this->pCurrentRow = pTmp;
	}
	
	//clean up
	delete pInflater;
}

void CPNGDecoder::DeFilter(byte filterType, uint16 x, uint8 nComponents)
{
	byte left[4], upper[4], upperLeft[4];
	
	byte *const& refpData = &this->pCurrentRow[x];
	
	//9.2 Filter types for filter method 0
	
	*(uint32 *)upper = *(uint32 *)&this->pPreviousRow[x];
	if(x == 0)
	{
		*(uint32 *)left = 0;
		*(uint32 *)upperLeft = 0;
	}
	else
	{
		*(uint32 *)left = *(uint32 *)&this->pCurrentRow[x - nComponents];
		*(uint32 *)upperLeft = *(uint32 *)&this->pPreviousRow[x - nComponents];
	}
	
	switch(filterType)
	{
	case 0:
		{
			for(uint8 i = 0; i < nComponents; i++)
				refpData[i] = refpData[i];
		}
		break;
	case 1:
		{
			for(uint8 i = 0; i < nComponents; i++)
				refpData[i] = refpData[i] + left[i];
		}
		break;
	case 2:
		{
			for(uint8 i = 0; i < nComponents; i++)
				refpData[i] = refpData[i] + upper[i];
		}
		break;
	case 3:
		{
			for(uint8 i = 0; i < nComponents; i++)
				refpData[i] = refpData[i] + (left[i] + upper[i]) / 2;
		}
		break;
	case 4:
		{
			for(uint8 i = 0; i < nComponents; i++)
				refpData[i] = refpData[i] + PaethPredictor(left[i], upper[i], upperLeft[i]);
		}
		break;
	default:
		ASSERT(0);
	}
}

uint32 CPNGDecoder::GetRowSize() const
{
	//Table 11.1
	
	switch(this->colorType)
	{
	case 2: //truecolor
		{
			if(this->bitDepth == 8)
				return 3 * this->refStream.width;

			return 3 * 2 * this->refStream.width;
		}
		break;
	case 6: //truecolor with alpha
		{
			if(this->bitDepth == 8)
				return 4 * this->refStream.width;
			
			return 4 * 2 * this->refStream.width;
		}
		break;
	}

	ASSERT(0);
	return 0;
}

void CPNGDecoder::ReadImageHeader(AInputStream &refInput)
{
	uint8 compressionMethod, filterMethod, interlaceMethod;
	uint32 width, height;
	
	//11.2.2

	width = refInput.ReadUInt32BE();
	height = refInput.ReadUInt32BE();
	this->bitDepth = refInput.ReadByte();
	this->colorType = refInput.ReadByte();
	compressionMethod = refInput.ReadByte();
	filterMethod = refInput.ReadByte();
	interlaceMethod = refInput.ReadByte();
	
	ASSERT(compressionMethod == 0);
	ASSERT(filterMethod == 0);
	ASSERT(interlaceMethod == 0);
	
	this->refStream.height = (uint16)height;
	this->refStream.width = (uint16)width;
	
	if(!this->pCurrentRow)
	{
		this->pPreviousRow = (byte *)MemAlloc(this->GetRowSize());
		this->pCurrentRow = (byte *)MemAlloc(this->GetRowSize());

		MemZero(this->pPreviousRow, this->GetRowSize());
	}

	switch(this->colorType)
	{
	case 2: //truecolor
		this->pImage = new CR8G8B8Image(this->refStream.width, this->refStream.height);
		break;
	case 6: //truecolor with alpha
		this->pImage = new CR8G8B8A8Image(this->refStream.width, this->refStream.height);
		break;
	default:
		ASSERT(0);
	}
}

//Public methods
void CPNGDecoder::Decode(const SPacket &refPacket)
{
	uint32 chunkSize, chunkType;
	CFIFOBuffer frameDataBuffer;
	
	CBufferInputStream input(refPacket.pData, refPacket.size);
	
	input.Skip(8); //skip header
	
	//read chunks as of 5.3
	while(true)
	{
		chunkSize = input.ReadUInt32BE();
		chunkType = input.ReadUInt32BE();

		if(input.HitEnd())
			break;

		switch(chunkType)
		{
		case 0x49444154: //IDAT
			input.FlushTo(frameDataBuffer, chunkSize);
			break;
		case 0x49484452: //IHDR
			this->ReadImageHeader(input);
			break;
		case 0x6348524D: //cHRM
		case 0x69434350: //iCCP
		case 0x49454E44: //IEND
		case 0x70485973: //pHYs
		case 0x73424954: //sBIT
		case 0x74455874: //tEXt
			input.Skip(chunkSize); //just ignore
			break;
		default:
			ASSERT(0);
		}
		
		input.Skip(4); //CRC
	}
	
	this->DecodeData(frameDataBuffer);

	this->AddFrame(new CVideoFrame(this->pImage));
}

EPixelFormat CPNGDecoder::GetPixelFormat() const
{
	switch(this->colorType)
	{
	case 2: //truecolor
		{
			ASSERT(this->bitDepth == 8);

			return EPixelFormat::R8G8B8;
		}
		break;
	case 6: //truecolor with alpha
		{
			ASSERT(this->bitDepth == 8);

			return EPixelFormat::R8G8B8A8;
		}
		break;
	}

	return EPixelFormat::Unknown;
}
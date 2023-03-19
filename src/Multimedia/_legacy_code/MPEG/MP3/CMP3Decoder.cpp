//Class header
#include "CMP3Decoder.h"
//Definitions
#define SQRT_2 1.414213562373095

//Constants
const uint8 cb_limit = 21; //2.4.2.7 'cb_limit'
const uint8 cblimit = cb_limit; //sometimes written like this...

//Constructor
CMP3Decoder::CMP3Decoder(AStream &refStream) : AAudioDecoder(refStream)
{
	this->Reset();
}

//Private methods
void CMP3Decoder::AntiAlias(float64 (&refxr)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], const SGranuleInfo &refGranuleInfo)
{
	uint8 subBand, subBandLimit, subBandSample;
	float64 a, b;

	/*
	Again badly described-.-
	3-A.5:
	-All 576 frequency lines are divided into parts of 18 lines per part
	-Last one of a part is exchanged with first one of next part
	-last one but one of a part is exchanged with second one of next part
	...
	-8 butterflies between all pairs
	3-A.6:
	-Describes what operation to perform during a butterfly exchange
	ca and cs tables: 3-B.9
	*/

	static const float64 ca[8] = {-0.51449575542752657, -0.47173196856497235, -0.31337745420390184, -0.18191319961098118, -0.094574192526420658, -0.040965582885304053, -0.014198568572471150, -0.0036999746737600373};
	static const float64 cs[8] = {0.85749292571254432, 0.88174199731770520, 0.94962864910273281, 0.98331459249179021, 0.99551781606758583, 0.99916055817814753, 0.99989919524444715, 0.99999315507028030};

	if(refGranuleInfo.window_switching_flag && refGranuleInfo.block_type == 2)
	{
		if(refGranuleInfo.mixed_block_flag)
		{
			subBandLimit = 1;
		}
		else
		{
			return;
		}
	}
	else
	{
		subBandLimit = MP3_SUBBANDLIMIT - 1;
	}

	for(subBand = 0; subBand < subBandLimit; subBand++)
	{
		for(subBandSample = 0; subBandSample < 8; subBandSample++)
		{
			a = refxr[subBand][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES - 1 - subBandSample];
			b = refxr[subBand + 1][subBandSample];

			refxr[subBand][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES - 1 - subBandSample] = (a * cs[subBandSample]) - (b * ca[subBandSample]);
			refxr[subBand + 1][subBandSample] = (b * cs[subBandSample]) + (a * ca[subBandSample]);
		}
	}
}

void CMP3Decoder::ApplyFrequencyInversion(float64(&refValues)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES])
{
	uint8 subBand, subBandSample;

	/*
	2.4.3.4 'Synthesis Filterbank'
	Quote: "For every second subband of the polyphase filterbank every second input value is multiplied by -1 to correct for the frequency inversion of the polyphase filterbank"
	*/

	for(subBand = 0; subBand < MP3_SUBBANDLIMIT; subBand++)
	{
		for(subBandSample = 0; subBandSample < MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES; subBandSample++)
		{
			if((subBand & 1) && (subBandSample & 1))
				refValues[subBand][subBandSample] = -refValues[subBand][subBandSample];
		}
	}
}

void CMP3Decoder::ApplyHybridFilter(float64(&refOutput)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], const float64(&refInput)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], float64(&refPreviousBlocks)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], const SGranuleInfo &refGranuleInfo)
{
	uint8 subBand, subBandSample, blockType;
	float64 dctOut[36]; //mdct produces 36 samples

	for(subBand = 0; subBand < MP3_SUBBANDLIMIT; subBand++)
	{
		blockType = refGranuleInfo.window_switching_flag && refGranuleInfo.mixed_block_flag && subBand < 2 ? 0 : refGranuleInfo.block_type;
		if(blockType == 2)
		{
			this->InverseMDCTShort(refInput[subBand], dctOut);
		}
		else
		{
			this->InverseMDCT(refGranuleInfo.block_type, refInput[subBand], dctOut);
		}

		//overlap addition
		for(subBandSample = 0; subBandSample < MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES; subBandSample++)
		{
			refOutput[subBand][subBandSample] = dctOut[subBandSample] + refPreviousBlocks[subBand][subBandSample];
			refPreviousBlocks[subBand][subBandSample] = dctOut[subBandSample + MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES];
		}
	}
}

void CMP3Decoder::DecodeHuffmanBits(int16(&refDecodedValues)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], uint32 part2_start, const SGranuleInfo &refGranuleInfo, CBitInputStream &refBitStream)
{
	int8 v, w;
	int16 x, y;
	uint16 region1start, region2start, i;
	const SHuffmanTable *pTable;

	/*
	Some important things which are very hard to find in the standard.
	2.4.2.7 'big_values' describes that the full spectrum is divided into 3 regions
	first is the 'big values' part
	second the 'count1' part, quadruples coded
	third the 'rzero' part, all values 0

	Info for 'big values' part
	2.4.2.7 'region_address1':
	-Quote: "It is a subdivision of the region which is described by big_values."
	-Quote: "The values region_address[1,2] are used to point to the boundaries of the regions. The region boundaries are aligned with the partitioning of the spectrum into critical bands."
	(3-B.8 is meant here)
	-Quote: "Three regions are used."
	2.4.2.7 'Huffman_code_bits':
	Quote: "Always pairs of values (x,y) are coded."

	Info for 'count1' part:
	2.4.3.4 'Huffman decoding':
	Quote: "Decoding is done until all Huffmancodebits have been decoded or until quantized values representing 576 frequency lines have been decoded, whichever comes first. If there are more Huffmancodebits than necessary to decode 576 values they are regarded as stuffing bits and discarded."
	*/

	const SScaleFactorTable &refScaleFactorTable = this->GetScalefactorBandTable();

	if(refGranuleInfo.window_switching_flag && refGranuleInfo.block_type == 2)
	{
		/*
		2.4.2.7 'blocksplit_flag' and 'block_type'
		-region_adress1 = 9
		-for each scalefactor table[9 / 3] = 12
		-3 short windows!
		-there is no region 2
		*/
		region1start = 36;
		region2start = 576; //no region2 means, region2 starts at the end of the frequency lines
	}
	else
	{
		/*
		MPEG-2 describes this better than MPEG-1:
		MPEG-2 2.5.2.13 'region0_count' and 'region1_count'
		*/
		region1start = refScaleFactorTable.l[refGranuleInfo.region0_count + 1];
		region2start = refScaleFactorTable.l[refGranuleInfo.region0_count + 1 + refGranuleInfo.region1_count + 1];
	}

	//Read the big_values area... (which has 3 subregions)
	for(i = 0; i < (refGranuleInfo.big_values * 2) && (refBitStream.GetNumberOfReadBits() < part2_start + refGranuleInfo.part2_3_length);)
	{
		if(i < region1start)
		{
			pTable = &this->GetHuffmanTable(refGranuleInfo.table_select[0]);
		}
		else if(i < region2start)
		{
			pTable = &this->GetHuffmanTable(refGranuleInfo.table_select[1]);
		}
		else
		{
			pTable = &this->GetHuffmanTable(refGranuleInfo.table_select[2]);
		}

		this->HuffmanDecode(x, y, *pTable, refBitStream);
		//read values consecutively
		refDecodedValues[i / MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES][i % MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES] = x;
		i++;
		refDecodedValues[i / MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES][i % MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES] = y;
		i++;
	}

	//Read count1 area
	while((refBitStream.GetNumberOfReadBits() < part2_start + refGranuleInfo.part2_3_length) && (i < MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES * MP3_SUBBANDLIMIT))
	{
		this->HuffmanDecodeQuadruple(v, w, x, y, refGranuleInfo.count1table_select, refBitStream);

		refDecodedValues[i / MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES][i % MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES] = v;
		i++;
		refDecodedValues[i / MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES][i % MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES] = w;
		i++;

		if(i >= MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES * MP3_SUBBANDLIMIT) //at this point we might get a buffer overflow
			break;

		refDecodedValues[i / MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES][i % MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES] = x;
		i++;
		refDecodedValues[i / MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES][i % MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES] = y;
		i++;
	}

	if(refBitStream.GetNumberOfReadBits() < part2_start + refGranuleInfo.part2_3_length)
	{
		//We have already decoded quantized values representing 576 frequency lines... rest are stuffing bits
		refBitStream.Skip(part2_start + refGranuleInfo.part2_3_length - refBitStream.GetNumberOfReadBits());
	}

	//'rzero' part... all values are simply 0
	for(; i < MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES * MP3_SUBBANDLIMIT; i++)
	{
		refDecodedValues[i / MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES][i % MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES] = 0;
	}
}

void CMP3Decoder::DecodeMSStereo(float64(&reflr)[2][MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES])
{
	uint8 subband, subbandSample;
	float64 left, right;

	/*
	2.4.3.4 'MS_stereo mode'

	input in reflr is Mi (in left channel), Si (in right channel)
	*/

	for(subband = 0; subband < MP3_SUBBANDLIMIT; subband++)
	{
		for(subbandSample = 0; subbandSample < MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES; subbandSample++)
		{
			left = (reflr[0][subband][subbandSample] + reflr[1][subband][subbandSample]) / SQRT_2;
			right = (reflr[0][subband][subbandSample] - reflr[1][subband][subbandSample]) / SQRT_2;

			reflr[0][subband][subbandSample] = left;
			reflr[1][subband][subbandSample] = right;
		}
	}
}

void CMP3Decoder::DecodeScalefactors(uint8 gr, uint8 ch, UScaleFactors(&refScaleFactors)[2], const SSideInfo &refSideInfo, CBitInputStream &refBitStream)
{
	uint8 scfsi_band, cb, window;

	const SChannelInfo &refChannelInfo = refSideInfo.ch[ch];
	const SGranuleInfo &refGranuleInfo = refSideInfo.ch[ch].gr[gr];

	//2.4.1.7

	static const uint8 slen[2][16] = //2.4.2.7 'scalefac_compress'
	{
		{0, 0, 0, 0, 3, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4}, //slen1
		{0, 1, 2, 3, 0, 1, 2, 3, 1, 2, 3, 1, 2, 3, 2, 3} //slen2
	};

	scfsi_band = 0;

	if(refGranuleInfo.window_switching_flag && refGranuleInfo.block_type == 2)
	{
		if(refGranuleInfo.mixed_block_flag)
		{
			ASSERT(0);
		}
		else
		{
			//short blocks

			//2.4.2.7 'scalefac_compress'
			for(cb = 0; cb < 12; cb++) //2.4.2.7 'cb_limit_short'
			{
				for(window = 0; window < 3; window++)
				{
					refScaleFactors[ch].s[window][cb] = (uint8)refBitStream.Read(slen[scfsi_band][refGranuleInfo.scalefac_compress]);
				}

				if(cb == 5)
					scfsi_band++;
			}
			refScaleFactors[ch].s[0][12] = 0;
			refScaleFactors[ch].s[1][12] = 0;
			refScaleFactors[ch].s[2][12] = 0;
		}
	}
	else
	{
		//long block (block_type != 2) //2.4.2.7 'cb_limit'
		for(cb = 0; cb < cblimit; cb++)
		{
			switch(cb) //2.4.2.7 'scfsi_band'
			{
			case 6:
			case 11:
			case 16:
				scfsi_band++;
			}

			if(gr == 0 || !refChannelInfo.scfsi[scfsi_band]) //2.4.1.7 is incorrect here!!!
			{
				refScaleFactors[ch].l[cb] = (uint8)refBitStream.Read(slen[(cb < 11 ? 0 : 1)][refGranuleInfo.scalefac_compress]);
			}
		}
		refScaleFactors[ch].l[22] = 0;
	}
}

void CMP3Decoder::DecodeScalefactorsMPEG2(uint8 gr, uint8 ch, UScaleFactors(&refScaleFactors)[2], SGranuleInfo &refGranuleInfo, const SMPEGAudioFrameHeader &refFrameHeader, CBitInputStream &refBitStream)
{
	uint8 blockTypeNumber, i, j, k, window;
	const uint8 *pNrOfSfb;
	uint8 slen[4];
	uint16 int_scalefac_comp;
	uint32 scalefac_buffer[54];

	//2.4.3.2 (MPEG-2) 'Scalefactors'

	pNrOfSfb = nullptr;


	//clear scalefactor buffer
	for(i = 0; i < 45; i++)
	{
		scalefac_buffer[i] = 0;
		this->illegalIntensityPositionBuffer[i] = 0;
	}

	blockTypeNumber = 0;
	if(refGranuleInfo.block_type == 2)
		blockTypeNumber = 1 + refGranuleInfo.mixed_block_flag;

	if(!(refFrameHeader.isIntensityStereo && !refFrameHeader.isMS_Stereo && ch == 1))
	{
		if(refGranuleInfo.scalefac_compress < 400)
		{
			static const uint8 nr_of_sfb[3][4] = {{6, 5, 5, 5},{9, 9, 9, 9},{6, 9, 9, 9}};

			slen[0] = (refGranuleInfo.scalefac_compress >> 4) / 5;
			slen[1] = (refGranuleInfo.scalefac_compress >> 4) % 5;
			slen[2] = (refGranuleInfo.scalefac_compress % 16) >> 2;
			slen[3] = (refGranuleInfo.scalefac_compress % 4);
			refGranuleInfo.preflag = false;

			pNrOfSfb = nr_of_sfb[blockTypeNumber];
		}
		else if(refGranuleInfo.scalefac_compress < 500)
		{
			static const uint8 nr_of_sfb[3][4] = {{6, 5, 7, 3},{9, 9, 12, 6},{6, 9, 12, 6}};

			slen[0] = ((refGranuleInfo.scalefac_compress - 400) >> 2) / 5;
			slen[1] = ((refGranuleInfo.scalefac_compress - 400) >> 2) % 5;
			slen[2] = (refGranuleInfo.scalefac_compress - 400) % 4;
			slen[3] = 0;
			refGranuleInfo.preflag = false;

			pNrOfSfb = nr_of_sfb[blockTypeNumber];
		}
		else if(refGranuleInfo.scalefac_compress < 512)
		{
			static const uint8 nr_of_sfb[3][4] = {{11, 10, 0, 0},{18, 18, 0, 0},{15, 18, 0, 0}};

			slen[0] = (refGranuleInfo.scalefac_compress - 500) / 3;
			slen[1] = (refGranuleInfo.scalefac_compress - 500) % 3;
			slen[2] = 0;
			slen[3] = 0;
			refGranuleInfo.preflag = true;

			pNrOfSfb = nr_of_sfb[blockTypeNumber];
		}
	}

	if(refFrameHeader.isIntensityStereo && !refFrameHeader.isMS_Stereo && ch == 1)
	{
		//intensity_scale = scalefac_comp %2;
		int_scalefac_comp = refGranuleInfo.scalefac_compress >> 1;

		if(int_scalefac_comp < 180)
		{
			static const uint8 nr_of_sfb[3][4] = {{7, 7, 7, 0},{12, 12, 12, 0},{6, 15, 12, 0}};

			slen[0] = int_scalefac_comp / 36;
			slen[1] = (int_scalefac_comp % 36) / 6;
			slen[2] = (int_scalefac_comp % 36) % 6;
			slen[3] = 0;
			refGranuleInfo.preflag = false;

			pNrOfSfb = nr_of_sfb[blockTypeNumber];
		}
		else if(int_scalefac_comp < 244)
		{
			static const uint8 nr_of_sfb[3][4] = {{6, 6, 6, 3},{12, 9, 9, 6},{6, 12, 9, 6}};

			slen[0] = ((int_scalefac_comp - 180) % 64) >> 4;
			slen[1] = ((int_scalefac_comp - 180) % 16) >> 2;
			slen[2] = (int_scalefac_comp - 180) % 4;
			slen[3] = 0;
			refGranuleInfo.preflag = false;

			pNrOfSfb = nr_of_sfb[blockTypeNumber];
		}
		else if(int_scalefac_comp < 255)
		{
			static const uint8 nr_of_sfb[3][4] = {{8, 8, 5, 0},{15, 12, 9, 0},{6, 18, 9, 0}};

			slen[0] = (int_scalefac_comp - 244) / 3;
			slen[1] = (int_scalefac_comp - 244) % 3;
			slen[2] = 0;
			slen[3] = 0;
			refGranuleInfo.preflag = false;

			pNrOfSfb = nr_of_sfb[blockTypeNumber];
		}
	}

	k = 0;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < pNrOfSfb[i]; j++)
		{
			if(slen[i] == 0)
			{
				scalefac_buffer[k] = 0;
				this->illegalIntensityPositionBuffer[k] = 0;
			}
			else
			{
				ASSERT(slen[i] <= 16);

				scalefac_buffer[k] = (uint32)refBitStream.Read(slen[i]);
				this->illegalIntensityPositionBuffer[k] = (uint16)pow(2, slen[i]) - 1; //MPEG-2 2.4.3.2 Quote: "If intensity stereo is selected, the maximum value for intensity position will indicate an illegal intensity position"
			}
			k++;
		}
	}

	if(refGranuleInfo.block_type == 2 && refGranuleInfo.window_switching_flag)
	{
		if(refGranuleInfo.mixed_block_flag)
		{
			ASSERT(0);
		}
		else
		{
			//short blocks
			for(i = 0; i < 12; i++)
			{
				for(window = 0; window < 3; window++)
				{
					refScaleFactors[ch].s[window][i] = scalefac_buffer[i * 3 + window];
				}
			}
			refScaleFactors[ch].s[0][12] = 0;
			refScaleFactors[ch].s[1][12] = 0;
			refScaleFactors[ch].s[2][12] = 0;
		}
	}
	else
	{
		//a maxmimum of 21 scalefactors are transmitted (sum over nr_of_sfb for long blocks)
		for(i = 0; i < 21; i++)
		{
			refScaleFactors[ch].l[i] = scalefac_buffer[i];
		}

		refScaleFactors[ch].l[21] = 0; //last scalefactor is never transmitted
		refScaleFactors[ch].l[22] = 0;
	}
}

void CMP3Decoder::DecodeSideInformation(SSideInfo &refSideInfo, const SMPEGAudioFrameHeader &refHeaderInfo, AInputStream &refInput)
{
	uint8 ch, scfsi_band, gr, region, window, nGranules;

	CBitInputStream &refBitStream = CBitInputStream(refInput);

	//2.4.1.7

	MemZero(&refSideInfo, sizeof(refSideInfo));

	if(refHeaderInfo.isMpeg1)
	{
		nGranules = 2;

		refSideInfo.main_data_begin = (uint16)refBitStream.Read(9);

		if(this->refStream.nChannels == 1)
			refBitStream.Skip(5); //private_bits
		else
			refBitStream.Skip(3); //private_bits

		for(ch = 0; ch < this->refStream.nChannels; ch++)
		{
			for(scfsi_band = 0; scfsi_band < 4; scfsi_band++)
			{
				refSideInfo.ch[ch].scfsi[scfsi_band] = refBitStream.Read(1) & 1;
			}
		}
	}
	else
	{
		//2.4.1.2 MPEG-2
		nGranules = 1;

		refSideInfo.main_data_begin = (uint16)refBitStream.Read(8);
		refBitStream.Skip(this->refStream.nChannels); //private_bits
	}


	for(gr = 0; gr < nGranules; gr++)
	{
		for(ch = 0; ch < this->refStream.nChannels; ch++)
		{
			refSideInfo.ch[ch].gr[gr].part2_3_length = (uint16)refBitStream.Read(12);
			refSideInfo.ch[ch].gr[gr].big_values = (uint16)refBitStream.Read(9);
			refSideInfo.ch[ch].gr[gr].global_gain = (uint8)refBitStream.Read(8);

			if(refHeaderInfo.isMpeg1)
				refSideInfo.ch[ch].gr[gr].scalefac_compress = (uint16)refBitStream.Read(4);
			else
				refSideInfo.ch[ch].gr[gr].scalefac_compress = (uint16)refBitStream.Read(9);

			refSideInfo.ch[ch].gr[gr].window_switching_flag = refBitStream.Read(1) != 0;

			if(refSideInfo.ch[ch].gr[gr].window_switching_flag)
			{
				refSideInfo.ch[ch].gr[gr].block_type = (uint8)refBitStream.Read(2);
				refSideInfo.ch[ch].gr[gr].mixed_block_flag = refBitStream.Read(1) != 0;

				for(region = 0; region < 2; region++)
				{
					refSideInfo.ch[ch].gr[gr].table_select[region] = (uint8)refBitStream.Read(5);
				}
				for(window = 0; window < 3; window++)
				{
					refSideInfo.ch[ch].gr[gr].subblock_gain[window] = (uint8)refBitStream.Read(3);
				}

				/*
				2.5.2.11 'block_type[gr][ch]' (MPEG-2)

				Set region count values, block_type must be != 0 here
				*/
				if(refSideInfo.ch[ch].gr[gr].block_type == 2 && !refSideInfo.ch[ch].gr[gr].mixed_block_flag)
				{
					refSideInfo.ch[ch].gr[gr].region0_count = 8;
				}
				else
				{
					refSideInfo.ch[ch].gr[gr].region0_count = 7;
				}
				//region 1 is rest
				refSideInfo.ch[ch].gr[gr].region1_count = 20 - refSideInfo.ch[ch].gr[gr].region0_count;
			}
			else
			{
				for(region = 0; region < 3; region++)
				{
					refSideInfo.ch[ch].gr[gr].table_select[region] = (uint8)refBitStream.Read(5);
				}
				//from MPEG-2, as it is way better described than MPEG-1
				refSideInfo.ch[ch].gr[gr].region0_count = (uint8)refBitStream.Read(4);
				refSideInfo.ch[ch].gr[gr].region1_count = (uint8)refBitStream.Read(3);
			}

			if(refHeaderInfo.isMpeg1)
				refSideInfo.ch[ch].gr[gr].preflag = refBitStream.Read(1) != 0;

			refSideInfo.ch[ch].gr[gr].scalefac_scale = refBitStream.Read(1) != 0;
			refSideInfo.ch[ch].gr[gr].count1table_select = (uint8)refBitStream.Read(1);

			refSideInfo.main_data_length += refSideInfo.ch[ch].gr[gr].part2_3_length;
		}
	}
}

void CMP3Decoder::DequantizeSamples(double(&refxr)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], const int16(&refis)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], const UScaleFactors &refScaleFactors, const SGranuleInfo &refGranuleInfo)
{
	bool isNegative;
	uint8 subBand, subBandSample, window, cb, subblock_index;
	int16 scaleFactor;
	uint16 next_cb_boundary, cb_width, cb_begin;

	const SScaleFactorTable &refScaleFactorTable = this->GetScalefactorBandTable();

	/*
	2.4.3.4 'Formula for requantization and all scaling'
	Formulas from ISO_IEC_TR_11172-5_1998_Software_Simulation as standard is fucking ununderstandable-.-
	*/

	window = 0;
	cb = 0;

	//Unbelievably hard to find in the standard... 'pretab' is the 3-B.6 table
	static const uint8 preEmphasisTable[21] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 3, 3, 3, 2};

	if(refGranuleInfo.window_switching_flag && refGranuleInfo.block_type == 2)
	{
		if(refGranuleInfo.mixed_block_flag)
		{
			ASSERT(0);
		}
		else
		{
			next_cb_boundary = refScaleFactorTable.s[1] * 3; //we have 3 windows for a short block
			cb_width = refScaleFactorTable.s[1];
			cb_begin = 0;
		}
	}
	else
	{
		//long blocks
		next_cb_boundary = refScaleFactorTable.l[1];
	}

	for(subBand = 0; subBand < MP3_SUBBANDLIMIT; subBand++)
	{
		for(subBandSample = 0; subBandSample < MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES; subBandSample++)
		{
			if(subBand * MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES + subBandSample == next_cb_boundary)
			{
				cb++;

				if(refGranuleInfo.window_switching_flag && refGranuleInfo.block_type == 2)
				{
					next_cb_boundary = refScaleFactorTable.s[cb + 1] * 3; //we have 3 windows for a short block
					cb_width = refScaleFactorTable.s[cb + 1] - refScaleFactorTable.s[cb];
					cb_begin = refScaleFactorTable.s[cb] * 3;
				}
				else
				{
					//long blocks
					next_cb_boundary = refScaleFactorTable.l[cb + 1];
				}
			}

			//global scaling
			refxr[subBand][subBandSample] = pow(2.0, 0.25 * (refGranuleInfo.global_gain - 210.0));

			if(refGranuleInfo.window_switching_flag && refGranuleInfo.block_type == 2 && (!refGranuleInfo.mixed_block_flag || (refGranuleInfo.mixed_block_flag && subBand >= 2)))
			{
				subblock_index = (subBand * MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES + subBandSample - cb_begin) / cb_width;
				ASSERT(IN_RANGE(subblock_index, 0, 2));
				refxr[subBand][subBandSample] *= pow(2.0, 0.25 * ((-8.0 * refGranuleInfo.subblock_gain[subblock_index]) + (-2.0 * (1.0 + refGranuleInfo.scalefac_scale) * refScaleFactors.s[subblock_index][cb])));
			}
			else
			{
				//long block types
				refxr[subBand][subBandSample] *= pow(2.0, -0.5 * (1.0 + refGranuleInfo.scalefac_scale) * (refScaleFactors.l[cb] + refGranuleInfo.preflag * preEmphasisTable[cb]));
			}

			//std libraries pow can't handly -base^exponent, where exponent is not an integer
			scaleFactor = refis[subBand][subBandSample];
			isNegative = scaleFactor < 0;
			if(isNegative)
				scaleFactor = -scaleFactor;
			refxr[subBand][subBandSample] *= pow((double)scaleFactor, (double)(4.0 / 3.0));
			if(isNegative)
				refxr[subBand][subBandSample] = -refxr[subBand][subBandSample];
		}
	}
}

int16 CMP3Decoder::FindLastNonZeroEntryShortBlocks(uint8 window, float64(&refxr)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES])
{
	int8 scaleFactorBand;
	uint8 scaleFactorBandWidth;
	uint16 i;

	const SScaleFactorTable &refScaleFactorTable = this->GetScalefactorBandTable();

	for(scaleFactorBand = 12; scaleFactorBand >= 0; scaleFactorBand--)
	{
		scaleFactorBandWidth = refScaleFactorTable.s[scaleFactorBand + 1] - refScaleFactorTable.s[scaleFactorBand];
		i = 3 * refScaleFactorTable.s[scaleFactorBand] + (window + 1) * scaleFactorBandWidth - 1;

		while(scaleFactorBandWidth)
		{
			if(refxr[i / 18][i % 18] != 0.0)
				return scaleFactorBand + 1;

			scaleFactorBandWidth--;
			i--;
		}
	}

	ASSERT(0);
	return -1;
}

const SScaleFactorTable &CMP3Decoder::GetScalefactorBandTable() const
{
	/*
	3-B.8, only 'index of start' column
	additionally always one value is appended..
	'index of start' of last entry in 3-B.8 Table + 'width of band' of last entry in 3-B.8 Table

	for MPEG-2: Table B.2
	*/

	switch(this->refStream.sampleRate)
	{
	case 22050: //MPEG-2
		{
			static const SScaleFactorTable tableB =
			{
				//long blocks
				{0, 6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96, 116, 140, 168, 200, 238, 284, 336, 396, 464, 522, 576},
				//short blocks
				{0, 4, 8, 12, 18, 24, 32, 42, 56, 74, 100, 132, 174, 192}
			};

			return tableB;
		}
		break;
	case 44100:
		{
			static const SScaleFactorTable tableB =
			{
				//long blocks
				{0, 4, 8, 12, 16, 20, 24, 30, 36, 44, 52, 62, 74, 90, 110, 134, 162, 196, 238, 288, 342, 418, 576},
				//short blocks
				{0, 4, 8, 12, 16, 22, 30, 40, 52, 66, 84, 106, 136, 192},
			};

			return tableB;
		}
		break;
	case 48000:
		{
			static const SScaleFactorTable tableC =
			{
				//long blocks
				{0, 4, 8, 12, 16, 20, 24, 30, 36, 42, 50, 60, 72, 88, 106, 128, 156, 190, 230, 276, 330, 384, 576},
				//short blocks
				{0, 4, 8, 12, 16, 22, 28, 38, 50, 64, 80, 100, 126, 192},
			};

			return tableC;
		}
		break;
	}

	ASSERT(0);
	return SScaleFactorTable();
}

void CMP3Decoder::HuffmanDecode(int16 &refX, int16 &refY, const SHuffmanTable &refTable, CBitInputStream &refBitStream)
{
	uint16 i;

	for(i = 0; i < refTable.nEntries; i++)
	{
		//Works only if tables are sorted by hlen!
		if(refBitStream.Get(refTable.pEntries[i].hlen) == refTable.pEntries[i].hcod)
		{
			refBitStream.Skip(refTable.pEntries[i].hlen);

			refX = refTable.pEntries[i].x;
			refY = refTable.pEntries[i].y;

			/*
			2.4.2.7 ''
			-Quote: "If one or both values of a pair is not zero, one or two sign bits are appended to the Huffman code word."
			0 is positive, 1 is negative
			*/
			if(refTable.linbits)
			{
				if(refX == 15) //max_table_entry
				{
					refX += (int16)refBitStream.Read(refTable.linbits);
				}
			}
			if(refX)
			{
				if(refBitStream.Read(1))
					refX = -refX;
			}

			if(refTable.linbits)
			{
				if(refY == 15) //max_table_entry
				{
					refY += (int16)refBitStream.Read(refTable.linbits);
				}
			}
			if(refY)
			{
				if(refBitStream.Read(1))
					refY = -refY;
			}

			return;
		}
	}

	ASSERT(0);
}

void CMP3Decoder::InverseMDCT(uint8 blockType, const float64(&refInput)[MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], double(&refOutput)[36])
{
	register uint8 i, k;
	register double sum;

	//2.4.3.4 'IMDCT'
	//inverse modified discrete cosine transform

	//2.4.3.4 'Windowing'
	static const float64 window[3][36] =
	{
		//block_type = 0
		{0.043619387365335958, 0.13052619222005146, 0.21643961393810265, 0.30070579950427284, 0.38268343236508939, 0.46174861323503347, 0.53729960834682333, 0.6087614290087201, 0.67559020761565969, 0.73727733681012342, 0.79335334029123461, 0.84339144581288517, 0.88701083317822127, 0.92387953251128629, 0.95371695074822649, 0.97629600711993303, 0.99144486137381027, 0.99904822158185769, 0.9990482215818578, 0.9914448613738106, 0.97629600711993381, 0.95371695074822749, 0.92387953251128752, 0.8870108331782226, 0.84339144581288683, 0.7933533402912365, 0.73727733681012553, 0.67559020761566202, 0.60876142900872254, 0.537299608346826, 0.4617486132350363, 0.38268343236509234, 0.30070579950427584, 0.21643961393810579, 0.13052619222005465, 0.043619387365339178},
		//block_type = 1
		{0.043619387365335958, 0.13052619222005146, 0.21643961393810265, 0.30070579950427284, 0.38268343236508939, 0.46174861323503347, 0.53729960834682333, 0.6087614290087201, 0.67559020761565969, 0.73727733681012342, 0.79335334029123461, 0.84339144581288517, 0.88701083317822127, 0.92387953251128629, 0.95371695074822649, 0.97629600711993303, 0.99144486137381027, 0.99904822158185769, 1, 1, 1, 1, 1, 1, 0.9914448613738106, 0.92387953251128752, 0.7933533402912365, 0.60876142900872254, 0.38268343236509234, 0.13052619222005465, 0, 0, 0, 0, 0, 0},
		//block_type = 3
		{0, 0, 0, 0, 0, 0, 0.13052619222005146, 0.38268343236508945, 0.6087614290087201, 0.79335334029123461, 0.92387953251128629, 0.99144486137381027, 1, 1, 1, 1, 1, 1, 0.9990482215818578, 0.9914448613738106, 0.97629600711993381, 0.95371695074822749, 0.92387953251128752, 0.8870108331782226, 0.84339144581288683, 0.7933533402912365, 0.73727733681012553, 0.67559020761566202, 0.60876142900872254, 0.537299608346826, 0.4617486132350363, 0.38268343236509234, 0.30070579950427584, 0.21643961393810579, 0.13052619222005465, 0.043619387365339178}
	};

	ASSERT(blockType != 2);
	if(blockType == 3)
		blockType = 2;

	for(i = 0; i < 36; i++)
	{
		sum = 0;
		for(k = 0; k < 18; k++)
		{
			sum += refInput[k] * cos((PI / 72) * (2.0 * i + 19.0) * (2.0 * k + 1.0));
		}

		refOutput[i] = sum * window[blockType][i];
	}
}

void CMP3Decoder::InverseMDCTShort(const float64(&refInput)[MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], float64(&refOutput)[36])
{
	register uint8 i, j, k;
	register float64 sum;
	float64 tmp[12];

	static const float64 window[12] = {0.13052619222005146, 0.38268343236508945, 0.6087614290087201, 0.79335334029123461, 0.92387953251128629, 0.99144486137381027, 0.9914448613738106, 0.92387953251128752, 0.7933533402912365, 0.60876142900872254, 0.38268343236509234, 0.13052619222005465};

	for(i = 0; i < 36; i++)
		refOutput[i] = 0;

	for(j = 0; j < 3; j++)
	{
		for(i = 0; i < 12; i++)
		{
			sum = 0;
			for(k = 0; k < 6; k++)
			{
				sum += refInput[j + 3 * k] * cos((PI / 24) * (2.0 * i + 7.0) * (2.0 * k + 1.0));
			}
			tmp[i] = sum * window[i];
		}
		for(i = 0; i < 12; i++)
		{
			refOutput[6 * j + i + 6] += tmp[i];
		}
	}
}

void CMP3Decoder::Progress()
{
	uint8 nGranules, gr, ch, subBandSample, subBand, nAncillaryBits;;
	uint16 samplesPerFrame;
	int32 nextMainDataOffset;
	uint32 part2_start;
	float32 *pChannels[2];
	SFrameInfo frameInfo;
	CAudioBuffer<float32> *pAudioBuffer;
	UScaleFactors scaleFactors[2];
	int16 is[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES];
	float64 xr[2][MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES];
	float64 hybridOut[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES];
	float64 polyPhaseIn[MP3_SUBBANDLIMIT];

	pAudioBuffer = nullptr;

	CBitInputStream bitStream(this->mainDataBuffer);
	
	while(!this->frameInfoQueue.IsEmpty())
	{
		nextMainDataOffset = this->lastFrameStart - this->main_data_end - this->frameInfoQueue.Get(0).sideInfo.main_data_begin;

		if(nextMainDataOffset < 0)
		{
			//not enough main data... wait for next packet
			ASSERT(0);
			break;
		}
		else if(nextMainDataOffset > 0)
		{
			this->main_data_end += nextMainDataOffset; //note that these are bytes!

			for(; nextMainDataOffset > 0; nextMainDataOffset--)
				bitStream.Skip(8);
		}
		//check the size
		if(this->mainDataBuffer.GetNumberOfElements() < uint32((this->frameInfoQueue.Get(0).sideInfo.main_data_length / 8)))
		{
			//not enough main data... wait for next packet
			//IMPOSSIBLE
			ASSERT(0);
			break;
		}
		frameInfo = this->frameInfoQueue.PopFront();

		//samplesPerFrame is of course per channel
		if(frameInfo.frameHeader.isMpeg1)
		{
			samplesPerFrame = 1152; //2.4.2.1 says we have 1152 samples per channel for each frame
			nGranules = 2;
		}
		else
		{
			samplesPerFrame = 576; //For MPEG-2 we have only one granule
			nGranules = 1;
		}

		//Allocate frame
		switch(frameInfo.frameHeader.nChannels)
		{
		case 1:
			pAudioBuffer = new CAudioBuffer<float32>(EChannelLayout::Mono, samplesPerFrame);
			break;
		case 2:
			pAudioBuffer = new CAudioBuffer<float32>(EChannelLayout::Stereo, samplesPerFrame);
			pChannels[1] = pAudioBuffer->GetChannel(EChannel::Right);
			break;
		}
		pChannels[0] = pAudioBuffer->GetChannel(EChannel::Left);
		
		//2.4.3.4

		for(gr = 0; gr < nGranules; gr++)
		{
			for(ch = 0; ch < this->refStream.nChannels; ch++)
			{
				part2_start = bitStream.GetNumberOfReadBits();

				//2. Decode Scalefactors
				if(frameInfo.frameHeader.isMpeg1)
				{
					this->DecodeScalefactors(gr, ch, scaleFactors, frameInfo.sideInfo, bitStream);
				}
				else
				{
					//mpeg-2 has special scalefactors
					this->DecodeScalefactorsMPEG2(gr, ch, scaleFactors, frameInfo.sideInfo.ch[ch].gr[gr], frameInfo.frameHeader, bitStream);
				}
				
				//3. Huffman decoding
				this->DecodeHuffmanBits(is, part2_start, frameInfo.sideInfo.ch[ch].gr[gr], bitStream);

				//4. Dequantize samples
				this->DequantizeSamples(xr[ch], is, scaleFactors[ch], frameInfo.sideInfo.ch[ch].gr[gr]);
			}

			if(frameInfo.frameHeader.isMpeg1)
			{
				if(frameInfo.frameHeader.isIntensityStereo)
				{
					ASSERT(0);
				}

				if(frameInfo.frameHeader.isMS_Stereo)
				{
					//decode ms stereo
					this->DecodeMSStereo(xr);
				}
			}
			else
			{
				if(frameInfo.frameHeader.isIntensityStereo)
				{
					this->DecodeIntensityStereoMPEG2(frameInfo.frameHeader, frameInfo.sideInfo.ch[0].gr[gr], frameInfo.sideInfo.ch[1].gr[gr], xr, scaleFactors);
				}

				if(frameInfo.frameHeader.isMS_Stereo)
				{
					ASSERT(0);
				}
			}

			for(ch = 0; ch < this->refStream.nChannels; ch++)
			{
				if(frameInfo.sideInfo.ch[ch].gr[gr].window_switching_flag && frameInfo.sideInfo.ch[ch].gr[gr].block_type == 2)
				{
					//reorder
					//MIGHT BE DANGEROUS BECAUSE USING SAME BUFFER FOR IN- AND OUTPUT
					this->Reorder(frameInfo.sideInfo.ch[ch].gr[gr], xr[ch]);
				}

				//'Synthesis filterbank'

				//Quote: "The frequency lines are preprocessed by the "alias reduction" scheme (see the block diagrams in in 3-Annex A Figure 3-A.5 and in 3-Annex B Table 3-B.9. for the coefficients)
				this->AntiAlias(xr[ch], frameInfo.sideInfo.ch[ch].gr[gr]);
				this->ApplyHybridFilter(hybridOut, xr[ch], this->previousBlocks[ch], frameInfo.sideInfo.ch[ch].gr[gr]);
				this->ApplyFrequencyInversion(hybridOut);

				for(subBandSample = 0; subBandSample < MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES; subBandSample++)
				{
					for(subBand = 0; subBand < MP3_SUBBANDLIMIT; subBand++)
						polyPhaseIn[subBand] = hybridOut[subBand][subBandSample];

					ApplySynthesisSubbandFilter(polyPhaseIn, this->V[ch], pChannels[ch]);

					pChannels[ch] += 32;
				}
			}
		}

		//Ready the frame
		this->AddFrame(new CAudioFrame(pAudioBuffer));

		//Skip ancillary bits
		nAncillaryBits = (8 - (frameInfo.sideInfo.main_data_length % 8)) % 8;
		bitStream.Skip(nAncillaryBits);

		this->lastFrameStart += frameInfo.frameHeader.mainDataSize;
		this->main_data_end += ((frameInfo.sideInfo.main_data_length + nAncillaryBits) / 8);

		if(this->main_data_end > 4096)
		{
			this->lastFrameStart -= 4096;
			this->main_data_end -= 4096;
		}
	}
}

void CMP3Decoder::Reorder(const SGranuleInfo &refGranuleInfo, float64(&refxr)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES])
{
	uint8 scaleFactorBand, window, cb_begin, cb_width, i, j;
	uint16 source, dest;
	float64 tmp[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES];

	const SScaleFactorTable &refScaleFactorTable = this->GetScalefactorBandTable();

	/*
	2.4.2.7 'Huffman_code_bits':
	Quote: "If the block_type is 2 (short blocks) then the Huffman encoded data are ordered in a pattern similar to that of the scalefactor values"
	*/

	for(i = 0; i < MP3_SUBBANDLIMIT; i++)
	{
		for(j = 0; j < MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES; j++)
		{
			tmp[i][j] = refxr[i][j];
			refxr[i][j] = 0;
		}
	}

	if(refGranuleInfo.mixed_block_flag)
	{
		ASSERT(0);
		return;
	}

	//pure short block
	for(scaleFactorBand = 0; scaleFactorBand < 13; scaleFactorBand++) //2.4.2.7 'cb_limit_short'
	{
		cb_begin = refScaleFactorTable.s[scaleFactorBand];
		cb_width = refScaleFactorTable.s[scaleFactorBand + 1] - cb_begin;
		for(window = 0; window < 3; window++)
		{
			for(i = 0; i < cb_width; i++)
			{
				source = cb_begin * 3 + window * cb_width + i;
				dest = cb_begin * 3 + window + i * 3;

				refxr[dest / MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES][dest % MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES] = tmp[source / MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES][source % MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES];
			}
		}
	}
}

//Public methods
void CMP3Decoder::Decode(const SPacket &refPacket)
{
	uint32 frameHeader;
	SFrameInfo frameInfo;
	
	CBufferInputStream input(refPacket.pData, refPacket.size);
	
	//The parser should put the header at the start of the packet..
	//...additionally only one header should be in a packet
	if(FindNextFrameHeader(frameHeader, input))
	{
		ParseFrameHeader(frameHeader, frameInfo.frameHeader);

		ASSERT(!frameInfo.frameHeader.crcFollows);

		//Update codec info
		if(!this->refStream.vbr)
			this->refStream.bitRate = frameInfo.frameHeader.bitRate;
		this->refStream.nChannels = frameInfo.frameHeader.nChannels;
		this->refStream.sampleRate = frameInfo.frameHeader.sampleRate;
		
		this->DecodeSideInformation(frameInfo.sideInfo, frameInfo.frameHeader, input);

		this->frameInfoQueue.InsertTail(frameInfo);

		//Put the rest of the packet into the main_data buffer
		while(!input.HitEnd())
		{
			input.FlushTo(this->mainDataBuffer);
			this->Progress();
		}
	}
}

void CMP3Decoder::Reset()
{
	uint8 subBand, subBandSample;
	uint16 i;

	ADecoder::Reset();

	this->lastFrameStart = 0;
	this->main_data_end = 0;
	this->frameInfoQueue.Release();
	this->mainDataBuffer.Release();

	for(subBand = 0; subBand < MP3_SUBBANDLIMIT; subBand++)
	{
		for(subBandSample = 0; subBandSample < MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES; subBandSample++)
		{
			this->previousBlocks[0][subBand][subBandSample] = 0;
			this->previousBlocks[1][subBand][subBandSample] = 0;
		}
	}

	//2.4.3.3 'Synthesis subband filter', "the samples in that subband are set to zero"
	for(i = 0; i < 1024; i++)
	{
		this->V[0][i] = 0;
		this->V[1][i] = 0;
	}
}
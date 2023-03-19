//Class header
#include "CMP2Decoder.h"
//Local
#include "../MPEGAudio.h"

//Constructor
CMP2Decoder::CMP2Decoder(AStream &refStream) : AAudioDecoder(refStream)
{
	uint16 i;
	
	//2.4.3.3 'Synthesis subband filter', "the samples in that subband are set to zero"
	for(i = 0; i < 1024; i++)
	{
		this->V[0][i] = 0;
		this->V[1][i] = 0;
	}
}

//Private methods
void CMP2Decoder::DecodeBitAllocation(uint8 sblimit, uint8(&refBitAlloc)[MP2_MAXCHANNELS][MP2_SUBBANDLIMIT], const SBitAllocationTable *pBitAllocTable, CBitInputStream &refBitStream) const
{
	uint8 sb, ch;

	//Implementation by 2.4.1.6 and 2.4.3.3 'Bit allocation decoding'

	for(sb = 0; sb < sblimit; sb++)
	{
		for(ch = 0; ch < this->refStream.nChannels; ch++)
		{
			refBitAlloc[ch][sb] = (uint8)refBitStream.Read(pBitAllocTable[sb].nbal);
		}
	}
}

void CMP2Decoder::DecodeSamples(uint8 sblimit, uint16(&refSamples)[MP2_MAXCHANNELS][MP2_NUMBEROFCONSECUTIVESUBBANDSAMPLES][MP2_SUBBANDLIMIT], const uint8(&refBitAlloc)[MP2_MAXCHANNELS][MP2_SUBBANDLIMIT], const SBitAllocationTable *pBitAllocTable, CBitInputStream &refBitStream) const
{
	uint8 sb, ch, m;
	uint16 c;

	//Implementation by 2.4.1.6 and 2.4.3.3 'Requantization of subband samples'

	for(sb = 0; sb < sblimit; sb++)
	{
		for(ch = 0; ch < this->refStream.nChannels; ch++)
		{
			if(refBitAlloc[ch][sb])
			{
				const uint16 &refStep = pBitAllocTable[sb].index[refBitAlloc[ch][sb]];
				const SQuantizationClass &refQuantClass = this->GetQuantizationClass(refStep);

				if(refQuantClass.grouping)
				{
					//nLevels = step here
					c = (uint16)refBitStream.Read(refQuantClass.bitsPerCodeWord);

					for(m = 0; m < 3; m++)
					{
						refSamples[ch][m][sb] = c % refStep;
						c /= refStep;
					}
				}
				else
				{
					for(m = 0; m < 3; m++)
					{
						refSamples[ch][m][sb] = (uint16)refBitStream.Read(refQuantClass.bitsPerCodeWord);
					}
				}
			}
		}
	}
}

void CMP2Decoder::DecodeScalefactors(uint8 sblimit, uint8(&refScalefactors)[MP2_MAXCHANNELS][MP2_NUMBEROFCONSECUTIVESUBBANDSAMPLES][MP2_SUBBANDLIMIT], const uint8(&refBitAlloc)[MP2_MAXCHANNELS][MP2_SUBBANDLIMIT], const uint8(&refscfsi)[MP2_MAXCHANNELS][MP2_SUBBANDLIMIT], CBitInputStream &refBitStream) const
{
	uint8 sb, ch;

	//Implementation by 2.4.1.6 and 2.4.3.3 'Scalefactor selection information decoding'

	for(sb = 0; sb < sblimit; sb++)
	{
		for(ch = 0; ch < this->refStream.nChannels; ch++)
		{
			if(refBitAlloc[ch][sb])
			{
				switch(refscfsi[ch][sb])
				{
				case 0:
					{
						refScalefactors[ch][0][sb] = (uint8)refBitStream.Read(6);
						refScalefactors[ch][1][sb] = (uint8)refBitStream.Read(6);
						refScalefactors[ch][2][sb] = (uint8)refBitStream.Read(6);
					}
					break;
				case 1:
					{
						refScalefactors[ch][0][sb] = (uint8)refBitStream.Read(6);
						refScalefactors[ch][1][sb] = refScalefactors[ch][0][sb];
						refScalefactors[ch][2][sb] = (uint8)refBitStream.Read(6);
					}
					break;
				case 2:
					{
						refScalefactors[ch][0][sb] = (uint8)refBitStream.Read(6);
						refScalefactors[ch][1][sb] = refScalefactors[ch][0][sb];
						refScalefactors[ch][2][sb] = refScalefactors[ch][0][sb];
					}
					break;
				case 3:
					{
						refScalefactors[ch][0][sb] = (uint8)refBitStream.Read(6);
						refScalefactors[ch][1][sb] = (uint8)refBitStream.Read(6);
						refScalefactors[ch][2][sb] = refScalefactors[ch][1][sb];
					}
					break;
				}
			}
		}
	}
}

void CMP2Decoder::DecodeScalefactorSelectionInformation(uint8 sblimit, uint8(&refscfsi)[MP2_MAXCHANNELS][MP2_SUBBANDLIMIT], const uint8(&refBitAlloc)[MP2_MAXCHANNELS][MP2_SUBBANDLIMIT], CBitInputStream &refBitStream) const
{
	uint8 sb, ch;

	//Implementation by 2.4.1.6 and 2.4.3.3 'Scalefactor selection information decoding'

	for(sb = 0; sb < sblimit; sb++)
	{
		for(ch = 0; ch < this->refStream.nChannels; ch++)
		{
			if(refBitAlloc[ch][sb])
			{
				refscfsi[ch][sb] = (uint8)refBitStream.Read(2);
			}
		}
	}
}

void CMP2Decoder::GetBitAllocationTable(bool isMPEG1, uint8 &refSubbandLimit, const SBitAllocationTable *&refpBitAllocTable) const
{
	uint16 bitRatePerChannel; //in kbit/s

	bitRatePerChannel = (this->refStream.bitRate / 1000) / this->refStream.nChannels;
	
	if(!isMPEG1)
	{
		//MPEG-2: Table B.1
		static SBitAllocationTable allocMPEG2[30] =
		{
			//Subbands...
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383}},
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383}},
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383}},
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383}},
			{3,{0, 3, 5, 9, 15, 31, 63, 127}},
			{3,{0, 3, 5, 9, 15, 31, 63, 127}},
			{3,{0, 3, 5, 9, 15, 31, 63, 127}},
			{3,{0, 3, 5, 9, 15, 31, 63, 127}},
			{3,{0, 3, 5, 9, 15, 31, 63, 127}},
			{3,{0, 3, 5, 9, 15, 31, 63, 127}},
			{3,{0, 3, 5, 9, 15, 31, 63, 127}},
			{2,{0, 3, 5, 9}},
			{2,{0, 3, 5, 9}},
			{2,{0, 3, 5, 9}},
			{2,{0, 3, 5, 9}},
			{2,{0, 3, 5, 9}},
			{2,{0, 3, 5, 9}},
			{2,{0, 3, 5, 9}},
			{2,{0, 3, 5, 9}},
			{2,{0, 3, 5, 9}},
			{2,{0, 3, 5, 9}},
			{2,{0, 3, 5, 9}},
			{2,{0, 3, 5, 9}},
			{2,{0, 3, 5, 9}},
			{2,{0, 3, 5, 9}},
			{2,{0, 3, 5, 9}},
			{2,{0, 3, 5, 9}},
			{2,{0, 3, 5, 9}},
			{2,{0, 3, 5, 9}},
			{2,{0, 3, 5, 9}},
		};

		refSubbandLimit = 30;
		refpBitAllocTable = allocMPEG2;
		
		return;
	}

	if((this->refStream.sampleRate == 48000 && bitRatePerChannel >= 56) || (IN_RANGE(bitRatePerChannel, 56, 80)))
	{
		//3-B.2a
		static SBitAllocationTable alloc1[27] =
		{
			//Subbands...
			{4,{0, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383, 32767, 65535}},
			{4,{0, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383, 32767, 65535}},
			{4,{0, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383, 32767, 65535}},
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 65535}},
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 65535}},
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 65535}},
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 65535}},
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 65535}},
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 65535}},
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 65535}},
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{2,{0, 3, 5, 65535}},
			{2,{0, 3, 5, 65535}},
			{2,{0, 3, 5, 65535}},
			{2,{0, 3, 5, 65535}},
		};

		refSubbandLimit = 27;
		refpBitAllocTable = alloc1;
	}
	else if(this->refStream.sampleRate != 48000 && bitRatePerChannel >= 96)
	{
		//3-B.2b
		static SBitAllocationTable alloc2[30] =
		{
			//Subbands...
			{4,{0, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383, 32767, 65535}},
			{4,{0, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383, 32767, 65535}},
			{4,{0, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383, 32767, 65535}},
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 65535}},
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 65535}},
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 65535}},
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 65535}},
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 65535}},
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 65535}},
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 65535}},
			{4,{0, 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{3,{0, 3, 5, 7, 9, 15, 31, 65535}},
			{2,{0, 3, 5, 65535}},
			{2,{0, 3, 5, 65535}},
			{2,{0, 3, 5, 65535}},
			{2,{0, 3, 5, 65535}},
			{2,{0, 3, 5, 65535}},
			{2,{0, 3, 5, 65535}},
			{2,{0, 3, 5, 65535}},
		};

		refSubbandLimit = 30;
		refpBitAllocTable = alloc2;
	}
	else if(this->refStream.sampleRate != 32000 && bitRatePerChannel <= 48)
	{
		//3-B.2c
		ASSERT(0);
	}
	else
	{
		//3-B.2d
		ASSERT(0);
	}
}

const CMP2Decoder::SQuantizationClass &CMP2Decoder::GetQuantizationClass(uint16 nSteps) const
{
	//3-B.4

	switch(nSteps)
	{
	case 3:
		{
			static const SQuantizationClass c3 =
			{
				true,
				5,
				1.33333333333,
				0.5
			};

			return c3;
		}
		break;
	case 5:
		{
			static const SQuantizationClass c5 =
			{
				true,
				7,
				1.6,
				0.5
			};

			return c5;
		}
		break;
	case 7:
		{
			static const SQuantizationClass c7 =
			{
				false,
				3,
				1.14285714286,
				0.25
			};

			return c7;
		}
		break;
	case 9:
		{
			static const SQuantizationClass c9 =
			{
				true,
				10,
				1.77777777777,
				0.5
			};

			return c9;
		}
		break;
	case 15:
		{
			static const SQuantizationClass c15 =
			{
				false,
				4,
				1.06666666666,
				0.125
			};

			return c15;
		}
		break;
	case 31:
		{
			static const SQuantizationClass c31 =
			{
				false,
				5,
				1.03225806452,
				0.0625
			};

			return c31;
		}
		break;
	case 63:
		{
			static const SQuantizationClass c63 =
			{
				false,
				6,
				1.01587301587,
				0.03125
			};

			return c63;
		}
		break;
	case 127:
		{
			static const SQuantizationClass c127 =
			{
				false,
				7,
				1.00787401575,
				0.01562500000
			};

			return c127;
		}
		break;
	case 255:
		{
			static const SQuantizationClass c255 =
			{
				false,
				8,
				1.00392156863,
				0.00781250000
			};

			return c255;
		}
		break;
	case 511:
		{
			static const SQuantizationClass c511 =
			{
				false,
				9,
				1.00195694716,
				0.00390625000
			};

			return c511;
		}
		break;
	case 1023:
		{
			static const SQuantizationClass c1023 =
			{
				false,
				10,
				1.00097751711,
				0.001953125
			};

			return c1023;
		}
		break;
	case 2047:
		{
			static const SQuantizationClass c2047 =
			{
				false,
				11,
				1.00048851979,
				0.00097656250
			};

			return c2047;
		}
		break;
	case 4095:
		{
			static const SQuantizationClass c4095 =
			{
				false,
				12,
				1.00024420024,
				0.00048828125
			};

			return c4095;
		}
		break;
	case 8191:
		{
			static const SQuantizationClass c8191 =
			{
				false,
				13,
				1.00012208522,
				0.00024414063
			};

			return c8191;
		}
		break;
	case 16383:
		{
			static const SQuantizationClass c16383 =
			{
				false,
				14,
				1.00006103888,
				0.00012207031
			};

			return c16383;
		}
		break;
	case 65535:
		{
			static const SQuantizationClass c65535 =
			{
				false,
				16,
				1.00001525902,
				0.00003051758
			};

			return c65535;
		}
		break;
	default:
		ASSERT(0);
	}
}

void CMP2Decoder::RequantizeSamples(uint8 sblimit, uint8 m_scaleFactors, const SBitAllocationTable *pBitAllocTable, const uint8(&refBitAlloc)[MP2_MAXCHANNELS][MP2_SUBBANDLIMIT], const uint8(&refScalefactors)[MP2_MAXCHANNELS][MP2_NUMBEROFCONSECUTIVESUBBANDSAMPLES][MP2_SUBBANDLIMIT], const uint16(&refSamples)[MP2_MAXCHANNELS][MP2_NUMBEROFCONSECUTIVESUBBANDSAMPLES][MP2_SUBBANDLIMIT], double(&refFraction)[MP2_MAXCHANNELS][MP2_NUMBEROFCONSECUTIVESUBBANDSAMPLES][MP2_SUBBANDLIMIT]) const
{
	uint8 sb, m, ch, msbOffset;

	static const double scaleFactors[63] =
	{
		2.00000000000000, 1.58740105196820, 1.25992104989487, 1.00000000000000,
		0.79370052598410, 0.62996052494744, 0.50000000000000, 0.39685026299205,
		0.31498026247372, 0.25000000000000, 0.19842513149602, 0.15749013123686,
		0.12500000000000, 0.09921256574801, 0.07874506561843, 0.06250000000000,
		0.04960628287401, 0.03937253280921, 0.03125000000000, 0.02480314143700,
		0.01968626640461, 0.01562500000000, 0.01240157071850, 0.00984313320230,
		0.00781250000000, 0.00620078535925, 0.00492156660115, 0.00390625000000,
		0.00310039267963, 0.00246078330058, 0.00195312500000, 0.00155019633981,
		0.00123039165029, 0.00097656250000, 0.00077509816991, 0.00061519582514,
		0.00048828125000, 0.00038754908495, 0.00030759791257, 0.00024414062500,
		0.00019377454248, 0.00015379895629, 0.00012207031250, 0.00009688727124,
		0.00007689947814, 0.00006103515625, 0.00004844363562, 0.00003844973907,
		0.00003051757813, 0.00002422181781, 0.00001922486954, 0.00001525878906,
		0.00001211090890, 0.00000961243477, 0.00000762939453, 0.00000605545445,
		0.00000480621738, 0.00000381469727, 0.00000302772723, 0.00000240310869,
		0.00000190734863, 0.00000151386361, 0.00000120155435
	};

	for(sb = 0; sb < sblimit; sb++)
	{
		for(m = 0; m < 3; m++)
		{
			for(ch = 0; ch < this->refStream.nChannels; ch++)
			{
				//2.4.3.3 'Synthesis subband filter' says that sample needs to be reset if no data exists
				refFraction[ch][m][sb] = 0;

				if(refBitAlloc[ch][sb])
				{
					const uint16 &refStep = pBitAllocTable[sb].index[refBitAlloc[ch][sb]];
					const SQuantizationClass &refQuantClass = this->GetQuantizationClass(refStep);

					if(refQuantClass.grouping)
					{
						//for grouped classes we have special msbOffsets
						//as when decoding we use % step on the sample... this gives the msbOffsets:
						if(refStep == 3)
						{
							msbOffset = 2;
						}
						else if(refStep == 5)
						{
							msbOffset = 3;
						}
						else //refStep == 9
						{
							msbOffset = 4;
						}
					}
					else
					{
						msbOffset = refQuantClass.bitsPerCodeWord;
					}

					//we need to invert the MSB... if bit is not set -> -1 else 0
					if(!((refSamples[ch][m][sb] >> (msbOffset - 1)) & 1))
					{
						refFraction[ch][m][sb] = -1;
					}

					//Build the 2's complement sample
					refFraction[ch][m][sb] += (double)(refSamples[ch][m][sb] & ((1 << (msbOffset - 1)) - 1)) / (double)(1 << (msbOffset - 1));

					//Requantize the sample using the formula from 2.4.3.3 'Requantization of subband samples'
					refFraction[ch][m][sb] += refQuantClass.D;
					refFraction[ch][m][sb] *= refQuantClass.C;

					//2.4.3.3 'Scalefactor decoding' says that after requantization we should multiply the sample with it's scalefactor
					refFraction[ch][m][sb] *= scaleFactors[refScalefactors[ch][m_scaleFactors][sb]];
				}
			}
		}
	}

	//Reset all samples which are beyond the sblimit for this frame... again this is needed for the synthesis subband filter
	for(sb = sblimit; sb < MP2_SUBBANDLIMIT; sb++)
	{
		for(m = 0; m < 3; m++)
		{
			for(ch = 0; ch < this->refStream.nChannels; ch++)
			{
				refFraction[ch][m][sb] = 0;
			}
		}
	}
}

//Public methods
void CMP2Decoder::Decode(const SPacket &refPacket)
{
	uint8 subbandLimit, gr, k;
	uint32 frameHeader;
	const SBitAllocationTable *pBitAllocTable;
	CAudioBuffer<float32> *pAudioBuffer;
	CAudioFrame *pFrame;
	float32 *pLeft, *pRight;
	SMPEGAudioFrameHeader header;

	uint8 bitAlloc[MP2_MAXCHANNELS][MP2_SUBBANDLIMIT];
	uint8 scalefactorSelectionInformation[MP2_MAXCHANNELS][MP2_SUBBANDLIMIT];
	uint8 scalefactors[MP2_MAXCHANNELS][MP2_NUMBEROFCONSECUTIVESUBBANDSAMPLES][MP2_SUBBANDLIMIT];
	uint16 samples[MP2_MAXCHANNELS][MP2_NUMBEROFCONSECUTIVESUBBANDSAMPLES][MP2_SUBBANDLIMIT];
	float64 fraction[MP2_MAXCHANNELS][MP2_NUMBEROFCONSECUTIVESUBBANDSAMPLES][MP2_SUBBANDLIMIT];

	pAudioBuffer = nullptr;
	pLeft = nullptr;
	pRight = nullptr;
	
	CBufferInputStream bufferStream(refPacket.pData, refPacket.size);

	frameHeader = 0;
	if(FindNextFrameHeader(frameHeader, bufferStream))
	{
		ParseFrameHeader(frameHeader, header);

		ASSERT(!header.crcFollows);
		
		//Update codec info
		this->refStream.bitRate = header.bitRate;
		this->refStream.nChannels = header.nChannels;
		this->refStream.sampleRate = header.sampleRate;
		
		//2.4.2.1 says we have 1152 samples per channel for each frame
		switch(header.nChannels)
		{
		case 1:
			pAudioBuffer = new CAudioBuffer<float32>(EChannelLayout::Mono, 1152);
			pLeft = pAudioBuffer->GetChannel(EChannel::Left);
			break;
		case 2:
			pAudioBuffer = new CAudioBuffer<float32>(EChannelLayout::Stereo, 1152);
			pLeft = pAudioBuffer->GetChannel(EChannel::Left);
			pRight = pAudioBuffer->GetChannel(EChannel::Right);
			break;
		}
		
		//Decoding process described in: 2.4.3.3 and 3-A.1
		CBitInputStream bitStream(bufferStream);
		
		//1. bit allocation decoding
		this->GetBitAllocationTable(header.isMpeg1, subbandLimit, pBitAllocTable);
		this->DecodeBitAllocation(subbandLimit, bitAlloc, pBitAllocTable, bitStream);

		//2. Scalefactor selection information decoding
		this->DecodeScalefactorSelectionInformation(subbandLimit, scalefactorSelectionInformation, bitAlloc, bitStream);

		//3. Scalefactor decoding
		this->DecodeScalefactors(subbandLimit, scalefactors, bitAlloc, scalefactorSelectionInformation, bitStream);
		
		for(gr = 0; gr < 12; gr++)
		{
			this->DecodeSamples(subbandLimit, samples, bitAlloc, pBitAllocTable, bitStream);
			this->RequantizeSamples(subbandLimit, gr >> 2, pBitAllocTable, bitAlloc, scalefactors, samples, fraction);
			
			for(k = 0; k < 3; k++) //3 consecutive subband samples
			{
				//left channel
				ApplySynthesisSubbandFilter(fraction[0][k], this->V[0], pLeft);
				pLeft += 32;

				//right channel
				if(this->refStream.nChannels == 2)
				{
					ApplySynthesisSubbandFilter(fraction[1][k], this->V[1], pRight);
					pRight += 32;
				}
			}
		}

		pFrame = new CAudioFrame(pAudioBuffer);
		pFrame->pts = refPacket.pts;
		
		this->AddFrame(pFrame);
	}
}
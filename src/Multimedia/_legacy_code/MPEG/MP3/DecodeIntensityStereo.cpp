//Class header
#include "CMP3Decoder.h"

//Private methods
void CMP3Decoder::ApplyIntensityStereo(uint16 start, uint8 width, float64(&refxr)[2][MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], float64(&refIntensityFactors)[2][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES * MP3_SUBBANDLIMIT])
{
	uint8 subband, subbandSample;
	uint16 i;

	//MPEG-2 2.4.3.2

	for(i = start; i < start + width; i++)
	{
		subband = i / MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES;
		subbandSample = i % MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES;

		refxr[1][subband][subbandSample] = refxr[0][subband][subbandSample] * refIntensityFactors[0][i];
		refxr[0][subband][subbandSample] = refxr[0][subband][subbandSample] * refIntensityFactors[1][i];
	}
}

void CMP3Decoder::DecodeIntensityStereoMPEG2(const SMPEGAudioFrameHeader &refHeaderInfo, const SGranuleInfo &refLeftGranuleInfo, const SGranuleInfo &refRightGranuleInfo, float64(&refxr)[2][MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], UScaleFactors(&refScaleFactors)[2])
{
	uint8 subBand, subBandSample, lowerScaleFactorBandIdx, scaleFactorBandIdx, scaleFactorBandWidth, window;
	uint16 subBandSampleIndexOffset, i;
	float64 i0;
	float64 kFactors[2][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES * MP3_SUBBANDLIMIT];

	const SScaleFactorTable &refScaleFactorTable = this->GetScalefactorBandTable();

	//see 2.4.3.4 MPEG-2
	if(refRightGranuleInfo.scalefac_compress & 1)
	{
		i0 = 0.70710678118654752440084436210485; //1 / sqrt(2)
	}
	else
	{
		i0 = 0.84089641525371454303112547623321; // 1 / sqrt(sqrt(2))
	}

	if(refLeftGranuleInfo.block_type == 2 && refLeftGranuleInfo.window_switching_flag)
	{
		if(refLeftGranuleInfo.mixed_block_flag)
		{
			ASSERT(0);
		}
		else
		{
			//short blocks
			for(window = 0; window < 3; window++)
			{
				lowerScaleFactorBandIdx = (uint8)this->FindLastNonZeroEntryShortBlocks(window, refxr[1]);

				if(lowerScaleFactorBandIdx < 12)
				{
					refScaleFactors[1].s[window][12] = refScaleFactors[1].s[window][11];
					this->illegalIntensityPositionBuffer[3 * 12 + window] = this->illegalIntensityPositionBuffer[3 * (12 - 1) + window]; //legal/illegal in sfb 12 same as in sfb 11
				}
				else if(lowerScaleFactorBandIdx == 12)
				{
					refScaleFactors[1].s[window][12] = 0; //only sfb 12 in intensity mode, use factors corresponding to is_pos[12] == 0
					this->illegalIntensityPositionBuffer[3 * 12 + window] = 1; //the scf value 0 in sfb12 is "legal"
				}

				for(scaleFactorBandIdx = lowerScaleFactorBandIdx; scaleFactorBandIdx <= 12; scaleFactorBandIdx++)
				{
					if(refScaleFactors[1].s[window][scaleFactorBandIdx] != this->illegalIntensityPositionBuffer[3 * scaleFactorBandIdx + window])
					{
						scaleFactorBandWidth = refScaleFactorTable.s[scaleFactorBandIdx + 1] - refScaleFactorTable.s[scaleFactorBandIdx];
						i = 3 * refScaleFactorTable.s[scaleFactorBandIdx] + window * scaleFactorBandWidth;

						for(subBand = 0; subBand < scaleFactorBandWidth; subBand++)
						{
							this->DecodeIntensityStereoMPEG2KFactors(refScaleFactors[1].s[window][scaleFactorBandIdx], i0, i, kFactors);
							i++;
						}
					}
				}

				//process lower audio samples which are not in intensity mode
				for(scaleFactorBandIdx = 0; scaleFactorBandIdx < lowerScaleFactorBandIdx; scaleFactorBandIdx++)
				{
					if(refHeaderInfo.isMS_Stereo)
					{
						ASSERT(0);
					}
				}

				//intensity stereo processing of the remaining scalefactor bands
				for(scaleFactorBandIdx = lowerScaleFactorBandIdx; scaleFactorBandIdx <= 12; scaleFactorBandIdx++)
				{
					scaleFactorBandWidth = refScaleFactorTable.s[scaleFactorBandIdx + 1] - refScaleFactorTable.s[scaleFactorBandIdx];
					i = 3 * refScaleFactorTable.s[scaleFactorBandIdx] + window * scaleFactorBandWidth;

					if(refScaleFactors[1].s[window][scaleFactorBandIdx] != this->illegalIntensityPositionBuffer[3 * scaleFactorBandIdx + window])
					{
						this->ApplyIntensityStereo(i, scaleFactorBandWidth, refxr, kFactors);
					}
					else if(refHeaderInfo.isMS_Stereo)
					{
						ASSERT(0);
					}
				}
			}
		}
	}
	else
	{
		//long blocks

		//find last sample in right channel which is not 0.0
		subBand = 31;
		subBandSample = 17;

		while(true)
		{
			if(refxr[1][subBand][subBandSample])
			{
				//found
				subBandSampleIndexOffset = subBand * MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES + subBandSample;
				break;
			}
			else
			{
				if(subBandSample == 0)
				{
					subBandSample = 17;
					if(subBand == 0)
					{
						//all xr[1][][] are zero
						lowerScaleFactorBandIdx = 0;
						break;
					}
					else
					{
						subBand--;
					}
				}
				else
				{
					subBandSample--;
				}
			}
		}

		if(subBand == 0)
		{
			if(subBandSample)
			{
				//only xr[1][0][0] is != 0.0
				lowerScaleFactorBandIdx = 1;
			}
		}
		else
		{
			for(i = 0; refScaleFactorTable.l[i] <= subBandSampleIndexOffset; i++);
			lowerScaleFactorBandIdx = (uint8)i;
		}

		if(lowerScaleFactorBandIdx < 21)
		{
			//more than one scale factor band in intensity mode
			//copy factors and legal/illegal information from sfb20 to sfb21
			refScaleFactors[1].l[21] = refScaleFactors[1].l[20];
			this->illegalIntensityPositionBuffer[21] = this->illegalIntensityPositionBuffer[20];
		}
		else
		{
			ASSERT(0);
		}

		for(scaleFactorBandIdx = lowerScaleFactorBandIdx; scaleFactorBandIdx <= 21; scaleFactorBandIdx++)
		{
			if(refScaleFactors[1].l[scaleFactorBandIdx] != this->illegalIntensityPositionBuffer[scaleFactorBandIdx])
			{
				//legal value
				for(i = refScaleFactorTable.l[scaleFactorBandIdx]; i < refScaleFactorTable.l[scaleFactorBandIdx + 1]; i++)
				{
					this->DecodeIntensityStereoMPEG2KFactors(refScaleFactors[1].l[scaleFactorBandIdx], i0, i, kFactors);
				}
			}
		}

		/*
		process audio samples
		lower scalefactor bands are not in intensity mode,
		means if they are not MS Stereo there is nothing to do
		*/
		if(refHeaderInfo.isMS_Stereo)
		{
			ASSERT(0);
		}

		//intensity stereo processing of the remaining scalefactor bands
		for(scaleFactorBandIdx = lowerScaleFactorBandIdx; scaleFactorBandIdx <= 21; scaleFactorBandIdx++)
		{
			scaleFactorBandWidth = refScaleFactorTable.l[scaleFactorBandIdx + 1] - refScaleFactorTable.l[scaleFactorBandIdx];

			if(refScaleFactors[1].l[scaleFactorBandIdx] != this->illegalIntensityPositionBuffer[scaleFactorBandIdx])
			{
				//legal position
				this->ApplyIntensityStereo(refScaleFactorTable.l[scaleFactorBandIdx], scaleFactorBandWidth, refxr, kFactors);
			}
			else if(refHeaderInfo.isMS_Stereo)
			{
				ASSERT(0);
			}
		}
	}
}

void CMP3Decoder::DecodeIntensityStereoMPEG2KFactors(uint8 is_pos, float64 i0, uint16 index, float64(&refKFactors)[2][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES * MP3_SUBBANDLIMIT])
{
	//MPEG-2 2.4.3.2

	if(is_pos == 0)
	{
		refKFactors[0][index] = 1;
		refKFactors[1][index] = 1;
	}
	else if(is_pos & 1) //(is_pos % 2) == 1
	{
		refKFactors[0][index] = pow(i0, (is_pos + 1) / 2.0);
		refKFactors[1][index] = 1;
	}
	else
	{
		refKFactors[0][index] = 1;
		refKFactors[1][index] = pow(i0, is_pos / 2.0);
	}
}
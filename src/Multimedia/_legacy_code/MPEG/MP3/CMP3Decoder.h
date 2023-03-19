#pragma once
#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;
//Local
#include "../MPEGAudio.h"
//Definitions
#define MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES 18 //2.4.2.7 'gr'; well it is not clearly said but derivable from mp2
#define MP3_SUBBANDLIMIT 32

/*
Implementation follows:
ISO IEC 11172-3 (MPEG-1)
ISO IEC 13818-3 (MPEG-2)
*/

//Structs
struct SGranuleInfo
{
	uint16 part2_3_length;
	uint16 big_values;
	uint8 global_gain;
	uint16 scalefac_compress;
	bool window_switching_flag; //the MPEG-2 name; in MPEG-1: "blocksplit_flag"
	uint8 block_type;
	bool mixed_block_flag; //the MPEG-2 name; in MPEG-1: "switch_point"
	uint8 table_select[3];
	uint8 subblock_gain[3];
	uint8 region0_count; //the MPEG-2 name; in MPEG-1: "region_address1"
	uint8 region1_count; //the MPEG-2 name; in MPEG-1: "region_address2"
	bool preflag;
	bool scalefac_scale;
	uint8 count1table_select;
};

struct SChannelInfo
{
	bool scfsi[4];
	SGranuleInfo gr[2];
};

struct SSideInfo
{
	uint16 main_data_begin;
	uint16 main_data_length;
	SChannelInfo ch[2];
};

struct SFrameInfo
{
	SMPEGAudioFrameHeader frameHeader;
	SSideInfo sideInfo;
};

struct SHuffmanTableEntry //3-B.7
{
	uint8 x;
	uint8 y;
	uint8 hlen;
	uint16 hcod;
};

struct SHuffmanTable
{
	uint8 linbits;
	uint16 nEntries;
	const SHuffmanTableEntry *pEntries;
};

/*
3-B.8

only index of start is included.
thus last but one entry is the last entry in the table (in specification) + width of this band
last entry is end of frequency spectrum (for bounding in the algorithms)
*/
struct SScaleFactorTable
{
	uint16 l[23]; //long blocks
	uint8 s[14]; //short blocks
};

//Unions
union UScaleFactors
{
	uint8 l[23];
	uint8 s[3][13];
};


class CMP3Decoder : public AAudioDecoder
{
private:
	//Members
	uint16 lastFrameStart;
	uint16 main_data_end; //in bytes, always of last frame
	CLinkedList<SFrameInfo> frameInfoQueue;
	CFIFOBuffer mainDataBuffer;
	uint16 illegalIntensityPositionBuffer[54]; //for MPEG-2 intensity stereo
	float64 previousBlocks[2][MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES];
	float64 V[2][1024]; //For algorithm described in 3-A.2
	
	//Methods
	void AntiAlias(float64 (&refxr)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], const SGranuleInfo &refGranuleInfo);
	void ApplyFrequencyInversion(float64(&refValues)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES]);
	void ApplyHybridFilter(float64(&refOutput)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], const float64(&refInput)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], float64(&refPreviousBlocks)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], const SGranuleInfo &refGranuleInfo);
	void ApplyIntensityStereo(uint16 start, uint8 width, float64(&refxr)[2][MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], float64(&refIntensityFactors)[2][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES * MP3_SUBBANDLIMIT]);
	void DecodeHuffmanBits(int16(&refDecodedValues)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], uint32 part2_start, const SGranuleInfo &refGranuleInfo, CBitInputStream &refBitStream);
	void DecodeIntensityStereoMPEG2(const SMPEGAudioFrameHeader &refHeaderInfo, const SGranuleInfo &refLeftGranuleInfo, const SGranuleInfo &refRightGranuleInfo, float64(&refxr)[2][MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], UScaleFactors(&refScaleFactors)[2]);
	void DecodeIntensityStereoMPEG2KFactors(uint8 is_pos, float64 i0, uint16 index, float64(&refKFactors)[2][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES * MP3_SUBBANDLIMIT]);
	void DecodeMSStereo(float64(&reflr)[2][MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES]);
	void DecodeScalefactors(uint8 gr, uint8 ch, UScaleFactors(&refScaleFactors)[2], const SSideInfo &refSideInfo, CBitInputStream &refBitStream);
	void DecodeScalefactorsMPEG2(uint8 gr, uint8 ch, UScaleFactors(&refScaleFactors)[2], SGranuleInfo &refGranuleInfo, const SMPEGAudioFrameHeader &refFrameHeader, CBitInputStream &refBitStream);
	void DecodeSideInformation(SSideInfo &refSideInfo, const SMPEGAudioFrameHeader &refHeaderInfo, AInputStream &refInput);
	void DequantizeSamples(float64 (&refxr)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], const int16(&refis)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], const UScaleFactors &refScaleFactors, const SGranuleInfo &refGranuleInfo);
	int16 FindLastNonZeroEntryShortBlocks(uint8 window, float64(&refxr)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES]);
	const SHuffmanTable &GetHuffmanTable(uint8 index) const;
	const SScaleFactorTable &GetScalefactorBandTable() const;
	void HuffmanDecode(int16 &refX, int16 &refY, const SHuffmanTable &refTable, CBitInputStream &refBitStream);
	void HuffmanDecodeQuadruple(int8 &refV, int8 &refW, int16 &refX, int16 &refY, uint8 tableIdx, CBitInputStream &refBitStream);
	void InverseMDCT(uint8 blockType, const float64(&refInput)[MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], float64(&refOutput)[36]);
	void InverseMDCTShort(const float64(&refInput)[MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES], float64(&refOutput)[36]);
	void Progress();
	void Reorder(const SGranuleInfo &refGranuleInfo, float64(&refxr)[MP3_SUBBANDLIMIT][MP3_NUMBEROFCONSECUTIVESUBBANDSAMPLES]);

public:
	//Constructor
	CMP3Decoder(AStream &refStream);

	//Methods
	void Decode(const SPacket &refPacket);
	void Reset();
};
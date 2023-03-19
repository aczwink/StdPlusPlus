#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;
//Definitions
#define MP2_MAXCHANNELS 2
#define MP2_NUMBEROFCONSECUTIVESUBBANDSAMPLES 3 //always 3 consecutive subband samples per subband
#define MP2_SUBBANDLIMIT 32

class CMP2Decoder : public AAudioDecoder
{
	struct SBitAllocationTable
	{
		uint8 nbal;
		uint16 index[16];
	};

	struct SQuantizationClass //3-B.4
	{
		bool grouping;
		uint8 bitsPerCodeWord;
		double C;
		double D;
	};
private:
	//Members
	float64 V[2][1024]; //For algorithm described in 3-A.2
	
	//Methods
	void DecodeBitAllocation(uint8 sblimit, uint8(&refBitAlloc)[MP2_MAXCHANNELS][MP2_SUBBANDLIMIT], const SBitAllocationTable *pBitAllocTable, CBitInputStream &refBitStream) const;
	void DecodeSamples(uint8 sblimit, uint16(&refSamples)[MP2_MAXCHANNELS][MP2_NUMBEROFCONSECUTIVESUBBANDSAMPLES][MP2_SUBBANDLIMIT], const uint8(&refBitAlloc)[MP2_MAXCHANNELS][MP2_SUBBANDLIMIT], const SBitAllocationTable *pBitAllocTable, CBitInputStream &refBitStream) const;
	void DecodeScalefactors(uint8 sblimit, uint8(&refScalefactors)[MP2_MAXCHANNELS][MP2_NUMBEROFCONSECUTIVESUBBANDSAMPLES][MP2_SUBBANDLIMIT], const uint8(&refBitAlloc)[MP2_MAXCHANNELS][MP2_SUBBANDLIMIT], const uint8(&refscfsi)[MP2_MAXCHANNELS][MP2_SUBBANDLIMIT], CBitInputStream &refBitStream) const;
	void DecodeScalefactorSelectionInformation(uint8 sblimit, uint8(&refscfsi)[MP2_MAXCHANNELS][MP2_SUBBANDLIMIT], const uint8(&refBitAlloc)[MP2_MAXCHANNELS][MP2_SUBBANDLIMIT], CBitInputStream &refBitStream) const;
	void GetBitAllocationTable(bool isMPEG1, uint8 &refSubbandLimit, const SBitAllocationTable *&refpBitAllocTable) const;
	const SQuantizationClass &GetQuantizationClass(uint16 nSteps) const;
	void RequantizeSamples(uint8 sblimit, uint8 m_scaleFactors, const SBitAllocationTable *pBitAllocTable, const uint8(&refBitAlloc)[MP2_MAXCHANNELS][MP2_SUBBANDLIMIT], const uint8(&refScalefactors)[MP2_MAXCHANNELS][MP2_NUMBEROFCONSECUTIVESUBBANDSAMPLES][MP2_SUBBANDLIMIT], const uint16(&refSamples)[MP2_MAXCHANNELS][MP2_NUMBEROFCONSECUTIVESUBBANDSAMPLES][MP2_SUBBANDLIMIT], double(&refFraction)[MP2_MAXCHANNELS][MP2_NUMBEROFCONSECUTIVESUBBANDSAMPLES][MP2_SUBBANDLIMIT]) const;

public:
	//Constructor
	CMP2Decoder(AStream &refStream);

	//Methods
	void Decode(const SPacket &refPacket);
};
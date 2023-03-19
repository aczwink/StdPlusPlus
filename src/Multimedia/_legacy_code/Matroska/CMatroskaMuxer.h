#include <ACStdLib.h>
//Local
#include "Matroska.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;
using namespace Matroska;

class CMatroskaMuxer : public AMuxer
{
	struct SCueEntry
	{
		uint64 clusterOffset;
		CArray<uint32> streamIndices;
	};
private:
	//Members
	CArray<uint64> elementSizeOffsets;
	CFraction timeCodeScale;
	struct
	{
		bool isClusterOpen;
		uint64 beginOffset;
		uint32 size;
		uint64 pts;
		uint64 basePTS;
	} currentCluster;
	CMap<uint64, SCueEntry> cues;
	
	//meta seeking
	uint64 segmentOutputStreamOffset;
	struct
	{
		uint64 segmentInfoOffset;
		uint64 trackInfoOffset;
		uint64 cueingDataOffset;
	} metaSeekInfoOffsets;
	struct
	{
		uint64 segmentInfoOffset;
		uint64 trackInfoOffset;
		uint64 cueingDataOffset;
	} metaSeekInfoWriteOffsets;
	
	//Methods
	void BeginElement(EMatroskaId id);
	void ComputeTimeScales();
	void EndElement();
	void FinalizeMetaSeekInfo();
	uint64 PrepareMetaSeekEntry(EMatroskaId id);
	void PrepareMetaSeekInfo();
	void WriteAdditionalAudioStreamInfo(CAudioStream &refStream);
	void WriteCodecElement(AStream &refStream);
	void WriteCuePoints();
	void WriteEBMLUInt(uint64 value);
	void WriteId(EMatroskaId id);
	void WriteUInt(uint64 value);

	//Inline
	inline uint64 MapSeconds(uint64 seconds) const
	{
		return seconds / this->timeScale;
	}
	
	inline uint64 TransformPTS(uint64 pts, uint32 streamIndex) const
	{
		if(pts == UINT64_MAX)
			return UINT64_MAX;
		
		return pts / this->timeScale * this->GetStream(streamIndex)->timeScale;
	}
	
	inline void WriteASCIIElement(EMatroskaId id, const C8BitString &refString)
	{
		this->BeginElement(id);
		this->refOutput.WriteBytes(refString.GetC_Str(), refString.GetLength());
		this->EndElement();
	}

	inline void WriteFloatElement(EMatroskaId id, float64 value)
	{
		this->BeginElement(id);
		this->refOutput.WriteFloat64BE(value);
		this->EndElement();
	}
	
	inline void WriteUIntElement(EMatroskaId id, uint64 value)
	{
		this->BeginElement(id);
		this->WriteUInt(value);
		this->EndElement();
	}

	inline void WriteUTF8Element(EMatroskaId id, const CUTF8String &refString)
	{
		this->BeginElement(id);
		this->refOutput.WriteBytes(refString.GetC_Str(), refString.GetNumberOfElements());
		this->EndElement();
	}

public:
	//Constructor
	CMatroskaMuxer(const IFormat &refFormat, ASeekableOutputStream &refOutput);
	
	//Methods
	void Finalize();
	void WriteHeader();
	void WritePacket(const SPacket &refPacket);
};
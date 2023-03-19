#include <ACStdLib.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

/*
Sources:
https://www.matroska.org/technical/specs/index.html
*/

enum class EMatroskaType
{
	ASCII_String,
	Binary,
	Date,
	Float,
	Master,
	UInt, //1 to 8 byte
	UTF8
};

struct SElemInfo
{
	EMatroskaType type;
};

struct STrackInfo
{
	uint64 number;
	uint8 type;
	ECodecId codecId;

	struct
	{
		uint8 bitDepth;
		uint8 nChannels;
		float64 samplingFrequency;
	} audio;

	struct
	{
		bool isMS_BMPHeader;
		bool isIntegerPCM;
		bool isFloatPCM;
		uint64 offset;
		uint64 size;
	} codecPrivate;
};

class CMatroskaDemuxer : public ADemuxer
{
private:
	//Members
	CClusterIndex clusters;
	CMap<uint64, uint32> trackToStreamMap;
	struct
	{
		struct
		{
			uint64 timeCode;
			CArray<uint64> offsets;
			CArray<uint64> sizes;
		} currentCluster;
		
		STrackInfo currentTrack;
	} parserState;
	
	//Methods
	void AddStream();
	void BeginParseChilds(uint64 id);
	uint64 DecodeVariableLengthInteger(uint8 &refLength);
	void EndParseChilds(uint64 id);
	bool GetElementInfo(uint64 id, SElemInfo &refElemInfo);
	ECodecId MapCodecId(const C8BitString &refString);
	void ParseASCIIString(uint64 id, const C8BitString &refString);
	void ParseBinary(uint64 id, uint64 size);
	uint64 ParseElement();
	void ParseFloat(uint64 id, float64 value);
	void ParseUInt(uint64 id, uint64 value);

	//Inline
	inline uint64 PutLength(uint64 uncoded, uint8 length)
	{
		return uncoded | (1ULL << uint64((length-1) * 8 + (8 - length)));
	}

public:
	//Constructor
	CMatroskaDemuxer(const IFormat &refFormat, ASeekableInputStream &refInput);
	
	//Methods
	void ReadHeader();
	bool ReadPacket(SPacket &refPacket);
};
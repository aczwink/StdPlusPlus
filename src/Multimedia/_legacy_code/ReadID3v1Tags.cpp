//Class header
#include "ID3.h"
//Local
#include "../FourCC.h"
//Definitions
#define ID3_V1_SIZE 128

//ID3V1
static CString ID3V1_MapGenre(uint8 genre)
{
	//http://mpgedit.org/mpgedit/mpeg_format/mpeghdr.htm#MPEGTAG

	static const CString genres[] =
	{
		"Blues",
		"Classic Rock",
		"Country",
		"Dance",
		"Disco",
		"Funk",
		"Grunge",
		"Hip-Hop",
		"Jazz",
		"Metal",
		"New Age",
		"Oldies",
		"Other"
	};

	if(genre > 115)
		return "Unknown";
	
	ASSERT(genre < sizeof(genres) / sizeof(genres[0])); //TODO: until table is complete this needs to stay here
	
	return genres[genre];
}

static void ReadID3V1Tags(ASeekableInputStream &refInput, SMetaInfo &refMetaInfo)
{
	byte genre;
	uint64 offset;
	char buffer[31];
	byte tag[3];
	
	offset = refInput.GetCurrentOffset();
	if(refInput.GetRemainingBytes() > ID3_V1_SIZE)
	{
		refInput.SetCurrentOffset(refInput.GetSize() - ID3_V1_SIZE);
		refInput.ReadBytes(tag, 3);
		if(MemCmp(tag, "TAG", 3) == 0)
		{
			buffer[30] = 0;
			
			//title
			refInput.ReadBytes(buffer, 30);
			refMetaInfo.title = CString(buffer);
			
			//artist
			refInput.ReadBytes(buffer, 30);
			refMetaInfo.artist = CString(buffer);
			
			//album
			refInput.ReadBytes(buffer, 30);
			refMetaInfo.album = CString(buffer);
			
			//year
			refInput.ReadBytes(buffer, 4);
			buffer[4] = 0;
			refMetaInfo.year = (uint16)StringToUInt64(CString(buffer));

			//comment
			refInput.ReadBytes(buffer, 30);
			buffer[30] = 0;
			refMetaInfo.additionalTags["comment"] = CString(buffer);
			if(buffer[28] == 0 && buffer[29])
			{
				refMetaInfo.trackNumber = buffer[29];
			}

			//genre
			genre = refInput.ReadByte();
			refMetaInfo.additionalTags["genre"] = ID3V1_MapGenre(genre);
		}
	}
	
	refInput.SetCurrentOffset(offset);
}

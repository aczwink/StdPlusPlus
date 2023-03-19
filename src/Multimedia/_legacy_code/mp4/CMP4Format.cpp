//Class Header
#include "__CMP4Format.h"
//Local
#include "__CMP4Demuxer.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;

//Public Methods
CString CMP4Format::GetFileExtension() const
{
	return "mp4";
}

void CMP4Format::GetFormatInfo(SFormatInfo &refFormatInfo) const
{
	MemZero(&refFormatInfo, sizeof(refFormatInfo));
}

EContainerFormatId CMP4Format::GetId() const
{
	return CONTAINERFORMAT_ID_MP4;
}

CString CMP4Format::GetName() const
{
	return "MP4 / MPEG-4 Part 14";
}

EFormatMatch CMP4Format::MatchesFormat(byte *pBuffer, uint32 bufferSize) const
{
	//ISO IEC 14496-12 6.2.3 "The file type box ‘ftyp’ shall occur before any variable-length box (e.g. movie, free space, media data). Only a fixed-size box such as a file signature, if required, may precede it."
	
	if(bufferSize < 8)
		return FORMATMATCH_BUFFERTOOSMALL;

	if(MemCmp(((byte *)pBuffer)+4, "ftyp", 4) == 0)
		return FORMATMATCH_PERFECT;
	
	return FORMATMATCH_NO;
}

IDemuxer *CMP4Format::RequestDemuxerInstance() const
{
	return new CMP4Demuxer;
}

IMuxer *CMP4Format::RequestMuxerInstance() const
{
	return NULL;
}
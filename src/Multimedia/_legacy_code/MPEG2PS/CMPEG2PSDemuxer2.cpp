//Class Header
#include "__CMPEG2PSDemuxer.h"
//Global
#include <iostream>
//Local
#include "__CMPEGAudioParser.h"
#include "__CMPEGVideoParser.h"
#include "__CMPEG1VideoDecoder.h"
#include "__MPEG.h"
#include "CAudioStream.h"
#include "CInputContext.h"
#include "CVideoStream.h"
#include "SJCMMLib.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;

//Private Functions
void CMPEG2PSDemuxer::AddStream(const SPESHeaderInfo &refInfo, CInputContext &refCtx)
{
	uint32 index;
	
	//for mpeg1/2 audio streams streamId has bitmask 110X XXXX
	else if((refInfo.streamId & 0xE0) == 0xC0)
	{
		//This could be mp1, mp2 or mp3... parser will find out the codec
		pStream = new CAudioStream;
		pStream->SetParser(new CMPEGAudioParser);
	}
	else
	{
		ASSERT(false);
	}

	if(refInfo.pts != TIMESTAMP_INVALID)
	{
		pStream->SetStartTime(refInfo.pts);
	}
	
	index = refCtx.AddStream(pStream);
	this->streamMap.Insert(refInfo.streamId, index);
}

void CMPEG2PSDemuxer::ReadPESHeader(ASeekableInputStream &refInput, SPESHeaderInfo &refInfo) const
{
	byte startCode;
	byte startCodePrefix[3];
	/*
	This Function reads a PES Packet and skips other headers
	See 2.5.3.1 and following...
	*/
start:;
	if(startCode == MPEG2PS_STARTCODE_PROGRAMEND)
	{
		/*
		Indicates the end of a program stream... simply skip this
		if it is really the end then next step will hit the end and jump out
		*/
		goto start;
	}
	else if(!(startCode == MPEG2PS_PADDINGSTREAM_STREAMID || (startCode & 0xE0) == 0xE0 || (startCode & 0xE0) == 0xC0))
	{
		//skip
		goto start;
	}
}
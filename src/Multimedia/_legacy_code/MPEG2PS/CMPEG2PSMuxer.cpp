/*//Class Header
#include "__CMPEG2PSMuxer.h"
//Global
#include <iostream>
//Local
#include "__MPEG.h"
#include "COutputContext.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;

//Constructor
CMPEG2PSMuxer::CMPEG2PSMuxer()
{
	this->packetSize = 2048; //default
	this->nVideoStreams = 0;
}

//Private Functions
bool CMPEG2PSMuxer::OutputPacket(COutputContext &refCtx)
{
	uint32 packetSize, headerSize;
	ASeekableOutputStream *pOutput;

	SStreamInfo &refStreamInfo = this->streamInfo[0]; //get stream info for stream
	
	packetSize = this->packetSize;
	pOutput = refCtx.GetOutputStream();

	packetSize -= this->PutPackHeader(pOutput);
	packetSize -= this->PutSystemHeader(pOutput);
	headerSize = 6; //PES Header size
	headerSize += 10; //pts and dts
	
	//put the startcode prefix first
	pOutput->WriteByte(0);
	pOutput->WriteByte(0);
	pOutput->WriteByte(MPEG2PS_STARTCODE_PREFIX);
	//put startcode
	pOutput->WriteByte(0xE0); //stream id... of course buggy here

	//put pes packet length (big endian)
	pOutput->WriteUInt16(ENDIAN_SWAP16(packetSize));

	//write data

	return false;
}

uint16 CMPEG2PSMuxer::PutPackHeader(ASeekableOutputStream *pOutput)
{
	//put the startcode prefix first
	pOutput->WriteByte(0);
	pOutput->WriteByte(0);
	pOutput->WriteByte(MPEG2PS_STARTCODE_PREFIX);
	//put startcode
	pOutput->WriteByte(MPEG2PS_STARTCODE_PACKHEADER);

	/*
	next byte (as bitmap):
		7 -> 0
		6 -> 0
		5 -> 1
		4 -> 0
		3 -> SCR 32
		2 -> SCR 31
		1 -> SCR 30
		0 -> 1
	*/
	//pOutput->WriteByte(0x21);

	/*
	next byte: SCR 29 to SCR 22
	*/
	//pOutput->WriteByte(0);

	/*
	next byte:
		bits 7 to 1 -> SCR 21 to SCR 15
		bit 0 -> 1
	*/
	//pOutput->WriteByte(1);

	/*
	next byte: SCR 14 to SCR 7
	*/
	//pOutput->WriteByte(0);

	/*
	next byte:
		bits 7 to 1 -> SCR 6 to SCR 0
		bit 0 -> 1
	*/
	//pOutput->WriteByte(1);

	/*
	next byte:
		bit 7 -> 1
		bit 6 to bit 0 -> program mux rate 21 to program mux rate 15
	*/
	//pOutput->WriteByte(0x80);

	/*
	next byte: program mux rate 14 to program mux rate 7
	*/
	//pOutput->WriteByte(0);

	/*
	next byte:
		bits 7 to 1 -> program mux rate 6 to program mux rate 0
		bit 0 -> 1
	*/
	//pOutput->WriteByte(1);

/*	return 12;
}

uint16 CMPEG2PSMuxer::PutSystemHeader(ASeekableOutputStream *pOutput)
{
	byte b10;
	uint16 headerSize;

	//put the startcode prefix first
	pOutput->WriteByte(0);
	pOutput->WriteByte(0);
	pOutput->WriteByte(MPEG2PS_STARTCODE_PREFIX);
	//put startcode
	pOutput->WriteByte(MPEG2PS_STARTCODE_SYSTEMHEADER);

	//next 2 bytes: header length (big endian)
	pOutput->WriteUInt16(0);

	/*
	next byte:
		bit 7 -> 1
		bits 6 to 0 -> rate bound 21 to rate bound 15
	*/
	//pOutput->WriteByte(0x80);

	/*
	next byte: rate bound bits 14 to 7
	*/
	//pOutput->WriteByte(0);

	/*
	next byte:
		bits 7 to 1 -> rate bount bits 6 to 0
		bit 0 -> 1
	*/
	//pOutput->WriteByte(1);

	/*
	next byte:
		bits 7 to 2 -> audio bound
		bit 1 -> fixed flag
		bit 0 -> constrained system parameter program stream flag
	*/
	//pOutput->WriteByte(0);

	/*
	next byte:
		bit 7 -> audio lock flag
		bit 6 -> video lock flag
		bit 5 -> 1
		bits 4 to 0 -> video bound; number of video streams in the program stream
	*/
	/*b10 = 0x20; //set bit 5
	b10 |= this->nVideoStreams & 0x1F;

	pOutput->WriteByte(b10);

	//next byte: reserved, all bits are set
	pOutput->WriteByte(0xFF);

	headerSize = 12; //so far
	
	/*
	put stream bound entries
	*/
	
	/*return headerSize;
}

//Public Functions
void CMPEG2PSMuxer::Finalize(COutputContext &refCtx) const
{
	ASSERT(0);
}

void CMPEG2PSMuxer::WriteHeader(COutputContext &refCtx)
{
	byte videoId;

	//MPEG 2 PS has no header, nothing to write

	videoId = MPEG2PS_STREAMID_VIDEO_FIRST;

	repeat(refCtx.GetNumberOfStreams(), i)
	{
		switch(refCtx.GetStream(i)->GetType())
		{
		case DATATYPE_VIDEO:
			this->streamInfo[i].streamId = videoId++;
			this->nVideoStreams++;
			break;
		}
	}
}

void CMPEG2PSMuxer::WritePacket(COutputContext &refCtx, const SPacket *pPacket)
{
	SStreamInfo &refStreamInfo = this->streamInfo[pPacket->streamIndex];

	refStreamInfo.buffer.WriteBytes(pPacket->pData, pPacket->size);

	while(this->OutputPacket(refCtx));
}*/
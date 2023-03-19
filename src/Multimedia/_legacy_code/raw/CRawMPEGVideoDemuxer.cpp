//Class Header
#include "__CRawMPEGVideoDemuxer.h"
//Global
#include <iostream>
//Local
#include "__CMPEG1VideoDecoder.h"
#include "__CMPEGVideoParser.h"
#include "__MPEG.h"
#include "CInputContext.h"
#include "CVideoStream.h"
#include "SJCMMLib.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;
//Definitions
#define RAW_VIDEO_PACKETSIZE 2048

//Public Functions
void CRawMPEGVideoDemuxer::ReadHeader(CInputContext &refCtx)
{
	AStream *pStream;

	pStream = new CVideoStream;
	pStream->SetCodec(GetCodec(CODEC_ID_MPEG1VIDEO));
	pStream->SetParser(new CMPEGVideoParser);
	pStream->SetStartTime(0);
	pStream->SetTimeBase(CFraction32(1, MPEG_SCR));
	
	refCtx.AddStream(pStream);
}

EResult CRawMPEGVideoDemuxer::ReadPacket(SPacket &refPacket, CInputContext &refCtx)
{
	uint32 left;
	ASeekableInputStream *pInput;

	pInput = refCtx.GetInputStream();

	if(pInput->HitEnd() || !pInput->GetRemainingBytes())
		return SJCMMLIB_ENDOFDATA;

	left = RAW_VIDEO_PACKETSIZE;
	if(pInput->GetRemainingBytes() < left)
		left = pInput->GetRemainingBytes();

	refPacket.pData = (byte *)malloc(left);
	refPacket.size = left;
	refPacket.streamIndex = 0;

	pInput->ReadBytes(refPacket.pData, left);

	return SJCMMLIB_OK;
}

void CRawMPEGVideoDemuxer::Release()
{
}
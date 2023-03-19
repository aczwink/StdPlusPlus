//Class Header
#include "__CRawAudioAC3Demuxer.h"
//Local
#include "__CAC3Parser.h"
#include "CAudioStream.h"
#include "CInputContext.h"
#include "SJCMMLib.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;
//Definitions
#define RAWPACKET_SIZE 1024

//Public methods
void CRawAudioAC3Demuxer::ReadHeader(CInputContext &refCtx)
{
	CAudioStream *pStream;

	pStream = new CAudioStream;
	pStream->SetCodec(GetCodec(CODEC_ID_AC3));
	pStream->SetParser(new CAC3Parser);
	pStream->SetStartTime(0);
	pStream->SetTimeBase(CFraction32(1, 14112000)); //least common multiple of all ac3 audio sample rates
	
	refCtx.AddStream(pStream);
}

EResult CRawAudioAC3Demuxer::ReadPacket(SPacket &refPacket, CInputContext &refCtx)
{
	uint16 left;
	AInputStream *pInput;
	
	pInput = refCtx.GetInputStream();

	if(pInput->HitEnd() || !pInput->GetRemainingBytes())
		return SJCMMLIB_ENDOFDATA;
	left = RAWPACKET_SIZE;
	if(pInput->GetRemainingBytes() < left)
		left = pInput->GetRemainingBytes();

	refPacket.pData = (byte *)MemAlloc(left);
	refPacket.pts = TIMESTAMP_INVALID;
	refPacket.size = left;
	refPacket.streamIndex = 0;

	pInput->ReadBytes(refPacket.pData, left);

	return SJCMMLIB_OK;
}

void CRawAudioAC3Demuxer::Release()
{
}
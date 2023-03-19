//Class Header
#include "CInputContext.h"
//Global
#include <iostream>
//Local
#include "__Math.h"
#include "__globals.h"
#include "SJCMMLib.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;

//Constructor
CInputContext::CInputContext()
{
	this->pFormat = NULL;
	this->pDemux = NULL;
	this->pInput = NULL;
}

//Destructor
CInputContext::~CInputContext()
{
	repeat(this->streams.GetNoOfElements(), i)
	{
		this->streams[i]->Release();
		delete this->streams[i];
	}
	if(this->pDemux)
	{
		this->pDemux->Release();
		delete this->pDemux;
	}
	this->streams.Release();
}

//Private Functions
bool CInputContext::AllInfoIsAvailable(uint32 streamIndex)
{
	if(!pDecoder->GetBitRate()) //decoder has no bit rate
		return false;

	if(pStream->GetTimeBase() == CFraction32())
		return false;

	//check audio specific things
	if(pStream->GetType() == DATATYPE_AUDIO)
	{
		AAudioDecoder *pADecoder;

		pADecoder = (AAudioDecoder *)pDecoder;

		if(!pADecoder->GetNumberOfChannels()) //we don't have a channel count
			return false;
	}
	
	return true;
}

bool CInputContext::EstimateTimingInfoFromBitrate()
{
	AStream *pStream;
	ADecoder *pDecoder;
	
	//Get the complete data bit rate
	repeat(this->GetNumberOfStreams(), i)
	{
		pStream = this->GetStream(i);
		pDecoder = pStream->GetDecoder();

		if(!pDecoder)
			continue; //no decoder found... skip this

		//unfortunately this can be 0 :S		
		this->bitRate += pDecoder->GetBitRate();
	}

	//If we have no bit rate.. there is nothing more to be found out...
	if(!this->bitRate)
		return false;
	
	//Check if all streams have a duration and bit rate
	repeat(this->GetNumberOfStreams(), i)
	{
		pStream = this->GetStream(i);

		if(pStream->GetDuration() == TIMESTAMP_INVALID)
		{
			//No duration set... estimate by bit rate

			const CFraction32 &refTimeBase = pStream->GetTimeBase();

			if(refTimeBase.GetNumerator() == 0)
				continue; //no time base :S
			
			pStream->SetDuration((uint32)Rescale(8 * this->pInput->GetSize(), refTimeBase.GetDenominator(), this->bitRate * refTimeBase.GetNumerator()));
		}
	}

	return true;
}

bool CInputContext::HasFullTimingInfo(bool allStreams)
{
	repeat(this->GetNumberOfStreams(), i)
	{
		if(!this->HasFullTimingInfo(i))
		{
			return false;
		}

		if(!allStreams)
			return true;
	}

	return true;
}

bool CInputContext::HasFullTimingInfo(uint32 streamIndex)
{
	AStream *pStream;

	pStream = this->GetStream(streamIndex);

	if(pStream->GetStartTime() == TIMESTAMP_INVALID)
		return false;
	if(pStream->GetDuration() == TIMESTAMP_INVALID)
		return false;
	
	return true;
}

//Public Functions
bool CInputContext::FindStreamInfo()
{
	uint8 i;
	bool ret, infoFound;
	uint32 currentOffset;
	EResult result;
	AStream *pStream;
	SFormatInfo formatInfo;
	SPacket probePacket;
	
	ret = true;
	infoFound = false;
	result = SJCMMLIB_OK;
	
	//save the offset where we are, so we can restore at the end
	currentOffset = this->pInput->GetCurrentOffset();

	this->pFormat->GetFormatInfo(formatInfo);
	
	//If the format has no header we need to decode some packets to find the streams
	if(!formatInfo.hasHeader)
	{
		while(this->pInput->GetCurrentOffset() - currentOffset < formatInfo.maxProbeSize && result == SJCMMLIB_OK)
		{
			result = this->ReadFrame(probePacket);
			
			if(result == SJCMMLIB_OK)
			{
				this->ExtractInfo(probePacket);
				free(probePacket.pData);
			}
		}
	}
	
	//Read packets until all info is found
	while(this->pInput->GetCurrentOffset() - currentOffset < formatInfo.maxProbeSize && )
	{
		if(result == SJCMMLIB_OK)
		{
			this->ExtractInfo(probePacket);
			free(probePacket.pData);
		}
	}

	if(this->GetNumberOfStreams() == 0)
		return false; //no streams found

	//Update timing info
	if(formatInfo.estimateDurationByReadingLastPacket)
	{
		this->EstimateTimingInfoFromPackets();
	}
	else if(this->HasFullTimingInfo(false))
	{
		//at least one stream has all info... use it for all others
		this->UpdateTimingInfo();

		for(i = 0; i < this->GetNumberOfStreams(); i++)
		{
			pStream = this->GetStream(i);

			if(!this->HasFullTimingInfo((uint32)i))
			{
				pStream->SetStartTime(Rescale(this->startTime, this->GetInternalTimeBase(), pStream->GetTimeBase()));
				pStream->SetDuration(Rescale(this->duration, this->GetInternalTimeBase(), pStream->GetTimeBase()));
			}
		}
	}
	else
	{
		if(!this->EstimateTimingInfoFromBitrate())
			return false;
	}
	
	this->UpdateTimingInfo();

	if(ret)
	{
		//check if all timing info is available
		for(i = 0; i < this->GetNumberOfStreams(); i++)
		{
			if(this->GetStream(i)->GetDuration() == TIMESTAMP_INVALID)
				return false;
			if(this->GetStream(i)->GetStartTime() == TIMESTAMP_INVALID)
				return false;
		}
	}
	
	//restore everything
	//this->pInput->SetCurrentOffset(currentOffset);
	
	return ret;
}
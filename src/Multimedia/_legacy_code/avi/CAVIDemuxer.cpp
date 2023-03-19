//Class Header
#include "__CAVIDemuxer.h"
//Global
#include <Windows.h>
#include <aviriff.h>
//Local
#include "__Bmp.h"
#include "__Math.h"
#include "__WAVE.h"
#include "CAudioStream.h"
#include "CInputContext.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;
//Definitions
#define AVI_AVIH_CHUNKID 0x68697661
#define AVI_IDX1_CHUNKID 0x31786469
#define AVI_INDX_CHUNKID 0x78646E69
#define AVI_LIST_CHUNKID 0x5453494C
#define AVI_STRF_CHUNKID 0x66727473
#define AVI_STRH_CHUNKID 0x68727473

#define AVI_LIST_MOVI 0x69766F6D

#define AVI_SUBCHUNK_AUDS_CHUNKID 0x73647561
#define AVI_SUBCHUNK_VIDS_CHUNKID 0x73646976

//Constructor
CAVIDemuxer::CAVIDemuxer()
{
	this->isInterleaved = true;
	this->indexLoaded = false;
	this->streamIndex = -1;
	this->moviListOffset = 0;
}

//Private Methods
void CAVIDemuxer::GuessInterleave(CInputContext &refCtx)
{
	uint32 currentOffset, nIndexEntries, firstEnd, lastStart;
	ASeekableInputStream *pInput;

	pInput = refCtx.GetInputStream();
	currentOffset = pInput->GetCurrentOffset();
	firstEnd = UINT32_MAX;
	lastStart = 0;

	repeat(refCtx.GetNumberOfStreams(), i)
	{
		const CArray<SIndexEntry> &refIndex = this->index.GetStreamIndices(i);

		nIndexEntries = refIndex.GetNoOfElements();
		
		if(!nIndexEntries)
			continue;
		if(nIndexEntries >= 2)
		{
			uint32 offset, size;

			offset = refIndex[0].offset;
			pInput->SetCurrentOffset(offset + 4);
			pInput->ReadUInt32(size);
			if(offset + size > refIndex[1].offset)
				lastStart = UINT32_MAX;
		}

		if(refIndex[0].offset > lastStart)
			lastStart = refIndex[0].offset;
		if(refIndex[nIndexEntries-1].offset < firstEnd)
			firstEnd = refIndex[nIndexEntries-1].offset;
	}
	pInput->SetCurrentOffset(currentOffset);
	if(lastStart > firstEnd)
		this->isInterleaved = false;
}

void CAVIDemuxer::ReadIndxChunk(ASeekableInputStream *pInput, uint32 frameNumber)
{
	byte indexType;
	uint16 convertedStreamIndex;
	char streamIndex[3];
	uint32 entriesInUse;
	uint64 base, offset;

	streamIndex[2] = 0;

	pInput->Skip(2); //longs per entry
	pInput->Skip(1); //index sub type
	indexType = pInput->ReadByte();
	pInput->ReadUInt32(entriesInUse);
	pInput->ReadBytes(&streamIndex, 2);
	pInput->Skip(2); //chunkId
	pInput->ReadUInt64(base);
	pInput->Skip(4); //reserved

	convertedStreamIndex = atoi(streamIndex);

	repeat(entriesInUse, i)
	{
		if(indexType)
		{
			uint32 entryOffset;
			uint32 size;
			bool isKeyFrame;

			pInput->ReadUInt32(entryOffset);
			pInput->ReadUInt32(size);

			offset = entryOffset + base - 8;
			isKeyFrame = (size & 0x80000000) == 0;
			size &= 0x7FFFFFFF;

			this->index.Add(convertedStreamIndex, (uint32)offset, size, this->streamInfo[convertedStreamIndex].currentTimeStamp, isKeyFrame);
		}
		else
		{
			uint32 duration, currentOffset;

			pInput->ReadUInt64(offset);
			pInput->Skip(4); //size
			pInput->ReadUInt32(duration);

			currentOffset = pInput->GetCurrentOffset();
			pInput->SetCurrentOffset(offset+8);
			this->ReadIndxChunk(pInput, frameNumber);
			frameNumber += duration;
			pInput->SetCurrentOffset(currentOffset);
		}
	}
}

bool CAVIDemuxer::Sync(CInputContext &refCtx)
{
	uint32 i, sync, size, fileSize, streamIndex;
	uint32 d[8];
	ASeekableInputStream *pInput;

	pInput = refCtx.GetInputStream();
	fileSize = pInput->GetSize();

start_sync:;
	MemSet(d, 0xFF, sizeof(d));

	for(i = sync = pInput->GetCurrentOffset(); pInput->GetRemainingBytes() > 0; i++)
	{
		MemCopy(d, &d[1], sizeof(d) - sizeof(d[7]));
		d[7] = pInput->ReadByte();
		size = d[4] + (d[5] << 8) + (d[6] << 16) + (d[7] << 24);
		streamIndex = -1;
		if(isdigit(d[2]) && isdigit(d[3]))
		{
			char streamIndexUnconverted[3];

			streamIndexUnconverted[2] = 0;
			streamIndexUnconverted[0] = d[2];
			streamIndexUnconverted[1] = d[3];
			streamIndex = atoi(streamIndexUnconverted);
		}
		if(i + size > fileSize || d[0] > 127)
			continue;

		if(
			//ix##
			(d[0] == 'i' && d[1] == 'x' && streamIndex < refCtx.GetNumberOfStreams())
			||
			//JUNK
			(d[0] == 'J' && d[1] == 'U' && d[2] == 'N' && d[3] == 'K')
			//idx1
			||
			(d[0] == 'i' && d[1] == 'd' && d[2] == 'x' && d[3] == '1')
		  )
		{
			pInput->Skip(size);
			goto start_sync;
		}

		if(isdigit(d[0]) && isdigit(d[1]))
		{
			char streamIndexUnconverted[3];

			streamIndexUnconverted[2] = 0;
			streamIndexUnconverted[0] = d[0];
			streamIndexUnconverted[1] = d[1];
			streamIndex = atoi(streamIndexUnconverted);
		}

		//##dc/##wb
		if(streamIndex < refCtx.GetNumberOfStreams())
		{
			this->streamIndex = streamIndex;
			this->streamInfo[streamIndex].packetSize = size + 8;
			this->streamInfo[streamIndex].remainingSize = size;

			return true;
		}
	}

	return false;
}

//Public Methods
void CAVIDemuxer::ReadHeader(CInputContext &refCtx)
{
	uint32 chunkId, chunkSize, nextChunkOffset, endOfListOffset, listId;
	ASeekableInputStream *pInput;
	AStream *pStream;

	pInput = refCtx.GetInputStream();

	pInput->Skip(12); //"RIFF", chunkSize, "AVI "

	while(true)
	{
		if(!pInput->GetRemainingBytes())
			break;
		
		pInput->ReadUInt32(chunkId);
		pInput->ReadUInt32(chunkSize);
		nextChunkOffset = pInput->GetCurrentOffset() + chunkSize;

		switch(chunkId)
		{
		case AVI_AVIH_CHUNKID:
			{
				uint32 flags;

				pInput->Skip(12);
				pInput->ReadUInt32(flags);
				pInput->Skip(40);

				if(flags & AVIF_MUSTUSEINDEX)
				{
					this->isInterleaved = false;
				}
			}
			break;
		case AVI_LIST_CHUNKID:
			endOfListOffset = nextChunkOffset;
			pInput->ReadUInt32(listId);

			switch(listId)
			{
			case AVI_LIST_MOVI:
				this->moviListOffset = pInput->GetCurrentOffset()-4;
				pInput->SetCurrentOffset(nextChunkOffset);
				break;
			}
			break;
		case AVI_IDX1_CHUNKID:
			{
				bool isKeyFrame;
				uint16 contentType;
				char streamIndex[3];
				uint32 nIndexEntries, flags, convertedStreamIndex, offset, size;

				if(this->indexLoaded)
				{
					//already read an index
					pInput->SetCurrentOffset(nextChunkOffset);
					break;
				}

				nIndexEntries = chunkSize / 16; //16 = sizeof an index entry
				streamIndex[2] = 0;
				
				repeat(nIndexEntries, i)
				{
					pInput->ReadBytes(&streamIndex, 2);
					pInput->ReadUInt16(contentType);
					pInput->ReadUInt32(flags);
					pInput->ReadUInt32(offset);
					pInput->ReadUInt32(size);

					isKeyFrame = (flags & AVIIF_KEYFRAME) != 0;
					convertedStreamIndex = atoi(streamIndex);
					offset += this->moviListOffset;
					if(size)
					{
						this->index.Add(convertedStreamIndex, offset, size, this->streamInfo[convertedStreamIndex].currentTimeStamp, isKeyFrame);
					}
					this->streamInfo[convertedStreamIndex].currentTimeStamp += (this->streamInfo[convertedStreamIndex].sampleSize ? size : 1);
				}
				this->indexLoaded = true;
			}
			break;
		case AVI_INDX_CHUNKID:
			{
				this->ReadIndxChunk(pInput, 0);
				pInput->SetCurrentOffset(nextChunkOffset);
				this->indexLoaded = true;
			}
			break;
		case AVI_STRF_CHUNKID: //Stream Format
			{
				switch(pStream->GetType())
				{
				case DATATYPE_AUDIO:
					{
						uint16 blockAlign, bitsPerSample;

						ReadWaveHeader(*pInput, *pStream, chunkSize, blockAlign, bitsPerSample);
					}
					break;
				case DATATYPE_VIDEO:
					{
						bool isBottomUp;
						
						ReadBmpHeader(isBottomUp, *pInput, *(CVideoStream *)pStream);
					}
					break;
				}
			}
			break;
		case AVI_STRH_CHUNKID: //Stream Header
			{
				uint32 subChunkId, scale, rate, start, nFrames, sampleSize, streamIndex;
				CFraction32 timeBase;
				
				pInput->ReadUInt32(subChunkId);
				pInput->Skip(16);
				pInput->ReadUInt32(scale);
				pInput->ReadUInt32(rate);
				pInput->ReadUInt32(start);
				pInput->ReadUInt32(nFrames);
				pInput->Skip(8);
				pInput->ReadUInt32(sampleSize);
				pInput->Skip(8);
				
				switch(subChunkId)
				{
				case AVI_SUBCHUNK_AUDS_CHUNKID:
					refCtx.AddStream(new CAudioStream);
					break;
				case AVI_SUBCHUNK_VIDS_CHUNKID:
					refCtx.AddStream(new CVideoStream);
					break;
				}
				streamIndex = refCtx.GetNumberOfStreams()-1;
				pStream = refCtx.GetStream(streamIndex);
				MemZero(&this->streamInfo[streamIndex], sizeof(this->streamInfo[streamIndex]));

				this->streamInfo[streamIndex].start = start * MAX(1, sampleSize);
				this->streamInfo[streamIndex].sampleSize = sampleSize;
				this->streamInfo[streamIndex].currentTimeStamp = start;
				this->streamInfo[streamIndex].currentTimeStamp *= MAX(1, sampleSize);
				this->streamInfo[streamIndex].currentFrame = this->streamInfo[streamIndex].currentTimeStamp;

				timeBase.Init(scale, rate);
				timeBase.Reduce();
				pStream->SetStartTime(start);
				pStream->SetTimeBase(timeBase);
				pStream->SetDuration(nFrames);
			}
			break;
		default:
			pInput->SetCurrentOffset(nextChunkOffset + (chunkSize & 1)); //skip
			break;
		}
	}

	pInput->SetCurrentOffset(this->moviListOffset+4);
	this->GuessInterleave(refCtx);
}

EResult CAVIDemuxer::ReadPacket(SPacket &refPacket, CInputContext &refCtx)
{
	uint32 size;
	ASeekableInputStream *pInput;

	MemZero(&refPacket, sizeof(refPacket));
	pInput = refCtx.GetInputStream();

	if(!this->isInterleaved)
	{
		AStream *pStream;
		int32 index, bestStreamIndex;
		uint64 lastTimeStamp, timeStamp, bestTimeStamp;
		CFraction32 c;
		SIndexEntry indexEntry;

		bestStreamIndex = -1;		
		bestTimeStamp = UINT64_MAX;

		repeat(refCtx.GetNumberOfStreams(), i)
		{
			const CArray<SIndexEntry> &refIndex = this->index.GetStreamIndices(i);

			pStream = refCtx.GetStream(i);
			
			timeStamp = this->streamInfo[i].currentFrame;
			lastTimeStamp = refIndex[refIndex.GetNoOfElements()-1].timestamp;
			if(!this->streamInfo[i].remainingSize && timeStamp > lastTimeStamp)
				continue;

			c.Init(MAX(1, this->streamInfo[i].sampleSize), /*TIME_BASE*/1);

			timeStamp = Rescale(timeStamp, pStream->GetTimeBase(), c);
			if(timeStamp < bestTimeStamp)
			{
				bestTimeStamp = timeStamp;
				bestStreamIndex = i;
			}
		}
		if(bestStreamIndex == -1)
			return SJCMMLIB_ENDOFDATA;
		
		bestTimeStamp = this->streamInfo[bestStreamIndex].currentFrame;
		if(this->streamInfo[bestStreamIndex].remainingSize)
		{
			index = this->index.Find(bestStreamIndex, bestTimeStamp, true);
			indexEntry = this->index.GetStreamIndices(bestStreamIndex)[index];
		}
		else
		{
			index = this->index.Find(bestStreamIndex, bestTimeStamp);
			indexEntry = this->index.GetStreamIndices(bestStreamIndex)[index];
			this->streamInfo[bestStreamIndex].currentFrame = (uint32)indexEntry.timestamp;
		}

		pInput->SetCurrentOffset(indexEntry.offset + this->streamInfo[bestStreamIndex].packetSize - this->streamInfo[bestStreamIndex].remainingSize + 8);

		if(!this->streamInfo[bestStreamIndex].remainingSize)
		{
			this->streamInfo[bestStreamIndex].packetSize = indexEntry.size;
			this->streamInfo[bestStreamIndex].remainingSize = indexEntry.size;
		}

		this->streamIndex = bestStreamIndex;
	}

resync:;
	if(this->streamIndex >= 0)
	{
		SAVIStreamInfo &refStreamInfo = this->streamInfo[this->streamIndex];

		size = refStreamInfo.sampleSize;
		if(size <= 1)
		{
			size = UINT32_MAX;
		}
		if(size < 32)
		{
			//avoid tiny packets for raw PCM
			size *= 1024;
		}
		
		if(size > refStreamInfo.remainingSize)
			size = refStreamInfo.remainingSize;

		refPacket.pData = (byte *)malloc(size);
		pInput->ReadBytes(refPacket.pData, size);
		refPacket.size = size;
		refPacket.streamIndex = this->streamIndex;

		refStreamInfo.currentFrame += (refStreamInfo.sampleSize ? refPacket.size : 1);

		refStreamInfo.remainingSize -= size;
		if(!refStreamInfo.remainingSize)
		{
			this->streamIndex = -1;
			refStreamInfo.packetSize = 0;
		}

		return SJCMMLIB_OK;
	}
	
	if(this->Sync(refCtx))
		goto resync;

	return SJCMMLIB_ENDOFDATA;
}

void CAVIDemuxer::Release()
{
	this->index.Release();
	this->streamInfo.Release();
}
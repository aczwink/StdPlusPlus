/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of Std++.
 *
 * Std++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Std++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Std++.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include <Std++/Multimedia/MediaPlayer.hpp>
//Local
#include <Std++/Multimedia/AudioFrame.hpp>
#include <Std++/Multimedia/Encoder.hpp>
#include <Std++/Multimedia/VideoFrame.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructors
DecoderThread::DecoderThread(StdXX::Multimedia::MediaPlayer *player)
	: player(player), shutdown(false), work(false), working(false), decoderContext(nullptr), encoderContext(nullptr), streamIndex(Unsigned<uint32>::Max())
{
}

//Destructor
DecoderThread::~DecoderThread()
{
	this->FlushInputQueue();
	this->FlushOutputQueue();

	delete this->encoderContext;
}

//Private methods
UniquePointer<IPacket> DecoderThread::GetNextInputPacket()
{
	this->inputPacketQueueLock.Lock();
	while(this->inputPacketQueue.IsEmpty())
	{
		this->inputPacketQueueSignal.Wait(this->inputPacketQueueLock);

		if(this->shutdown)
		{
			this->inputPacketQueueLock.Unlock();
			return nullptr;
		}
		if(!this->work)
		{
			this->inputPacketQueueLock.Unlock();
			return nullptr;
		}
	}
	UniquePointer<IPacket> packet = this->inputPacketQueue.PopFront();
	this->inputPacketQueueSignal.Signal();
	this->inputPacketQueueLock.Unlock();

	return packet;
}

void DecoderThread::Run()
{
	while(true)
	{
		//check if kill thread
		if(this->shutdown)
			break;

		//wait for work
		if(this->WaitForWork())
			continue;

		//check if we can work
		if(!this->decoderContext)
		{
			this->Stop();
			continue;
		}

		//lets work
		this->working = true;

		if(this->decoderContext->IsFrameReady())
		{
			Frame *frame = this->decoderContext->GetNextFrame();

			//resample
			if (this->requireResampling)
			{
				Frame* resampledFrame;
				if (this->pixmapResampler.IsNull())
				{
					//audio
					const AudioFrame* audioFrame = dynamic_cast<const AudioFrame*>(frame);
					const AudioBuffer* srcBuffer = audioFrame->GetAudioBuffer();

					const AudioStream* sourceStream = dynamic_cast<const AudioStream*>(this->player->Demuxer().GetStream(this->streamIndex));
					const AudioStream& destStream = dynamic_cast<const AudioStream&>(*this->encodingStream);

					resampledFrame = new AudioFrame(srcBuffer->Resample(*sourceStream->sampleFormat, *destStream.sampleFormat));
				}
				else
				{
					const VideoFrame* videoFrame = dynamic_cast<const VideoFrame*>(frame);

					Pixmap* resampledPixmap = this->pixmapResampler->Run(*videoFrame->GetPixmap());

					//video
					resampledFrame = new VideoFrame(resampledPixmap);
				}

				resampledFrame->pts = frame->pts;

				delete frame;
				frame = resampledFrame;
			}

			//encode as the desired playback format
			this->encoderContext->Encode(*frame);
			while(this->encoderContext->IsPacketReady())
			{
				//this packet is ready to be played
				this->AddOutputPacket(this->encoderContext->GetNextPacket());
			}

			delete frame;

			continue; //go to begin
		}

		//We have no more frames for playback... decode new ones
		UniquePointer<IPacket> packet = this->GetNextInputPacket();
		if(!packet.IsNull())
		{
			this->decoderContext->Decode(*packet);
		}
	}

	this->working = false;
}

bool DecoderThread::WaitForWork()
{
	//wait for the instruction to work
	if(!this->work)
	{
		this->working = false;
		this->workLock.Lock();
		this->workSignal.Wait(this->workLock);
		this->workLock.Unlock();
		if(this->work)
		{
			if(this->streamIndex != Unsigned<uint32>::Max())
				this->decoderContext = const_cast<Stream *>(this->player->Demuxer().GetStream(this->streamIndex))->GetDecoderContext();
		}

		return true;
	}

	return false;
}

//Public methods
void DecoderThread::Flush()
{
	if(this->decoderContext)
		this->decoderContext->Reset();

	this->FlushInputQueue();
	this->FlushOutputQueue();
}

void DecoderThread::FlushInputQueue()
{
	this->inputPacketQueueLock.Lock();
	this->inputPacketQueue.Release();
	this->inputPacketQueueLock.Unlock();
}

void DecoderThread::FlushOutputQueue()
{
	this->outputPacketQueueLock.Lock();
	while(!this->outputPacketQueue.IsEmpty())
		delete this->outputPacketQueue.PopFront();
	this->outputPacketQueueLock.Unlock();
}

void DecoderThread::SetStreamIndex(uint32 streamIndex)
{
	this->streamIndex = streamIndex;

	const Stream* sourceStream = this->player->Demuxer().GetStream(streamIndex);
	CodingFormatId codingFormatId;
	switch (sourceStream->codingParameters.dataType)
	{
	case DataType::Audio:
	{
		const AudioStream* audioSrcStream = dynamic_cast<const AudioStream*>(sourceStream);

		AudioStream *audioStream = new AudioStream;
		audioStream->sampleFormat = AudioSampleFormat(audioSrcStream->sampleFormat->nChannels, AudioSampleType::S16, false);

		this->encodingStream = audioStream;
		codingFormatId = CodingFormatId::PCM_S16LE;

		this->requireResampling = *audioSrcStream->sampleFormat != *audioStream->sampleFormat;
	}
	break;
	case DataType::Video:
	{
		const VideoStream* videoSrcStream = dynamic_cast<const VideoStream*>(sourceStream);

		VideoStream* videoStream = new VideoStream;
		videoStream->pixelFormat = PixelFormat(NamedPixelFormat::RGB_24);
		videoStream->codingParameters.video.size = videoSrcStream->codingParameters.video.size;

		if (*videoSrcStream->pixelFormat != *videoStream->pixelFormat)
		{
			this->pixmapResampler = new ComputePixmapResampler(videoSrcStream->codingParameters.video.size, *videoSrcStream->pixelFormat);
			this->pixmapResampler->ChangePixelFormat(*videoStream->pixelFormat);
		}

		this->encodingStream = videoStream;
		codingFormatId = CodingFormatId::RawVideo;

		this->requireResampling = !this->pixmapResampler.IsNull();
	}
	break;
	}
	this->encodingStream->timeScale = sourceStream->timeScale;
	this->encoderContext = CodingFormat::GetCodingFormatById(codingFormatId)->GetBestMatchingEncoder()->CreateContext(*this->encodingStream);
}

void DecoderThread::Shutdown()
{
	this->shutdown = true;
	this->work = false;

	this->workLock.Lock();
	this->workSignal.Signal();
	this->workLock.Unlock();

	this->inputPacketQueueLock.Lock();
	this->inputPacketQueueSignal.Signal();
	this->inputPacketQueueLock.Unlock();

	this->outputPacketQueueLock.Lock();
	this->outputPacketQueueSignal.Signal();
	this->outputPacketQueueLock.Unlock();
}

Packet *DecoderThread::TryGetNextOutputPacket()
{
	Packet *packet = nullptr;

	this->outputPacketQueueLock.Lock();
	if (!this->outputPacketQueue.IsEmpty())
	{
		packet = this->outputPacketQueue.PopFront();
		this->outputPacketQueueSignal.Signal();
	}
	this->outputPacketQueueLock.Unlock();

	return packet;
}

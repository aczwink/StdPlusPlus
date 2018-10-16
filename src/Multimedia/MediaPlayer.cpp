/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Audio/Buffer.hpp>
#include <Std++/Audio/Source.hpp>
#include <Std++/Devices/DeviceEnumerator.hpp>
#include <Std++/UI/Displays/VideoWidget.hpp>
#include <Std++/Integer.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::Multimedia;

//Constructor
MediaPlayer::MediaPlayer(SeekableInputStream &inputStream) : inputStream(inputStream), masterClockTimer(Function<void()>(&MediaPlayer::OnMasterClockTriggered, this)), demuxerThread(this)
{
	this->demuxer = nullptr;
	this->isPlaying = false;
	this->masterClock = 0;

	this->audio.activeStreamIndex = Natural<uint32>::Max();
	this->audio.lastPTS = 0;
	this->audio.nextBufferIndex = 0;
	this->audio.nextPacket = nullptr;

	this->video.activeStreamIndex = Natural<uint32>::Max();
	this->video.nextPacket = nullptr;
	this->video.outputWidget = nullptr;

	for (uint8 i = 0; i < c_nAudioBuffers; i++)
		this->audio.bufferDurations[i] = 0;

	//first find format
	this->format = Format::Find(this->inputStream);
	if(!this->format)
	{
		NOT_IMPLEMENTED_ERROR;
		/*
		//second try by extension
		this->format = Format::FindByExtension(refPath.GetFileExtension());
		if(!this->pFormat)
		{
			this->refLogger << "No format could be found for file '" << refPath.GetString() << "'. Either the format is not supported or this is not a valid media file." << endl;
			return;
		}
		*/
	}

	//get demuxer
	this->demuxer = this->format->CreateDemuxer(this->inputStream);
	if(!this->demuxer)
	{
		//this->refLogger << "No demuxer is available for format '" << pFormat->GetName() << "'." << endl;
		NOT_IMPLEMENTED_ERROR;
		return;
	}

	this->demuxer->ReadHeader();
	this->demuxer->FindStreamInfo();

	for(uint32 i = 0; i < this->demuxer->GetNumberOfStreams(); i++)
	{
		Stream *stream = this->demuxer->GetStream(i);

		switch(stream->GetType())
		{
			case DataType::Audio:
			{
				AudioStream *const& audioStream = (AudioStream *)stream;

				if(audioStream->AllInfoIsAvailable())
				{
					//audio playback is only possible if really all info is available
					this->audio.streams.Insert(i, audioStream);
				}
			}
			break;
			case DataType::Subtitle:
			{
				SubtitleStream *const& subtitleStream = (SubtitleStream *)stream;

				this->subtitleStreams.Insert(i, subtitleStream);
			}
			break;
			case DataType::Video:
			{
				VideoStream *const& videoStream = (VideoStream *)stream;

				if(videoStream->GetDecoderContext())
				{
					this->video.streams.Insert(i, videoStream);
				}
			}
			break;
		}
	}

	//get audio device context
	if (!this->audio.streams.IsEmpty())
	{
		DeviceEnumerator deviceEnumerator(DeviceType::Audio);
		AutoPointer<Device> audioDevice = deviceEnumerator.GetNextDevice();
		if (!audioDevice.IsNull())
		{
			this->audio.deviceContext = audioDevice.Cast<AudioDevice>()->CreateDeviceContext();
			this->audio.source = this->audio.deviceContext->CreateSource();

			for (uint8 i = 0; i < c_nAudioBuffers; i++)
			{
				this->audio.buffers[i] = this->audio.deviceContext->CreateBuffer();
			}
		}
	}
	
	//start threads
	this->audio.decodeThread = new _stdxx_::DecoderThread(this);
	this->video.decodeThread = new _stdxx_::DecoderThread(this);

	this->demuxerThread.Connect(this->audio.decodeThread.operator->(), this->video.decodeThread.operator->());

	this->demuxerThread.Start();
	this->audio.decodeThread->Start();
	this->video.decodeThread->Start();
}

//Destructor
MediaPlayer::~MediaPlayer()
{
	this->HaltPlayback();

	//wait for threads to terminate
	while (this->demuxerThread.IsAlive())
	{
		this->ShutdownThreads();
		this->demuxerThread.Join_ms(1);
	}
	while (this->audio.decodeThread->IsAlive())
	{
		this->ShutdownThreads();
		this->audio.decodeThread->Join_ms(1);
	}
	while (this->video.decodeThread->IsAlive())
	{
		this->ShutdownThreads();
		this->video.decodeThread->Join_ms(1);
	}

	//
	delete this->demuxer;
	delete this->video.nextPacket;
}

//Eventhandlers
void MediaPlayer::OnMasterClockTriggered()
{
	uint64 elapsed;

	elapsed = this->clock.GetElapsedMicroseconds();
	this->clock.Start(); //restart the clock
	this->masterClock += elapsed;

	//check video
	if(this->video.activeStreamIndex != Natural<uint32>::Max())
	{
		VideoStream *videoStream = this->video.streams[this->video.activeStreamIndex];

		if(this->video.nextPacket) //do we already have the next packet?
		{
			this->video.frameDelay -= elapsed;
			if(this->video.frameDelay <= 0 && this->video.outputWidget)
			{
				this->video.outputWidget->UpdatePicture(this->video.nextPacket, videoStream->size);
				this->video.nextPacket = nullptr;
			}
		}
		else
		{
			this->video.nextPacket = this->video.decodeThread->TryGetNextOutputPacket();
			if(this->video.nextPacket)
			{
				if(this->video.nextPacket->pts == Natural<uint64>::Max())
					this->video.frameDelay = 20 * 1000; //next frame in 20ms hence 50FPS
				else
					this->video.frameDelay = (this->video.nextPacket->pts * videoStream->timeScale.numerator * 1000000 / videoStream->timeScale.denominator) - this->masterClock;
			}
		}
	}

	//check audio
	int64 audioFrameDelay = Integer<int64>::Max();
	if (this->audio.activeStreamIndex != Natural<uint32>::Max())
	{
		AudioStream *audioStream = this->audio.streams[this->audio.activeStreamIndex];

		if (!this->audio.deviceContext.IsNull()) //process only if we have a playback device
		{
			if (!this->audio.nextPacket) //do we already have the next packet?
				this->audio.nextPacket = this->audio.decodeThread->TryGetNextOutputPacket();

			if(this->audio.nextPacket)
			{
				if (this->audio.source->GetNumberOfQueuedBuffers() < c_nAudioBuffers) //is a buffer free?
				{
					uint8 bufferIndex = this->audio.nextBufferIndex;
					uint8 prevBufferIndex = (this->audio.nextBufferIndex + c_nAudioBuffers + 1) % c_nAudioBuffers;

					this->audio.bufferDurations[bufferIndex] = this->audio.nextPacket->pts - this->audio.lastPTS;
					this->audio.lastPTS = this->audio.nextPacket->pts;
					
					this->audio.buffers[bufferIndex]->SetData(this->audio.nextPacket->GetData(), this->audio.nextPacket->GetSize(), audioStream->sampleRate, audioStream->sampleFormat->nChannels);
					this->audio.source->EnqueueBuffer(*this->audio.buffers[bufferIndex]);

					//packet is not needed anymore
					delete this->audio.nextPacket;
					this->audio.nextPacket = nullptr;
					
					this->audio.nextBufferIndex = (this->audio.nextBufferIndex + 1) % c_nAudioBuffers;

					if (this->audio.source->GetNumberOfQueuedBuffers() < c_nAudioBuffers)
						audioFrameDelay = 0; //call again immediatly
					else
						audioFrameDelay = this->audio.bufferDurations[prevBufferIndex]; //wait until at least the previous buffer was played
				}
				else
				{
					//no buffer free

					uint8 prevBufferIndex = (this->audio.nextBufferIndex + c_nAudioBuffers + 1) % c_nAudioBuffers;
					audioFrameDelay = this->audio.bufferDurations[prevBufferIndex]; //wait until at least the previous buffer was played
				}
			}
			else
			{
				audioFrameDelay = 1000; //hm hope an audio frame comes in the next ms
			}
		}
	}

	//set up next call
	int64 nextDelay = Math::Min(this->video.frameDelay, audioFrameDelay);
	nextDelay = Max(nextDelay, int64(1));

	this->masterClockTimer.OneShot((uint32) (nextDelay * 1000));
}

//Private methods
void MediaPlayer::HaltPlayback()
{
	this->isPlaying = false;
	this->masterClockTimer.Stop();

	if (!this->audio.source.IsNull())
		this->audio.source->Stop();
}

void MediaPlayer::ShutdownThreads()
{
	this->demuxerThread.Shutdown();
	this->audio.decodeThread->Shutdown();
	this->video.decodeThread->Shutdown();
}

//Public methods
void MediaPlayer::Pause()
{
	if(this->isPlaying)
	{
		this->HaltPlayback();
	}
}

void MediaPlayer::Play()
{
	if(!this->isPlaying)
	{
		this->isPlaying = true;

		this->demuxerThread.Work();
		this->audio.decodeThread->Work();
		this->video.decodeThread->Work();

		this->clock.Start();
		this->masterClockTimer.OneShot(1);
		
		if (!this->audio.source.IsNull())
			this->audio.source->Play();
	}
}
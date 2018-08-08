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
#include <Std++/UI/Displays/VideoWidget.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::Multimedia;

//Constructor
/*MediaPlayer::MediaPlayer(SeekableInputStream &inputStream) : inputStream(inputStream), masterClockTimer(Function<void()>(&MediaPlayer::OnMasterClockTriggered, this)), audioDecodeThread(this, CodecId::PCM_S16LE), demuxerThread(this), videoDecodeThread(this, CodecId::RGB24)
{
	this->demuxer = nullptr;
	this->audioStreamIndex = Natural<uint32>::Max();
	this->videoStreamIndex = Natural<uint32>::Max();
	this->isPlaying = false;
	this->masterClock = 0;
	this->nextVideoPacket = nullptr;
	this->videoWidget = nullptr;

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
		*//*
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
					this->audioStreams.Insert(i, audioStream);
					if(this->audioStreamIndex == Natural<uint32>::Max())
						this->audioStreamIndex = i;
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

				if(videoStream->GetDecoder())
				{
					this->videoStreams.Insert(i, videoStream);
					if(this->videoStreamIndex == Natural<uint32>::Max())
						this->videoStreamIndex = i;
				}
			}
			break;
		}
	}

	this->demuxerThread.Connect(&this->audioDecodeThread, &this->videoDecodeThread);
	this->demuxerThread.SetStreamIndices(this->videoStreamIndex, this->audioStreamIndex);
	this->audioDecodeThread.SetStreamIndex(this->audioStreamIndex);
	this->videoDecodeThread.SetStreamIndex(this->videoStreamIndex);

	//start threads
	this->demuxerThread.Start();
	this->audioDecodeThread.Start();
	this->videoDecodeThread.Start();
}*/

//Destructor
MediaPlayer::~MediaPlayer()
{
	this->HaltPlayback();

	if(this->demuxer)
		delete this->demuxer;

	if(this->nextVideoPacket)
		delete this->nextVideoPacket;

	//wait for threads to terminate
	this->demuxerThread.Shutdown();
	this->audioDecodeThread.Shutdown();
	this->videoDecodeThread.Shutdown();
}

//Eventhandlers
void MediaPlayer::OnMasterClockTriggered()
{
	uint64 elapsed;

	elapsed = this->clock.GetElapsedMicroseconds();
	this->clock.Start(); //restart the clock
	this->masterClock += elapsed;

	//check video
	if(this->videoStreamIndex != Natural<uint32>::Max())
	{
		VideoStream *videoStream = this->videoStreams[this->videoStreamIndex];

		if(this->nextVideoPacket) //do we already have the next packet?
		{
			this->videoFrameDelay -= elapsed;
			if(this->videoFrameDelay <= 0 && this->videoWidget)
			{
				this->videoWidget->UpdatePicture(this->nextVideoPacket, Size<uint16>(videoStream->width, videoStream->height));
				this->nextVideoPacket = nullptr;
			}
		}
		else
		{
			this->nextVideoPacket = this->videoDecodeThread.TryGetNextOutputPacket();
			if(this->nextVideoPacket)
			{
				if(this->nextVideoPacket->pts == Natural<uint64>::Max())
					this->videoFrameDelay = 20 * 1000; //next frame in 20ms hence 50FPS
				else
					this->videoFrameDelay = (this->nextVideoPacket->pts * videoStream->timeScale.numerator * 1000000 / videoStream->timeScale.denominator) - this->masterClock;
			}
		}
	}

	//set up next call
	int64 nextDelay = MIN(this->videoFrameDelay, 0); //TODO: audioframedealy
	nextDelay = Max(nextDelay, int64(1));

	this->masterClockTimer.OneShot((uint32) (nextDelay * 1000));
}

//Private methods
void MediaPlayer::HaltPlayback()
{
	this->isPlaying = false;
	this->masterClockTimer.Stop();

	/*TODO: AUDIO
	if(this->pAudioVoice)
		this->pAudioVoice->Stop();
	 */
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

		this->demuxerThread.Run();
		this->audioDecodeThread.Run();
		this->videoDecodeThread.Run();

		this->clock.Start();
		this->masterClockTimer.OneShot(1);

		/*TODO: AUDIO
		if(this->pAudioVoice)
			this->pAudioVoice->Start();
		 */
	}
}
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
#pragma once
//Local
#include <Std++/SmartPointers/UniquePointer.hpp>
#include "../Containers/LinkedList/LinkedList.hpp"
#include "../Multitasking/ConditionVariable.hpp"
#include "../Multitasking/Mutex.hpp"
#include "../Multitasking/Thread.hpp"
#include "../Streams/SeekableInputStream.hpp"
#include "../Time/Clock.hpp"
#include "../Time/Timer.hpp"
#include "AudioStream.hpp"
#include "Demuxer.hpp"
#include "Format.hpp"
#include "SubtitleStream.hpp"
#include "VideoStream.hpp"

namespace StdXX
{
	namespace Multimedia
	{
		//Move declarations
		class MediaPlayer;
	}
}

namespace _stdxx_
{
	class DecoderThread : public StdXX::Thread
	{
	public:
		//Constructors
		DecoderThread(StdXX::Multimedia::MediaPlayer *player, StdXX::Multimedia::CodingFormatId encodingCodec);
		DecoderThread(StdXX::Multimedia::MediaPlayer* player, StdXX::Multimedia::NamedPixelFormat pixelFormatName);

		//Destructor
		~DecoderThread();

		//Methods
		void FlushInputQueue();
		void FlushOutputQueue();
		StdXX::Multimedia::Packet *TryGetNextOutputPacket();

		//Inline
		inline void AddInputPacket(StdXX::Multimedia::Packet *packet)
		{
			this->inputPacketQueueLock.Lock();
			this->inputPacketQueue.InsertTail(packet);
			this->inputPacketQueueLock.Unlock();
			this->inputPacketQueueSignal.Signal();
		}

		inline void Run()
		{
			this->work = true;
			this->workLock.Lock();
			this->workSignal.Signal();
			this->workLock.Unlock();
		}

		inline void SetStreamIndex(uint32 streamIndex)
		{
			this->streamIndex = streamIndex;
		}

		inline void Shutdown()
		{
			this->shutdown = true;
			this->work = false;

			this->workLock.Lock();
			this->workSignal.Signal();
			this->workLock.Unlock();

			this->inputPacketQueueSignal.Signal();
			this->Join();
		}

		inline void Stop()
		{
			this->work = false;
			this->inputPacketQueueSignal.Signal();
		}

	private:
		//Members
		StdXX::Multimedia::MediaPlayer *player;
		bool shutdown;
		bool work;
		StdXX::Mutex workLock;
		StdXX::ConditionVariable workSignal;
		volatile bool working;
		uint32 streamIndex;
		StdXX::Multimedia::DecoderContext *decoderContext;
		StdXX::UniquePointer<StdXX::Multimedia::Stream> encodingStream;
		StdXX::Multimedia::EncoderContext *encoderContext;
		StdXX::LinkedList<StdXX::Multimedia::Packet *> inputPacketQueue;
		StdXX::Mutex inputPacketQueueLock;
		StdXX::ConditionVariable inputPacketQueueSignal;
		StdXX::LinkedList<StdXX::Multimedia::Packet *> outputPacketQueue;
		StdXX::Mutex outputPacketQueueLock;

		//Methods
		StdXX::Multimedia::Packet *GetNextInputPacket();
		int32 ThreadMain();
		bool WaitForWork();

		//Inline
		inline void AddOutputPacket(StdXX::Multimedia::Packet *packet)
		{
			this->outputPacketQueueLock.Lock();
			this->outputPacketQueue.InsertTail(packet);
			this->outputPacketQueueLock.Unlock();
		}
	};

	class DemuxerThread : public StdXX::Thread
	{
	public:
		//Constructor
		DemuxerThread(StdXX::Multimedia::MediaPlayer *player);

		//Inline
		inline void Connect(DecoderThread *audioDecodeThread, DecoderThread *videoDecodeThread)
		{
			this->audioDecodeThread = audioDecodeThread;
			this->videoDecodeThread = videoDecodeThread;
		}

		inline void Run()
		{
			this->work = true;
			this->workLock.Lock();
			this->workSignal.Signal();
			this->workLock.Unlock();
		}

		inline void SetStreamIndices(uint32 videoStreamIndex, uint32 audioStreamIndex)
		{
			this->videoStreamIndex = videoStreamIndex;
			this->audioStreamIndex = audioStreamIndex;
		}

		inline void Shutdown()
		{
			this->shutdown = true;
			this->work = false;

			this->workLock.Lock();
			this->workSignal.Signal();
			this->workLock.Unlock();
			this->Join();
		}

	private:
		//Members
		StdXX::Multimedia::MediaPlayer *player;
		StdXX::Multimedia::Demuxer *demuxer;
		bool shutdown;
		bool work;
		StdXX::Mutex workLock;
		StdXX::ConditionVariable workSignal;
		volatile bool working;
		uint32 audioStreamIndex;
		DecoderThread *audioDecodeThread;
		uint32 videoStreamIndex;
		DecoderThread *videoDecodeThread;

		//Methods
		int32 ThreadMain();
	};
}

namespace StdXX
{
	namespace UI
	{
		//Move declarations
		class VideoWidget;
	}

	namespace Multimedia
	{
		class STDPLUSPLUS_API MediaPlayer
		{
		public:
			//Constructor
			MediaPlayer(SeekableInputStream &inputStream);

			//Destructor
			~MediaPlayer();

			//Methods
			void Pause();
			void Play();

			//Inline
			inline Demuxer *GetDemuxer()
			{
				return this->demuxer;
			}

			inline bool IsPlaying() const
			{
				return this->isPlaying;
			}

			inline void SetVideoOutput(UI::VideoWidget *videoWidget)
			{
				this->videoWidget = videoWidget;
			}

		private:
			//Members
			SeekableInputStream &inputStream;
			const Format *format;
			Demuxer *demuxer;
			Map<uint32, AudioStream *> audioStreams;
			uint32 audioStreamIndex;
			Map<uint32, SubtitleStream *> subtitleStreams;
			Map<uint32, VideoStream *> videoStreams;
			uint32 videoStreamIndex;
			bool isPlaying;
			StdXX::Clock clock;
			/**
			 * In microseconds.
			 */
			uint64 masterClock;
			Timer masterClockTimer;
			Packet *nextVideoPacket;
			/**
			 * In microseconds.
			 * Time until nextVideoPacket should be presented.
			 */
			int64 videoFrameDelay;
			UI::VideoWidget *videoWidget;

			_stdxx_::DemuxerThread demuxerThread;
			_stdxx_::DecoderThread audioDecodeThread;
			_stdxx_::DecoderThread videoDecodeThread;

			//Eventhandlers
			void OnMasterClockTriggered();

			//Methods
			void HaltPlayback();
		};
	}
}
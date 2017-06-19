/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
//Local
#include "../Containers/LinkedList/LinkedList.hpp"
#include "../Multitasking/ConditionVariable.hpp"
#include "ACStdLib/Multitasking/Mutex.hpp"
#include "../Multitasking/Thread.hpp"
#include "../Streams/SeekableInputStream.hpp"
#include "../Time/Clock.hpp"
#include "../Time/Timer.hpp"
#include "AudioStream.hpp"
#include "Demuxer.hpp"
#include "Format.hpp"
#include "SubtitleStream.hpp"
#include "VideoStream.hpp"

namespace ACStdLib
{
	namespace Multimedia
	{
		//Forward declarations
		class MediaPlayer;
	}
}

namespace _AC_Intern
{
	class DecoderThread : public ACStdLib::Thread
	{
	public:
		//Constructor
		DecoderThread(ACStdLib::Multimedia::MediaPlayer *player, ACStdLib::Multimedia::CodecId encodingCodec);

		//Destructor
		~DecoderThread();

		//Methods
		void FlushInputQueue();
		void FlushOutputQueue();
		ACStdLib::Multimedia::Packet *TryGetNextOutputPacket();

		//Inline
		inline void AddInputPacket(ACStdLib::Multimedia::Packet *packet)
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
		ACStdLib::Multimedia::MediaPlayer *player;
		bool shutdown;
		bool work;
		ACStdLib::Mutex workLock;
		ACStdLib::ConditionVariable workSignal;
		volatile bool working;
		uint32 streamIndex;
		ACStdLib::Multimedia::Decoder *decoder;
		ACStdLib::Multimedia::Encoder *encoder;
		ACStdLib::LinkedList<ACStdLib::Multimedia::Packet *> inputPacketQueue;
		ACStdLib::Mutex inputPacketQueueLock;
		ACStdLib::ConditionVariable inputPacketQueueSignal;
		ACStdLib::LinkedList<ACStdLib::Multimedia::Packet *> outputPacketQueue;
		ACStdLib::Mutex outputPacketQueueLock;

		//Methods
		ACStdLib::Multimedia::Packet *GetNextInputPacket();
		int32 ThreadMain();
		bool WaitForWork();

		//Inline
		inline void AddOutputPacket(ACStdLib::Multimedia::Packet *packet)
		{
			this->outputPacketQueueLock.Lock();
			this->outputPacketQueue.InsertTail(packet);
			this->outputPacketQueueLock.Unlock();
		}
	};

	class DemuxerThread : public ACStdLib::Thread
	{
	public:
		//Constructor
		DemuxerThread(ACStdLib::Multimedia::MediaPlayer *player);

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
		ACStdLib::Multimedia::MediaPlayer *player;
		ACStdLib::Multimedia::Demuxer *demuxer;
		bool shutdown;
		bool work;
		ACStdLib::Mutex workLock;
		ACStdLib::ConditionVariable workSignal;
		volatile bool working;
		uint32 audioStreamIndex;
		DecoderThread *audioDecodeThread;
		uint32 videoStreamIndex;
		DecoderThread *videoDecodeThread;

		//Methods
		int32 ThreadMain();
	};
}

namespace ACStdLib
{
	namespace UI
	{
		//Forward declarations
		class VideoWidget;
	}

	namespace Multimedia
	{
		class MediaPlayer
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
			ACStdLib::Clock clock;
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

			_AC_Intern::DemuxerThread demuxerThread;
			_AC_Intern::DecoderThread audioDecodeThread;
			_AC_Intern::DecoderThread videoDecodeThread;

			//Eventhandlers
			void OnMasterClockTriggered();

			//Methods
			void HaltPlayback();
		};
	}
}
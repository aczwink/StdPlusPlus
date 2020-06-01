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
#pragma once
//Local
#include <Std++/Devices/AudioDevice.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>
#include "../Containers/LinkedList/LinkedList.hpp"
#include "../Multitasking/ConditionVariable.hpp"
#include "../Multitasking/Mutex.hpp"
#include "../Multitasking/Thread.hpp"
#include "../Streams/SeekableInputStream.hpp"
#include "../Time/Clock.hpp"
#include "../Time/Timer.hpp"
#include "AudioStream.hpp"
#include "ComputePixmapResampler.hpp"
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
		DecoderThread(StdXX::Multimedia::MediaPlayer *player);

		//Destructor
		~DecoderThread();

		//Methods
		void Flush();
		void FlushInputQueue();
		void FlushOutputQueue();
		void SetStreamIndex(uint32 streamIndex);
		void Shutdown();
		StdXX::Multimedia::Packet *TryGetNextOutputPacket();

		//Inline
		inline void AddInputPacket(StdXX::UniquePointer<StdXX::Multimedia::IPacket> packet)
		{
			this->inputPacketQueueLock.Lock();
			while (this->inputPacketQueue.GetNumberOfElements() >= 100)
			{
				if (this->shutdown)
					break;
				if (!this->work)
					break;

				this->inputPacketQueueSignal.Wait(this->inputPacketQueueLock);
			}
			this->inputPacketQueue.InsertTail(Move(packet));
			this->inputPacketQueueSignal.Signal();
			this->inputPacketQueueLock.Unlock();
		}

		inline void Work()
		{
			this->work = true;
			this->workLock.Lock();
			this->workSignal.Signal();
			this->workLock.Unlock();
		}

		inline void Stop()
		{
			this->work = false;
			this->inputPacketQueueLock.Lock();
			this->inputPacketQueueSignal.Signal();
			this->inputPacketQueueLock.Unlock();
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
		bool requireResampling;
		StdXX::UniquePointer<StdXX::Multimedia::ComputePixmapResampler> pixmapResampler;
		StdXX::UniquePointer<StdXX::Multimedia::Stream> encodingStream;
		StdXX::Multimedia::EncoderContext *encoderContext;
		StdXX::LinkedList<StdXX::UniquePointer<StdXX::Multimedia::IPacket>> inputPacketQueue;
		StdXX::Mutex inputPacketQueueLock;
		StdXX::ConditionVariable inputPacketQueueSignal;
		StdXX::LinkedList<StdXX::Multimedia::Packet *> outputPacketQueue;
		StdXX::Mutex outputPacketQueueLock;
		StdXX::ConditionVariable outputPacketQueueSignal;

		//Methods
		StdXX::UniquePointer<StdXX::Multimedia::IPacket> GetNextInputPacket();
		void Run() override;
		bool WaitForWork();

		//Inline
		inline void AddOutputPacket(StdXX::Multimedia::Packet *packet)
		{
			this->outputPacketQueueLock.Lock();
			while (this->outputPacketQueue.GetNumberOfElements() >= 100)
			{
				if (this->shutdown)
					break;
				if (!this->work)
					break;

				this->outputPacketQueueSignal.Wait(this->outputPacketQueueLock);
			}
			this->outputPacketQueue.InsertTail(packet);
			this->outputPacketQueueSignal.Signal();
			this->outputPacketQueueLock.Unlock();
		}
	};

	class DemuxerThread : public StdXX::Thread
	{
	public:
		//Constructor
		DemuxerThread(StdXX::Multimedia::Demuxer *demuxer);

		//Properties
		inline bool IsWorking() const
		{
			return this->working;
		}

		//Inline
		inline void Connect(DecoderThread *audioDecodeThread, DecoderThread *videoDecodeThread)
		{
			this->audioDecodeThread = audioDecodeThread;
			this->videoDecodeThread = videoDecodeThread;
		}

		inline void Work()
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
		}

		inline void Stop()
		{
			this->work = false;
		}

	private:
		//Members
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
		void Run() override;
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
			static const uint8 c_nAudioBuffers = 3;
		public:
			//Constructor
			MediaPlayer(SeekableInputStream &inputStream);

			//Destructor
			~MediaPlayer();

			//Methods
			void Pause();
			void Play();
			void Seek(uint64 ts, const TimeScale& timeScale);

			//Properties
			inline const class Demuxer& Demuxer() const
			{
				return *this->demuxer;
			}

			inline uint64 MasterClock() const
			{
				return this->masterClock;
			}

			//Inline
			inline const Map<uint32, AudioStream*>& GetAudioStreams() const
			{
				return this->audio.streams;
			}

			inline const Map<uint32, SubtitleStream*>& GetSubtitleStreams() const
			{
				return this->subtitleStreams;
			}

			inline const Map<uint32, VideoStream*>& GetVideoStreams() const
			{
				return this->video.streams;
			}

			inline bool IsPlaying() const
			{
				return this->isPlaying;
			}

			inline void SetAudioStreamIndex(uint32 streamIndex)
			{
				this->audio.activeStreamIndex = streamIndex;
				this->demuxerThread->SetStreamIndices(this->video.activeStreamIndex, this->audio.activeStreamIndex);
				this->audio.decodeThread->SetStreamIndex(this->audio.activeStreamIndex);
			}

			inline void SetVideoOutput(UI::VideoWidget *videoWidget)
			{
				this->video.outputWidget = videoWidget;
			}

			inline void SetVideoStreamIndex(uint32 streamIndex)
			{
				this->video.activeStreamIndex = streamIndex;
				this->demuxerThread->SetStreamIndices(this->video.activeStreamIndex, this->audio.activeStreamIndex);
				this->video.decodeThread->SetStreamIndex(this->video.activeStreamIndex);
			}

		private:
			//Members
			SeekableInputStream &inputStream;
			const Format *format;
			class Demuxer *demuxer;
			struct
			{
				Map<uint32, AudioStream *> streams;
				uint32 activeStreamIndex;
				Packet *nextPacket;

				AutoPointer<AudioDevice> device;
				UniquePointer<Audio::DeviceContext> deviceContext;
				UniquePointer<Audio::Source> source;
				UniquePointer<Audio::Buffer> buffers[c_nAudioBuffers];
				uint64 bufferDurations[c_nAudioBuffers];
				uint8 nextBufferIndex;
				uint64 lastPTS;
				UniquePointer<_stdxx_::DecoderThread> decodeThread;
			} audio;
			Map<uint32, SubtitleStream *> subtitleStreams;
			struct
			{
				Map<uint32, VideoStream *> streams;
				uint32 activeStreamIndex;
				Packet *nextPacket;
				/**
				 * In microseconds.
				 * Time until nextPacket should be presented.
				 */
				int64 frameDelay;

				UI::VideoWidget *outputWidget;
				UniquePointer<_stdxx_::DecoderThread> decodeThread;
			} video;
			bool isPlaying;
			StdXX::Clock clock;
			/**
			 * In microseconds.
			 */
			uint64 masterClock;
			Timer masterClockTimer;

			UniquePointer<_stdxx_::DemuxerThread> demuxerThread;

			//Eventhandlers
			void OnMasterClockTriggered();

			//Methods
			void HaltPlayback();
			void ShutdownThreads();
			void StartMasterClock();
		};
	}
}
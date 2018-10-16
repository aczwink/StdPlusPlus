/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
//Global
#include <XAudio2.h>
//Local
#include <Std++/Audio/Source.hpp>
#include <Std++/Containers/LinkedList/LinkedList.hpp>
#include <Std++/Containers/Map/Map.hpp>
#include <Std++/Multitasking/Mutex.hpp>
#include "XAudio2Buffer.hpp"

class XAudio2Source : public StdXX::Audio::Source, public IXAudio2VoiceCallback
{
	struct VoiceData
	{
		IXAudio2SourceVoice* pXAudio2SourceVoice;
		uint32 sampleRate;
	};
public:
	//Constructor
	inline XAudio2Source(IXAudio2* pXAudio2) : isPlaying(false), pXAudio2(pXAudio2), activeVoice(nullptr), nSubmittedBuffers(0)
	{
	}

	//Destructor
	~XAudio2Source();

	//Methods
	void EnqueueBuffer(const StdXX::Audio::Buffer & buffer) override;
	uint32 GetNumberOfQueuedBuffers() const override;
	bool IsPlaying() const override;
	void Play() override;
	void SetGain(float32 gain) override;
	void SetLooping(bool loop) override;
	void SetPitch(float32 pitch) override;
	void SetPosition(const StdXX::Math::Vector3s & pos) override;
	void SetVelocity(const StdXX::Math::Vector3s & vel) override;
	void Stop() override;

	//IXAudio2VoiceCallback event handlers
	void OnVoiceProcessingPassStart(UINT32 BytesRequired) override;
	void OnVoiceProcessingPassEnd(void) override;
	void OnStreamEnd(void) override;
	void OnBufferStart(void * pBufferContext) override;
	void OnBufferEnd(void * pBufferContext) override;
	void OnLoopEnd(void * pBufferContext) override;
	void OnVoiceError(void * pBufferContext, HRESULT Error) override;

private:
	//Members
	bool isPlaying;
	IXAudio2* pXAudio2;
	VoiceData* activeVoice;
	uint8 activeVoiceChannelsCount;
	StdXX::Map<uint8, VoiceData> channelVoices;
	StdXX::LinkedList<const XAudio2Buffer*> bufferQueue;
	uint32 nSubmittedBuffers;
	mutable StdXX::Mutex bufferQueueLock;

	//Methods
	void PlayNextBuffer();
};
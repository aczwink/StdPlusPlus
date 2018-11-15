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
//Class header
#include "XAudio2Source.hpp"
//Namespaces
using namespace StdXX;
using namespace StdXX::Audio;

//Destructor
XAudio2Source::~XAudio2Source()
{
	for (const auto& kv : this->channelVoices)
		kv.value.pXAudio2SourceVoice->DestroyVoice();
}

//Public methods
void XAudio2Source::EnqueueBuffer(const Buffer & buffer)
{
	const XAudio2Buffer& typedBuffer = dynamic_cast<const XAudio2Buffer&>(buffer);

	if (!this->channelVoices.Contains(typedBuffer.GetNumberOfChannels()))
	{
		//fill out wave format
		WAVEFORMATEX waveFormat{};
		waveFormat.cbSize = sizeof(waveFormat);
		waveFormat.wFormatTag = WAVE_FORMAT_PCM;
		waveFormat.wBitsPerSample = 16;
		waveFormat.nChannels = typedBuffer.GetNumberOfChannels();
		waveFormat.nSamplesPerSec = typedBuffer.GetSampleRate();
		waveFormat.nBlockAlign = waveFormat.nChannels * (waveFormat.wBitsPerSample / 8);
		waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
		
		IXAudio2SourceVoice *activeVoice = nullptr;
		HRESULT hr = this->pXAudio2->CreateSourceVoice(&activeVoice, &waveFormat, 0, XAUDIO2_DEFAULT_FREQ_RATIO, this);
		ASSERT(hr == S_OK, u8"Report this please!");

		VoiceData vd;
		vd.pXAudio2SourceVoice = activeVoice;
		vd.sampleRate = typedBuffer.GetSampleRate();

		this->channelVoices[typedBuffer.GetNumberOfChannels()] = vd;
	}

	this->bufferQueueLock.Lock();
	this->bufferQueue.InsertTail(&typedBuffer);
	this->bufferQueueLock.Unlock();

	if (this->isPlaying)
		this->PlayNextBuffer();
}

uint32 XAudio2Source::GetNumberOfQueuedBuffers() const
{
	AutoLock lock(this->bufferQueueLock);
	return this->bufferQueue.GetNumberOfElements() + this->nSubmittedBuffers;
}

bool XAudio2Source::IsPlaying() const
{
	return this->isPlaying;
}

void XAudio2Source::Play()
{
	ASSERT(!this->isPlaying, u8"Can't play a source thats already playing!");

	this->isPlaying = true;
	this->PlayNextBuffer();
}

void XAudio2Source::SetGain(float32 gain)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void XAudio2Source::SetLooping(bool loop)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void XAudio2Source::SetPitch(float32 pitch)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void XAudio2Source::SetPosition(const Math::Vector3s & pos)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void XAudio2Source::SetVelocity(const Math::Vector3s & vel)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void XAudio2Source::Stop()
{
	this->isPlaying = false;
	if(this->activeVoice)
		this->activeVoice->pXAudio2SourceVoice->Stop();
}

//Private methods
void XAudio2Source::PlayNextBuffer()
{
	const XAudio2Buffer* buffer;
	{
		AutoLock lock(this->bufferQueueLock);

		if (this->bufferQueue.IsEmpty())
			return;

		buffer = this->bufferQueue.PopFront(); //get next buffer
		this->nSubmittedBuffers++;
	}
	
	//check if we need to change voice
	if ((this->activeVoice == nullptr) || (buffer->GetNumberOfChannels() != this->activeVoiceChannelsCount))
	{
		if(this->activeVoice != nullptr)
			this->activeVoice->pXAudio2SourceVoice->Stop(); //stop old voice

		this->activeVoice = &this->channelVoices[buffer->GetNumberOfChannels()]; //the target voice must exist
		this->activeVoiceChannelsCount = buffer->GetNumberOfChannels();

		this->activeVoice->pXAudio2SourceVoice->Start();
	}

	//check if we need to change sample rate
	if (buffer->GetSampleRate() != this->activeVoice->sampleRate)
	{
		this->activeVoice->sampleRate = buffer->GetSampleRate();
		this->activeVoice->pXAudio2SourceVoice->SetSourceSampleRate(buffer->GetSampleRate());
	}
	
	this->activeVoice->pXAudio2SourceVoice->SubmitSourceBuffer(&buffer->GetNativeBuffer());
}

//IXAudio2VoiceCallback event handlers
void XAudio2Source::OnVoiceProcessingPassStart(UINT32 BytesRequired)
{
}

void XAudio2Source::OnVoiceProcessingPassEnd(void)
{
}

void XAudio2Source::OnStreamEnd(void)
{
}

void XAudio2Source::OnBufferStart(void * pBufferContext)
{
}

void XAudio2Source::OnBufferEnd(void * pBufferContext)
{
	this->bufferQueueLock.Lock();
	this->nSubmittedBuffers--;
	this->bufferQueueLock.Unlock();

	this->PlayNextBuffer();
}

void XAudio2Source::OnLoopEnd(void * pBufferContext)
{
}

void XAudio2Source::OnVoiceError(void * pBufferContext, HRESULT Error)
{
}
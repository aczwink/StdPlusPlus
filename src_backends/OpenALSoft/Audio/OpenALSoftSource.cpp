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
#include "OpenALSoftSource.hpp"
//Local
#include "OpenALSoftBuffer.hpp"

//Constructor
OpenALSoftSource::OpenALSoftSource(OpenALSoftDeviceContext &deviceContext) : deviceContext(deviceContext)
{
	this->deviceContext.Bind();
	alGenSources(1, &this->id);
}

//Destructor
OpenALSoftSource::~OpenALSoftSource()
{
	this->deviceContext.Bind();
	alDeleteSources(1, &this->id);
}

//Public methods
bool OpenALSoftSource::IsPlaying() const
{
	this->deviceContext.Bind();
	ALint state;
	alGetSourcei(this->id, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}

void OpenALSoftSource::Play()
{
	this->deviceContext.Bind();
	alSourcePlay(this->id);
}

void OpenALSoftSource::SetBuffer(const Buffer *buffer)
{
	const OpenALSoftBuffer *openALSoftBuffer = (OpenALSoftBuffer *) buffer;

	this->deviceContext.Bind();
	alSourcei(this->id, AL_BUFFER, openALSoftBuffer->GetId());
}

void OpenALSoftSource::SetGain(float32 gain)
{
	this->deviceContext.Bind();
	alSourcef(this->id, AL_GAIN, gain);
}

void OpenALSoftSource::SetLooping(bool loop)
{
	this->deviceContext.Bind();
	alSourcei(this->id, AL_LOOPING, loop);
}

void OpenALSoftSource::SetPitch(float32 pitch)
{
	this->deviceContext.Bind();
	alSourcef(this->id, AL_PITCH, pitch);
}

void OpenALSoftSource::SetPosition(const Math::Vector3s &pos)
{
	this->deviceContext.Bind();
	alSourcefv(this->id, AL_POSITION, &pos.x);
}

void OpenALSoftSource::SetVelocity(const Math::Vector3s &vel)
{
	this->deviceContext.Bind();
	alSourcefv(this->id, AL_VELOCITY, &vel.x);
}
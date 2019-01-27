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
#include "OpenALSoftDeviceContext.hpp"
//Local
#include "OpenALSoftSource.hpp"
#include "OpenALSoftBuffer.hpp"

//Class members
ALCcontext *OpenALSoftDeviceContext::currentContext = nullptr;

//Constructor
OpenALSoftDeviceContext::OpenALSoftDeviceContext(ALCdevice *device)
{
	this->context = alcCreateContext(device, nullptr);
}

//Destructor
OpenALSoftDeviceContext::~OpenALSoftDeviceContext()
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(this->context);
}

//Public methods
Buffer *OpenALSoftDeviceContext::CreateBuffer()
{
	return new OpenALSoftBuffer(*this);
}

Source *OpenALSoftDeviceContext::CreateSource()
{
	return new OpenALSoftSource(*this);
}

void OpenALSoftDeviceContext::SetListenerOrientation(const Math::Vector3S &at, const Math::Vector3S &up)
{
	this->Bind();
	Math::Vector3S orientation[2] = {at, up};
	alListenerfv(AL_ORIENTATION, &orientation[0].x);
}

void OpenALSoftDeviceContext::SetListenerPosition(const Math::Vector3S &pos)
{
	this->Bind();
	alListenerfv(AL_POSITION, &pos.x);
}

void OpenALSoftDeviceContext::SetListenerVelocity(const Math::Vector3S &vel)
{
	this->Bind();
	alListenerfv(AL_VELOCITY, &vel.x);
}
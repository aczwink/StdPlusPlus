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
#include "OpenALSoftBuffer.hpp"

//Constructor
OpenALSoftBuffer::OpenALSoftBuffer(OpenALSoftDeviceContext &deviceContext) : deviceContext(deviceContext)
{
	this->deviceContext.Bind();
	alGenBuffers(1, &this->id);
}

//Destructor
OpenALSoftBuffer::~OpenALSoftBuffer()
{
	this->deviceContext.Bind();
	alDeleteBuffers(1, &this->id);
}

//Public methods
void OpenALSoftBuffer::SetData(bool stereo, bool samples16bit, void *data, uint32 size, uint32 sampleRate)
{
	ALenum format;
	if(stereo)
	{
		if(samples16bit)
			format = AL_FORMAT_STEREO16;
		else
			format = AL_FORMAT_STEREO8;
	}
	else
	{
		if(samples16bit)
			format = AL_FORMAT_MONO16;
		else
			format = AL_FORMAT_MONO8;
	}
	this->deviceContext.Bind();
	alBufferData(this->id, format, data, size, sampleRate);
}

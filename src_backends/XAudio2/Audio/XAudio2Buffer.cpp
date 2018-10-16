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
#include "XAudio2Buffer.hpp"
//Local
#include <Std++/Memory.hpp>
//Namespaces
using namespace StdXX;

//Destructor
XAudio2Buffer::~XAudio2Buffer()
{
	if (this->data)
		MemFree(this->data);
}

//Public methods
void XAudio2Buffer::SetData(void * data, uint32 size, uint32 sampleRate, uint8 nChannels)
{
	if (this->data)
		MemFree(this->data);

	this->data = MemAlloc(size);
	MemCopy(this->data, data, size);
	
	this->audioBuffer.AudioBytes = size;
	this->audioBuffer.pAudioData = (BYTE*)this->data;

	this->sampleRate = sampleRate;
	this->nChannels = nChannels;
}
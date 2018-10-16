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
#pragma once
//Global
#include <XAudio2.h>
//Local
#include <Std++/Audio/Buffer.hpp>

class XAudio2Buffer : public StdXX::Audio::Buffer
{
public:
	//Constructor
	inline XAudio2Buffer() : audioBuffer({}), data(nullptr)
	{
	}

	//Destructor
	~XAudio2Buffer();

	//Methods
	void SetData(void * data, uint32 size, uint32 sampleRate, uint8 nChannels) override;

	//Inline
	inline const XAUDIO2_BUFFER& GetNativeBuffer() const
	{
		return this->audioBuffer;
	}

	inline uint8 GetNumberOfChannels() const
	{
		return this->nChannels;
	}

	inline uint32 GetSampleRate() const
	{
		return this->sampleRate;
	}

private:
	//Members
	XAUDIO2_BUFFER audioBuffer;
	void* data;
	uint32 sampleRate;
	uint8 nChannels;
};
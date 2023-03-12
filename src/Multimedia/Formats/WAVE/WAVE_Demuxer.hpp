/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/Demuxer.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

class WAVE_Demuxer : public Demuxer
{
public:
	//Constructor
	inline WAVE_Demuxer(const Format& format, SeekableInputStream& inputStream) : Demuxer(format, inputStream)
	{
	}

	//Methods
	void ReadHeader() override;
	void Seek(uint64 timestamp, const class TimeScale &timeScale) override;

private:
	//State
	uint64 dataOffset;
	uint32 dataSize;
	uint16 blockAlign;

	//Methods
	void ReadFmtChunk(uint32 chunkSize, uint16 &refBitsPerSample);
	UniquePointer<IPacket> ReadPacket() override;

	//Inline
	inline uint64 DataEndOffset() const
	{
		return this->dataOffset + this->dataSize;
	}
};
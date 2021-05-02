/*
 * Copyright (c) 2017-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/Muxer.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

class WAVE_Muxer : public Muxer
{
public:
	//Constructor
	WAVE_Muxer(const Format &refFormat, SeekableOutputStream &refOutput);

	//Methods
	void Finalize();
	void WriteHeader();
	void WritePacket(const IPacket& packet);

private:
	//Members
	uint32 riffTagSizeOffset;
	uint32 dataChunkSizeOffset;

	//Methods
	uint16 GetBitsPerSample(CodingFormatId codingFormatId) const;
	uint16 MapCodingFormatId(CodingFormatId codingFormatId) const;
};
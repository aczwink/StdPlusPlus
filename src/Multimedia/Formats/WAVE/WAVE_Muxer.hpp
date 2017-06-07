/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <ACStdLib/Multimedia/Muxer.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

class WAVE_Muxer : public Muxer
{
public:
	//Constructor
	WAVE_Muxer(const Format &refFormat, ASeekableOutputStream &refOutput);

	//Methods
	void Finalize();
	void WriteHeader();
	void WritePacket(const Packet &packet);

private:
	//Members
	uint32 riffTagSizeOffset;
	uint32 dataChunkSizeOffset;

	//Methods
	uint16 GetBitsPerSample(CodecId codecId) const;
};
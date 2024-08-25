/*
 * Copyright (c) 2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/Muxer.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

class DDS_Muxer : public Muxer
{
public:
	//Constructor
	inline DDS_Muxer(const ContainerFormat& format, SeekableOutputStream& outputStream) : Muxer(format, outputStream)
	{
	}

	//Methods
	void Finalize() override;
	void WriteHeader() override;
	void WritePacket(const IPacket &packet) override;

private:
	//State
	uint64 flagsOffset;
	uint64 mipMapCountOffset;
	uint64 capsOffset;

	//Methods
	uint32 ComputePitchSize() const;
	void WritePixelFormat(DataWriter& dataWriter);
};
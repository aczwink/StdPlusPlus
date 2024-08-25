/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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

class BMP_Muxer : public Muxer
{
private:
	//Members
	uint64 startOffset;
	uint32 imageSize;

public:
	//Constructor
	inline BMP_Muxer(const ContainerFormat &refFormat, SeekableOutputStream &refOutput) : Muxer(refFormat, refOutput)
	{
		this->imageSize = 0;
	}

	//Methods
	void Finalize();
	void WriteHeader();
	void WritePacket(const IPacket& packet);
};
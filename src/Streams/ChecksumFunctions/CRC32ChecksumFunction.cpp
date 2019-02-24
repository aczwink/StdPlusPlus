/*
 * Copyright (c) 2018-2019 Amir Czwink (amir130@hotmail.de)
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
#include "CRC32ChecksumFunction.hpp"
//Local
#include <Std++/Memory.hpp>
//Namespaces
using namespace _stdxx_;

uint32 g_crcTable[256];

//Constructor
CRC32ChecksumFunction::CRC32ChecksumFunction() : crc(0xFFFFFFFF)
{
	static bool tableComputed = false;
	if (!tableComputed)
	{
		tableComputed = true;
		
		for(uint16 b = 0; b <= 255; b++)
		{
			uint32 crc = b;
			for (int8 i = 7; i >= 0; i--)
			{
				uint32 mask = -(crc & 1);
				crc = (crc >> 1) ^ (0xEDB88320 & mask);
			}
			
			g_crcTable[b] = crc;
		}
	}
}

//Public methods
uint32 CRC32ChecksumFunction::GetChecksumSize() const
{
	return 4;
}

void CRC32ChecksumFunction::Finish()
{
	this->crc = ~this->crc; //invert all bits
}

void CRC32ChecksumFunction::StoreChecksum(void * target) const
{
	StdXX::MemCopy(target, &this->crc, this->GetChecksumSize());
}

void CRC32ChecksumFunction::Update(const void * buffer, uint32 size)
{
	const byte* data = (byte *) buffer;
	for (uint32 i = 0; i < size; i++)
	{
		byte currentByte = *data++;
		uint8 lookupIndex = ((this->crc ^ currentByte) & 0xFF);
		this->crc = (this->crc >> 8) ^ g_crcTable[lookupIndex];
	}
}
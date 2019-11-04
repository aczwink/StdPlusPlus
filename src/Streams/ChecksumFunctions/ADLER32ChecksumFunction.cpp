/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include "ADLER32ChecksumFunction.hpp"
//Namespaces
using namespace _stdxx_;

//Public methods
uint8 ADLER32ChecksumFunction::GetChecksumSize() const
{
	return 4;
}

void ADLER32ChecksumFunction::Finish()
{
}

uint64 ADLER32ChecksumFunction::GetChecksum() const
{
    return (this->b << 16) | this->a;
}

void ADLER32ChecksumFunction::Update(const void *buffer, uint32 size)
{
	const uint32_t MOD_ADLER = 65521;

	const byte* data = static_cast<const byte *>(buffer);
	for(uint32 i = 0; i < size; i++)
	{
		this->a = (this->a + data[i]) % MOD_ADLER;
		this->b = (this->b + this->a) % MOD_ADLER;
	}
}
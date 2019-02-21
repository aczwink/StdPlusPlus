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
#pragma once
//Local
#include <Std++/Containers/Strings/String.hpp>
#include "HashFunction.hpp"

namespace StdXX
{
	namespace Crypto
	{
		void PBKDF2(const String& password, const uint8* salt, uint16 saltSize, HashAlgorithm hashAlgorithm, uint32 nIterations, uint8* key, uint16 keySize);
		void scrypt(const String& password, const uint8* salt, uint8 saltSize, uint8* key, uint8 keySize, uint8 costFactor, uint8 blockSizeFactor = 8, uint8 parallelizationFactor = 1);
	}
}
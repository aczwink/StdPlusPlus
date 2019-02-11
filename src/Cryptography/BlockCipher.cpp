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
#include <Std++/Cryptography/BlockCipher.hpp>
//Local
#include "AES/AESCipher.hpp"
#ifdef _STDXX_EXTENSION_OPENSSL
#include "../../src_backends/OpenSSL/OpenSSL_Extension.hpp"
#endif
//Namespaces
using namespace StdXX;

//Class functions
UniquePointer<BlockCipher> BlockCipher::Create(CipherAlgorithm algorithm, const byte* key, uint16 keyLength)
{
	//prefer openssl over anything
#ifdef _STDXX_EXTENSION_OPENSSL
	return _stdxx_::OpenSSL_Extension::CreateCipher(algorithm, key, keyLength);
#endif

	//own implementations
	switch(algorithm)
	{
		/*
		 TODO: this implementation is buggy
		case CipherAlgorithm::AES:
			return new _stdxx_::AESCipher(key, keyLength);
		 */
	}

	return nullptr;
}

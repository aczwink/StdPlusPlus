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
//Global
#include <openssl/evp.h>
//Local
#include <Std++/Cryptography/BlockDecipher.hpp>

namespace _stdxx_
{
	class OpenSSL_BlockDecipher : public StdXX::BlockDecipher
	{
	public:
		//Constructor
		OpenSSL_BlockDecipher(const EVP_CIPHER* cipher, const byte* key, uint8 blockSize);

		//Destructor
		~OpenSSL_BlockDecipher();

		void Decrypt(const void *encrypted, byte *unencrypted) const override;

		uint8 GetBlockSize() const override;

	private:
		//Members
		uint8 blockSize;
		EVP_CIPHER_CTX* ctx;
	};
}
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
#include <Std++/Cryptography/HashFunction.hpp>

namespace _stdxx_
{
	class OpenSSL_Hasher : public StdXX::Crypto::HashFunction
	{
	public:
		//Constructor
		OpenSSL_Hasher(const EVP_MD* messageDigest);

		//Destructor
		~OpenSSL_Hasher();

		//Methods
		uint32 GetDigestSize() const override;
		void Finish() override;
		void StoreDigest(void *target) const override;
		void Update(const void *buffer, uint32 size) override;

		uint32 GetBlockSize() const override;

	private:
		//Members
		EVP_MD_CTX* ctx;
		byte hash[128];
	};
}
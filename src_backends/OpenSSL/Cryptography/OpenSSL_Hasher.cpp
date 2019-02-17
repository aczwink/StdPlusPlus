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
#include "OpenSSL_Hasher.hpp"
//Local
#include <Std++/Memory.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Constructor
OpenSSL_Hasher::OpenSSL_Hasher(const EVP_MD *messageDigest)
{
	this->ctx = EVP_MD_CTX_create();
	ASSERT(this->ctx, u8"Report this please!");

	int ret = EVP_DigestInit_ex(this->ctx, messageDigest, nullptr);
	ASSERT(ret == 1, u8"Report this please!");
}

//Destructor
OpenSSL_Hasher::~OpenSSL_Hasher()
{
	EVP_MD_CTX_destroy(this->ctx);
}

//Public methods
uint32 OpenSSL_Hasher::GetBlockSize() const
{
	return static_cast<uint32>(EVP_MD_block_size(EVP_MD_CTX_md(this->ctx)));
}

uint32 OpenSSL_Hasher::GetDigestSize() const
{
	return static_cast<uint32>(EVP_MD_size(EVP_MD_CTX_md(this->ctx)));
}

void OpenSSL_Hasher::Finish()
{
	ASSERT(sizeof(this->hash) >= this->GetDigestSize(), u8"REPORT THIS PLEASE!");
	unsigned int len;
	int ret = EVP_DigestFinal_ex(this->ctx, this->hash, &len);
	ASSERT(ret == 1, u8"Report this please!");
	ASSERT(len <= this->GetDigestSize(), u8"REPORT THIS PLEASE!");
}

void OpenSSL_Hasher::StoreDigest(void *target) const
{
	MemCopy(target, this->hash, this->GetDigestSize());
}

void OpenSSL_Hasher::Update(const void *buffer, uint32 size)
{
	int ret = EVP_DigestUpdate(this->ctx, buffer, size);
	ASSERT(ret == 1, u8"Report this please!");
}
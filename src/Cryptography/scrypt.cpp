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
//Corresponding header
#include <Std++/Cryptography/KeyDerivationFunctions.hpp>
//Local
#include <Std++/Streams/BufferInputStream.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
//Namespaces
using namespace StdXX;

//Based on code from https://en.wikipedia.org/wiki/Salsa20
static void salsa20_8_block(uint32_t out[16], uint32_t const in[16])
{
#define ROTL(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define QR(a, b, c, d)(		\
	b ^= ROTL(a + d, 7),	\
	c ^= ROTL(b + a, 9),	\
	d ^= ROTL(c + b,13),	\
	a ^= ROTL(d + c,18))

	int i;
	uint32_t x[16];

	for (i = 0; i < 16; ++i)
		x[i] = in[i];
	const int N_ROUNDS = 8;
	for (i = 0; i < N_ROUNDS; i += 2) {
		// Odd round
		QR(x[ 0], x[ 4], x[ 8], x[12]);	// column 1
		QR(x[ 5], x[ 9], x[13], x[ 1]);	// column 2
		QR(x[10], x[14], x[ 2], x[ 6]);	// column 3
		QR(x[15], x[ 3], x[ 7], x[11]);	// column 4
		// Even round
		QR(x[ 0], x[ 1], x[ 2], x[ 3]);	// row 1
		QR(x[ 5], x[ 6], x[ 7], x[ 4]);	// row 2
		QR(x[10], x[11], x[ 8], x[ 9]);	// row 3
		QR(x[15], x[12], x[13], x[14]);	// row 4
	}
	for (i = 0; i < 16; ++i)
		out[i] = x[i] + in[i];

#undef ROTL
#undef QR
}
//TODO SALSA

//Local functions
static inline void BlockXor(uint8* destsrc, const uint8* src, uint16 count)
{
	for(uint16 j = 0; j < count; j++)
		destsrc[j] = destsrc[j] xor src[j];
}

static FixedArray<uint8> BlockMix(const FixedArray<uint8>& block, uint8 r)
{
	FixedArray<uint8> result(static_cast<uint32>(2 * r * 64));

	uint8 X[64];
	MemCopy(X, &block[(2*r - 1) * 64], sizeof(X));
	for(uint16 i = 0; i < 2*r; i++)
	{
		//even blocks
		BlockXor(X, &block[i * 64], sizeof(X));
		salsa20_8_block((uint32*)X, (uint32*)X);
		MemCopy(&result[i * 64 / 2], X, sizeof(X));
		i++;

		//odd blocks
		BlockXor(X, &block[i * 64], sizeof(X));
		salsa20_8_block((uint32*)X, (uint32*)X);
		MemCopy(&result[r * 64 + (i-1) * 64 / 2], X, sizeof(X));
	}

	return result;
}

static uint64 Integerify(uint8* block, uint8 r)
{
	block += (2 * r - 1) * 64;

	BufferInputStream bufferInputStream(block, sizeof(uint64));
	DataReader dataReader(false, bufferInputStream);

	return dataReader.ReadUInt64();
}

static void ROMix(uint8* block, uint16 blockSize, uint32 nInterations, uint8 r)
{
	FixedArray<UniquePointer<FixedArray<uint8>>> V(nInterations);

	FixedArray<uint8> X(blockSize);
	MemCopy(&X[0], block, blockSize);
	for(uint32 i = 0; i < nInterations; i++)
	{
		FixedArray<uint8>* Vi = new FixedArray<uint8>(blockSize);
		MemCopy(&(*Vi)[0], &X[0], blockSize);
		V[i] = Move(Vi);

		X = BlockMix(X, r);
	}

	for(uint32 i = 0; i < nInterations; i++)
	{
		uint64 j = Integerify(&X[0], r) & (nInterations - 1); //mod nInterations
		BlockXor(&X[0], &((*V[j])[0]), static_cast<uint16>(X.GetNumberOfElements()));
		X = BlockMix(X, r);
	}

	//write result back to block
	MemCopy(block, &X[0], blockSize);
}

void StdXX::Crypto::scrypt(const String& password, const uint8* salt, uint8 saltSize, uint8* key, uint8 keySize, uint8 costFactor, uint8 blockSizeFactor, uint8 parallelizationFactor)
{
	//generate expensive salt
	uint16 blockSize = static_cast<uint16>(128 * blockSizeFactor);

	FixedArray<uint8> blocks(blockSize * parallelizationFactor);
	PBKDF2(password, salt, saltSize, HashAlgorithm::SHA256, 1, &blocks[0], static_cast<uint16>(blocks.GetNumberOfElements()));

	for(uint8 i = 0; i < parallelizationFactor; i++)
	{
		ROMix(&blocks[i * blockSize], blockSize, uint32(1) << costFactor, blockSizeFactor);
	}

	PBKDF2(password, &blocks[0], static_cast<uint16>(blocks.GetNumberOfElements()), HashAlgorithm::SHA256, 1, key, keySize);
}
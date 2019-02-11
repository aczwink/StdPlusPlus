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
#include "AESCipher.hpp"
//Local
#include <Std++/Memory.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Precalculated from http://en.wikipedia.org/wiki/Rijndael_key_schedule#Rcon
static const uint8 roundConstantTable[11] = {0x8D, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

//https://en.wikipedia.org/wiki/Rijndael_S-box
//Tables
static const uint8 substitutionBox[256] =
{
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

//Local functions
static inline void RotWord(byte (&refWord)[4])
{
	byte tmp;

	tmp = refWord[0];
	refWord[0] = refWord[1];
	refWord[1] = refWord[2];
	refWord[2] = refWord[3];
	refWord[3] = tmp;
}

static inline void SubWord(byte (&refWord)[4])
{
	refWord[0] = substitutionBox[refWord[0]];
	refWord[1] = substitutionBox[refWord[1]];
	refWord[2] = substitutionBox[refWord[2]];
	refWord[3] = substitutionBox[refWord[3]];
}

//Constructor
AESCipher::AESCipher(const byte *key, uint16 keyLength)
{
	switch(keyLength)
	{
		case 128:
			this->keyLength = 4;
			this->nRounds = 10;
			break;
		case 192:
			this->keyLength = 6;
			this->nRounds = 12;
			break;
		case 256:
			this->keyLength = 8;
			this->nRounds = 14;
			break;
		default:
			NOT_IMPLEMENTED_ERROR;
	}

	this->KeyExpansion(key);
}

//Public methods
void AESCipher::Encrypt(const byte *unencrypted, byte *encrypted) const
{
	MemCopy(this->state, unencrypted, sizeof(this->state));

	this->AddRoundKey(0);

	for(uint8 round = 1; round < this->nRounds; round++)
	{
		this->SubBytes();
		this->ShiftRows();
		this->MixColumns();
		this->AddRoundKey(round);
	}

	this->SubBytes();
	this->ShiftRows();
	this->AddRoundKey(this->nRounds);

	MemCopy(encrypted, this->state, sizeof(this->state));
}

uint8 AESCipher::GetBlockSize() const
{
	return 16;
}

//Private methods
void AESCipher::AddRoundKey(uint8 round) const
{
	for(uint8 i = 0; i < 4; i++)
	{
		for(uint8 j = 0; j < 4; j++)
		{
			this->state[i][j] = this->state[i][j] XOR this->roundKeys[round * AES_NUMBER_OF_BLOCKS * 4 + i * AES_NUMBER_OF_BLOCKS + j];
		}
	}
}

void AESCipher::KeyExpansion(const byte* key)
{
	uint8 i;
	uint8 tmp[4];

	MemCopy(this->roundKeys, key, sizeof(key));

	for(i = this->keyLength; i < AES_NUMBER_OF_BLOCKS * (this->nRounds + 1); i++)
	{
		tmp[0] = this->roundKeys[(i-1) * 4];
		tmp[1] = this->roundKeys[(i-1) * 4 + 1];
		tmp[2] = this->roundKeys[(i-1) * 4 + 2];
		tmp[3] = this->roundKeys[(i-1) * 4 + 3];

		if((i % this->keyLength) == 0)
		{
			RotWord(tmp);
			SubWord(tmp);
			tmp[0] = tmp[0] XOR roundConstantTable[i / this->keyLength];
		}
		else if(this->keyLength > 6 && (i % this->keyLength) == 4)
		{
			SubWord(tmp);
		}

		this->roundKeys[i * 4] = this->roundKeys[(i - this->keyLength) * 4] XOR tmp[0];
		this->roundKeys[i * 4 + 1] = this->roundKeys[(i - this->keyLength) * 4 + 1] XOR tmp[1];
		this->roundKeys[i * 4 + 2] = this->roundKeys[(i - this->keyLength) * 4 + 2] XOR tmp[2];
		this->roundKeys[i * 4 + 3] = this->roundKeys[(i - this->keyLength) * 4 + 3] XOR tmp[3];
	}
}

void AESCipher::MixColumns() const
{
	uint8 orig[4];
	uint8 mul2[4];
	uint8 mul3[4];

	//http://en.wikipedia.org/wiki/Rijndael_mix_columns

	for(uint8 i = 0; i < 4; i++)
	{
		for(uint8 c = 0; c < 4; c++)
		{
			orig[c] = this->state[i][c];
			mul2[c] = orig[c] << 1; //orig[c] * 2 (in Rijndael's Galois field)
			mul2[c] = mul2[c] XOR (0x1B & ((int8)orig[c] >> 7)); //something with overflow...
			mul3[c] = orig[c] XOR mul2[c];
		}
		this->state[i][0] = mul2[0] XOR mul3[1] XOR orig[2] XOR orig[3];
		this->state[i][1] = orig[0] XOR mul2[1] XOR mul3[2] XOR orig[3];
		this->state[i][2] = orig[0] XOR orig[1] XOR mul2[2] XOR mul3[3];
		this->state[i][3] = mul3[0] XOR orig[1] XOR orig[2] XOR mul2[3];
	}
}

void AESCipher::ShiftRows() const
{
	uint8 tmp;

	//state is [column][row]

	//row 1
	tmp = this->state[0][1];
	this->state[0][1] = this->state[1][1];
	this->state[1][1] = this->state[2][1];
	this->state[2][1] = this->state[3][1];
	this->state[3][1] = tmp;

	//row 2
	tmp = this->state[0][2];
	this->state[0][2] = this->state[2][2];
	this->state[2][2] = tmp;

	tmp = this->state[1][2];
	this->state[1][2] = this->state[3][2];
	this->state[3][2] = tmp;

	//Row 3
	tmp = this->state[0][3];
	this->state[0][3] = this->state[3][3];
	this->state[3][3] = this->state[2][3];
	this->state[2][3] = this->state[1][3];
	this->state[1][3] = tmp;
}

void AESCipher::SubBytes() const
{
	for(uint8 i = 0; i < 4; i++)
	{
		SubWord(this->state[i]);
	}
}
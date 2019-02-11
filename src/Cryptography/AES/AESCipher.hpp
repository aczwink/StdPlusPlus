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
//Local
#include <Std++/Cryptography/BlockCipher.hpp>

//Definitions
#define AES_MAX_KEYLENGTH 8
#define AES_MAX_NUMBER_OF_ROUNDS 14
#define AES_NUMBER_OF_BLOCKS 4

//Based on FIPS 197 AES
namespace _stdxx_
{
	class AESCipher : public StdXX::BlockCipher
	{
	public:
		//Constructor
		AESCipher(const byte* key, uint16 keyLength);

		//Methods
		void Encrypt(const byte *unencrypted, byte *encrypted) const override;
		uint8 GetBlockSize() const override;

	private:
		//Members
		uint8 keyLength; //in uint32s'
		uint8 nRounds;
		mutable byte state[4][4];
		byte roundKeys[4 * AES_NUMBER_OF_BLOCKS * (AES_MAX_NUMBER_OF_ROUNDS + 1)];

		//Methods
		void AddRoundKey(uint8 round) const;
		void KeyExpansion(const byte* key);
		void MixColumns() const;
		void ShiftRows() const;
		void SubBytes() const;
	};
}
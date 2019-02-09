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
#include <Std++/_Backends/Extension.hpp>
#include <Std++/Cryptography/Cipher.hpp>

namespace _stdxx_
{
	class OpenSSL_Extension : public Extension
	{
	public:
		//Methods
		void Load() override;
		void Unload() const override;

		//Functions
		static StdXX::UniquePointer<StdXX::Cipher> CreateCipher(StdXX::CipherAlgorithm algorithm, const byte* key, uint16 keyLength);
	};
}
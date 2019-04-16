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
#include "Counter.hpp"

namespace StdXX
{
	namespace Crypto
	{
		/**
		 * A counter that produces n-byte keystream blocks by concatenating a m-byte nonce with a l-byte counter (in big endian).
		 * The counter value must be an unsigned integer with a fixed bit length.
		 */
		template <uint32 nonceSize, typename CounterType>
		class DefaultCounter : public Counter
		{
		public:
			//Constructor
			inline DefaultCounter(const uint8* nonce, CounterType initialValue = 0)
			{
				MemCopy(this->keystreamBlock.nonce, nonce, nonceSize);
				this->counter = initialValue;
			}

			//Methods
			const void *GetKeyStreamBlock() const override
			{
#ifdef XPC_ENDIANNESS_LITTLE
				for(uint32 i = 0; i < sizeof(this->keystreamBlock.counter); i++)
					((uint8 *)this->keystreamBlock.counter)[i] = ((uint8*)&this->counter)[sizeof(CounterType) - i - 1];
#else
				MemCopy(this->keystreamBlock.counter, &this->counter, sizeof(CounterType));
#endif
				return &this->keystreamBlock;
			}

			uint8 GetKeyStreamBlockSize() const override
			{
				return nonceSize + sizeof(CounterType);
			}

			void Increment() override
			{
				this->counter++;
			}

		private:
			//Members
			struct
			{
				uint8 nonce[nonceSize];
				uint8 counter[sizeof(CounterType)];
			} keystreamBlock;
			CounterType counter;
		};
	}
}
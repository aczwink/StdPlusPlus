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
#include <Std++/Streams/OutputStream.hpp>

namespace _stdxx_
{
	class SlidingDictionary
	{
	public:
		//Constructor
		SlidingDictionary(uint32 size);

		//Destructor
		~SlidingDictionary();

		//Methods
		void Copy(uint16 distance, uint16 length, StdXX::OutputStream &refOutput);

		//Inline
		inline void Append(byte b)
		{
			this->pBuffer[this->index++] = b;

			if(this->index == this->size)
				this->index = 0;
		}

	private:
		//Members
		uint32 size;
		byte *pBuffer;
		uint32 index;
	};
}
/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
//Local
#include "../Definitions.h"

namespace ACStdLib
{
    namespace Multimedia
    {
		class Packet
		{
		private:
			//Members
			byte *data;
			uint32 size;
			uint32 capacity;

		public:
			//Members
			uint32 streamIndex;
			uint64 pts; //the presentation time stamp
			bool containsKeyframe;

			//Constructor
			Packet();

			//Destructor
			~Packet();

			//Methods
			void Allocate(uint32 size);

			//Inline
			inline byte *GetData()
			{
				return this->data;
			}

			inline const byte *GetData() const
			{
				return this->data;
			}

			inline uint32 GetSize() const
			{
				return this->size;
			}
		};
    }
}
/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include "../Definitions.h"
#include <Std++/Debug.hpp>
#include <Std++/Utility.hpp>

namespace StdXX
{
    namespace Multimedia
    {
		class STDPLUSPLUS_API Packet
		{
		public:
			//Members
			uint32 streamIndex;
			uint64 pts; //the presentation time stamp
			uint64 duration;
			bool containsKeyframe;

			//Constructors
			Packet();

			inline Packet(const Packet &source) : data(nullptr), capacity(0) //copy ctor
			{
				*this = source;
			}

			inline Packet(Packet &&source) : data(nullptr) //move ctor
			{
				*this = Move(source);
			}

			//Destructor
			~Packet();

			//Operators
			Packet &operator=(const Packet &source); //copy assign
			Packet &operator=(Packet &&source); //move assign

			//Methods
			void Allocate(uint32 size);
			void CopyAttributesFrom(const Packet& p);

			//Inline
			inline void AllocateAdditional(uint32 size)
			{
				this->Allocate(this->size + size);
			}

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

			inline void RemoveEnd(uint32 nBytes)
			{
				ASSERT(this->size > nBytes, u8"Can't remove more bytes than are in packet.");
				this->size -= nBytes;
			}

		private:
			//Members
			byte * data;
			uint32 size;
			uint32 capacity;
		};
    }
}
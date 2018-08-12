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
#include <Std++/Containers/LinkedList/LinkedList.hpp>

namespace StdXX
{
    namespace Multimedia
    {
        //Move declarations
		class Frame;
		class Packet;

        class EncoderContext
        {
        public:
            //Destructor
            virtual ~EncoderContext(){}

            //Methods
            virtual void Encode(const Frame &frame) = 0;
            virtual void Flush() = 0;

            //Inline
			inline Packet *GetNextPacket()
			{
				return this->orderedPackets.PopFront();
			}

			inline bool IsPacketReady() const
			{
				return !this->orderedPackets.IsEmpty();
			}

		protected:
        	//Inline
        	inline void AddPacket(Packet *packet)
			{
				this->orderedPackets.InsertTail(packet);
			}

		private:
        	//Members
			LinkedList<Packet *> orderedPackets;
        };
    }
}
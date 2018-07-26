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
#include "../Memory.h"
#include "NetAddress.hpp"

namespace StdXX
{
	class Datagram
	{
		friend class DatagramSocket;
	private:
		//Members
		byte *buffer;
		uint32 bufferSize;
		uint32 packetSize;
		NetAddress *sender;
		uint16 senderPort;

	public:
		//Constructor
		Datagram();

		//Destructor
		~Datagram();

		//Inline
		inline void Allocate(uint32 size)
		{
			if(size > this->bufferSize)
			{
				this->buffer = (byte *)MemRealloc(this->buffer, size);
				this->bufferSize = size;
			}
		}

		inline byte *GetBuffer()
		{
			return this->buffer;
		}

		inline uint32 GetBufferSize() const
		{
			return this->bufferSize;
		}

		inline uint32 GetPacketSize() const
		{
			return this->packetSize;
		}

		inline const NetAddress *GetSender() const
		{
			return this->sender;
		}

		inline uint16 GetSenderPort() const
		{
			return this->senderPort;
		}

		inline void Reset()
		{
			if(this->sender)
				delete this->sender;
			this->sender = nullptr;
			this->senderPort = 0;
			this->packetSize = 0;
		}
	};
}
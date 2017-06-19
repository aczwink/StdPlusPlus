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
//Class header
#include <ACStdLib/Multimedia/Packet.hpp>
//Local
#include <ACStdLib/Memory.h>
#include <ACStdLib/Natural.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

//Constructor
Packet::Packet()
{
	this->data = nullptr;
	this->size = 0;
	this->capacity = 0;
	this->pts = Natural<uint64>::Max();
	this->containsKeyframe = false;
}

//Destructor
Packet::~Packet()
{
	if(this->data)
		MemFree(this->data);
}

//Public methods
void Packet::Allocate(uint32 size)
{
	if(size > this->capacity)
	{
		this->data = (byte *)MemRealloc(this->data, size);
		this->capacity = size;
	}

	this->size = size;
}
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
#include <ACStdLib/Network/Datagram.hpp>
//Namespaces
using namespace ACStdLib;

//Constructor
Datagram::Datagram()
{
	this->bufferSize = 0;
	this->buffer = nullptr;
	this->packetSize = 0;
	this->sender = nullptr;
	this->senderPort = 0;

	this->Allocate(1500); //MTU
}

//Destructor
Datagram::~Datagram()
{
	if(this->buffer)
		MemFree(this->buffer);
	if(this->sender)
		delete this->sender;
}
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
//Class header
#include <Std++/Multimedia/ParserContext.hpp>
//Local
#include <Std++/Multimedia/Stream.hpp>
#include <Std++/Unsigned.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructor
ParserContext::ParserContext(Stream& stream) : stream(stream), repack(stream.parserFlags.repack)
{
}

//Destructor
ParserContext::~ParserContext()
{
	this->Reset();
}

//Public methods
void ParserContext::Reset()
{
	this->frameBuffer = Packet();
}

//Protected methods
void ParserContext::AddToFrameBuffer(const Packet &refPacket)
{
	this->AddToFrameBuffer(refPacket.GetData(), refPacket.GetSize());

	this->frameBuffer.streamIndex = refPacket.streamIndex;
	if (this->frameBuffer.pts == Unsigned<uint64>::Max() && refPacket.pts != Unsigned<uint64>::Max())
		this->frameBuffer.pts = refPacket.pts;
	if (refPacket.containsKeyframe)
		this->frameBuffer.containsKeyframe = true;
}

void ParserContext::AddToFrameBuffer(const void *pData, uint32 size)
{
	uint32 offset = this->frameBuffer.GetSize();
	this->frameBuffer.AllocateAdditional(size);
	MemCopy(this->frameBuffer.GetData() + offset, pData, size);
}

void ParserContext::ReadyFrameBuffer(uint32 nOverreadBytes)
{
	Packet parsedBuffer = Move(this->frameBuffer);
	this->frameBuffer = Packet();
	if (nOverreadBytes)
	{
		//Put the overread bytes in the next frame buffer
		byte *data = parsedBuffer.GetData() + parsedBuffer.GetSize() - nOverreadBytes;
		this->AddToFrameBuffer(data, nOverreadBytes);
	}
	
	parsedBuffer.RemoveEnd(nOverreadBytes); //remove overread bytes from parser buffer
	this->parsedFrames.InsertTail(parsedBuffer);
}
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
#include <Std++/Multimedia/DecoderContext.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

//Destructor
DecoderContext::~DecoderContext()
{
	this->Reset();
}

//Protected methods
void DecoderContext::AddFrame(Frame *pFrame, uint32 frameNumber)
{
	this->unorderedFrames.Insert({frameNumber, pFrame});

	//Flush the ready frames
	while(!this->unorderedFrames.IsEmpty() && this->unorderedFrames.GetFirst().Get<0>() <= this->frameCounter)
	{
		this->orderedFrames.InsertTail(this->unorderedFrames.PopFirst().Get<1>());
		this->frameCounter++;
	}
}

//Public methods
void DecoderContext::Reset()
{
	while(!this->unorderedFrames.IsEmpty())
		delete this->unorderedFrames.PopFirst().Get<1>();

	for(const Frame *const& refpFrame : this->orderedFrames)
		delete refpFrame;
	this->orderedFrames.Release();
}
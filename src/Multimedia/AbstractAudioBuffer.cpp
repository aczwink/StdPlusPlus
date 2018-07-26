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
#include <Std++/Multimedia/AbstractAudioBuffer.hpp>
//Local
#include <Std++/Debug.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

//Public methods
uint8 AbstractAudioBuffer::GetNumberOfChannels() const
{
	switch(this->GetChannelLayout())
	{
		case ChannelLayout::Mono:
			return 1;
		case ChannelLayout::Stereo:
			return 2;
	}

	NOT_IMPLEMENTED_ERROR;
	return -1;
}
/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/PacketWrapper.hpp>
//Namespaces
using namespace StdXX::Multimedia;

bool PacketWrapper::ContainsKeyFrame() const
{
	return this->packet.ContainsKeyFrame();
}

const uint8 *PacketWrapper::GetData() const
{
	return this->packet.GetData();
}

uint64 PacketWrapper::GetDecodeTimestamp() const
{
	return this->dts;
}

uint64 PacketWrapper::GetPresentationTimestamp() const
{
	return this->pts;
}

uint32 PacketWrapper::GetSize() const
{
	return this->packet.GetSize();
}

uint32 PacketWrapper::GetStreamIndex() const
{
	return this->streamIndex;
}
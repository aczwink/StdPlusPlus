/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include "libav_Packet.hpp"
//Local
#include <Std++/Debug.hpp>
//Namespaces
using namespace _stdxx_;

//Public methods
bool libav_Packet::ContainsKeyFrame() const
{
	return this->pkt.flags & AV_PKT_FLAG_KEY;
}

const uint8 *libav_Packet::GetData() const
{
	return this->pkt.data;
}

uint64 libav_Packet::GetPresentationTimestamp() const
{
	return this->pkt.pts;
}

uint32 libav_Packet::GetSize() const
{
	return this->pkt.size;
}

uint32 libav_Packet::GetStreamIndex() const
{
	return this->pkt.stream_index;
}
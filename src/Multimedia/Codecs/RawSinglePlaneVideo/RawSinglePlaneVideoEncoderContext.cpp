/*
 * Copyright (c) 2017-2024 Amir Czwink (amir130@hotmail.de)
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
#include "RawSinglePlaneVideoEncoderContext.hpp"
//Local
#include <Std++/Multimedia/Packet.hpp>

//Public methods
void RawSinglePlaneVideoEncoderContext::Encode(const Frame& frame)
{
	const auto& fmt = *this->Parameters().video.pixelFormat;
	const auto& size = this->Parameters().video.size;

	ASSERT_EQUALS(1, fmt.nPlanes);

	uint32 imageSize = fmt.ComputeLineSize(0, size.width) * size.height;

	Packet* packet = new Packet;
	packet->Allocate(imageSize);
	MemCopy(packet->GetData(), frame.GetPixmap()->GetPlane(0), imageSize);

	this->AddPacket(packet);
}

void RawSinglePlaneVideoEncoderContext::Flush()
{
	//this encoder always writes through
}
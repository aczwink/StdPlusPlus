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
#pragma once
//Local
#include <Std++/Optional.hpp>
#include "IPacket.hpp"

namespace StdXX::Multimedia
{
	class PacketWrapper : public IPacket
	{
	public:
		//Members
		uint64 dts;
		uint64 pts;
		uint32 streamIndex;

		//Constructor
		inline PacketWrapper(const IPacket& packet) : packet(packet)
		{
			this->dts = packet.GetDecodeTimestamp();
			this->pts = packet.GetPresentationTimestamp();
			this->streamIndex = packet.GetStreamIndex();
		}

		//Methods
		bool ContainsKeyFrame() const override;
		const uint8 *GetData() const override;
		uint64 GetDecodeTimestamp() const override;
		uint64 GetPresentationTimestamp() const override;
		uint32 GetSize() const override;
		uint32 GetStreamIndex() const override;

	private:
		//Members
		const IPacket& packet;
	};
}
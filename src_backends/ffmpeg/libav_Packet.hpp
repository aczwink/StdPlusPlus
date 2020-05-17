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
//Global
extern "C"
{
#include <libavcodec/avcodec.h>
}
//Local
#include <Std++/Multimedia/IPacket.hpp>
#include <Std++/Definitions.h>

namespace _stdxx_
{
	class libav_Packet : public StdXX::Multimedia::IPacket
	{
		friend class libavformat_Demuxer;

	public:
		//Methods
		bool ContainsKeyFrame() const override;
		const uint8 *GetData() const override;
		uint64 GetPresentationTimestamp() const override;
		uint32 GetSize() const override;
		uint32 GetStreamIndex() const override;

	private:
		//Members
		AVPacket pkt;
	};
}
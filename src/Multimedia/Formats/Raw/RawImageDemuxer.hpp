/*
 * Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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
//Local
#include <Std++/Multimedia/Demuxer.hpp>

namespace _stdxx_
{
	class RawImageDemuxer : public StdXX::Multimedia::Demuxer
	{
	public:
		//Constructor
		inline RawImageDemuxer(const StdXX::Multimedia::ContainerFormat &format, StdXX::SeekableInputStream &inputStream, StdXX::Multimedia::CodingFormatId id)
				: Demuxer(format, inputStream), codingFormatId(id)
		{
		}

		//Methods
		void ReadHeader() override;
		StdXX::UniquePointer<StdXX::Multimedia::IPacket> ReadPacket() override;
		void Seek(uint64 timestamp, const StdXX::Multimedia::TimeScale &timeScale) override;

	private:
		//Members
		StdXX::Multimedia::CodingFormatId codingFormatId;
	};
}
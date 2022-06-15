/*
 * Copyright (c) 2020,2022 Amir Czwink (amir130@hotmail.de)
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
#include <libavformat/avformat.h>
}
//Local
#include <Std++/Streams/SeekableInputStream.hpp>
#include <Std++/Multimedia/Demuxer.hpp>
#include "libavio_InputStreamWrapper.hpp"

namespace _stdxx_
{
	//Forward declarations
	class libavformat_Format;

	class libavformat_Demuxer : public StdXX::Multimedia::Demuxer
	{
	public:
		//Constructor
		libavformat_Demuxer(const libavformat_Format& format, StdXX::SeekableInputStream& seekableInputStream);

		//Destructor
		~libavformat_Demuxer() override;

		//Methods
		void ReadHeader() override;
		void Seek(uint64 timestamp, const StdXX::Multimedia::TimeScale &timeScale) override;

	private:
		//Members
		const AVInputFormat* avInputFormat;
		AVFormatContext *fmt_ctx;
		libavio_InputStreamWrapper streamWrapper;

		//Methods
		StdXX::UniquePointer<StdXX::Multimedia::IPacket> ReadPacket() override;
	};
}
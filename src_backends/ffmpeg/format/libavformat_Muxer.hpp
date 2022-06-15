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
#include <Std++/Multimedia/Muxer.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>
#include "libavio_OutputStreamWrapper.hpp"

namespace _stdxx_
{
	//Forward declarations
	class libavformat_Format;

	class libavformat_Muxer : public StdXX::Multimedia::Muxer
	{
	public:
		//Constructor
		libavformat_Muxer(const libavformat_Format& format, StdXX::SeekableOutputStream& seekableOutputStream);

		//Destructor
		~libavformat_Muxer() override;

		//Methods
		void Finalize() override;
		void WriteHeader() override;
		void WritePacket(const StdXX::Multimedia::IPacket& packet) override;

	private:
		//Members
		const AVOutputFormat* avOutputFormat;
		AVFormatContext *fmt_ctx;
		StdXX::UniquePointer<libavio_OutputStreamWrapper> streamWrapper;
		StdXX::DynamicArray<uint8*> extradata;
	};
}
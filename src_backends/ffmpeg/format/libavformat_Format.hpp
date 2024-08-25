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
#include <Std++/Multimedia/ContainerFormat.hpp>

namespace _stdxx_
{
	class libavformat_Format : public StdXX::Multimedia::ContainerFormat
	{
	public:
		//Constructor
		libavformat_Format(const char* shortName);

		//Properties
		inline const AVInputFormat* InputFormat() const
		{
			return this->avInputFormat;
		}

		inline const AVOutputFormat* OutputFormat() const
		{
			return this->avOutputFormat;
		}

		//Methods
		StdXX::Multimedia::Demuxer *CreateDemuxer(StdXX::SeekableInputStream &refInput) const override;
		StdXX::Multimedia::Muxer *CreateMuxer(StdXX::SeekableOutputStream &refOutput) const override;
		StdXX::String GetExtension() const override;
		void GetFormatInfo(StdXX::Multimedia::FormatInfo &refFormatInfo) const override;
		StdXX::String GetName() const override;
		StdXX::DynamicArray<const StdXX::Multimedia::CodingFormat *> GetSupportedCodingFormats(StdXX::Multimedia::DataType dataType) const override;
		float32 Probe(StdXX::BufferInputStream &refBuffer) const override;

	private:
		//Members
		const AVInputFormat* avInputFormat;
		const AVOutputFormat* avOutputFormat;
	};
}
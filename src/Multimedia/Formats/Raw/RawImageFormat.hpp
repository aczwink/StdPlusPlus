/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/Format.hpp>
#include <Std++/Multimedia/CodingFormatId.hpp>

namespace _stdxx_
{
	class RawImageFormat : public StdXX::Multimedia::Format
	{
	public:
		//Constructor
		inline RawImageFormat(StdXX::Multimedia::CodingFormatId codingFormatId) : codingFormatId(codingFormatId)
		{
		}

		//Methods
		StdXX::Multimedia::Demuxer *CreateDemuxer(StdXX::SeekableInputStream &refInput) const override;
		StdXX::Multimedia::Muxer *CreateMuxer(StdXX::SeekableOutputStream &outputStream) const override;
		//StdXX::Multimedia::CodecId GetDefaultCodec(StdXX::Multimedia::DataType dataType) const override;
		void GetFormatInfo(StdXX::Multimedia::FormatInfo &refFormatInfo) const override;
		StdXX::String GetName() const override;

	private:
		//Members
		StdXX::Multimedia::CodingFormatId codingFormatId;
	};
}
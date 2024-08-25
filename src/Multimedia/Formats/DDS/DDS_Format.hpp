/*
 * Copyright (c) 2023-2024 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/ContainerFormat.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
#include "DDS_Muxer.hpp"
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

class DDS_Format : public ContainerFormat
{
public:
	Demuxer *CreateDemuxer(SeekableInputStream &inputStream) const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return nullptr;
	}

	Muxer *CreateMuxer(SeekableOutputStream &outputStream) const override
	{
		return new DDS_Muxer(*this, outputStream);
	}

	String GetExtension() const override
	{
		return "dds";
	}

	void GetFormatInfo(FormatInfo &formatInfo) const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
	}

	String GetName() const override
	{
		return "DirectDraw Surface";
	}

	DynamicArray<const CodingFormat *> GetSupportedCodingFormats(DataType dataType) const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return DynamicArray<const CodingFormat *>();
	}

	float32 Probe(BufferInputStream& inputStream) const override
	{
		DataReader dataReader(false, inputStream);

		if(dataReader.ReadUInt32() == FOURCC(u8"DDS "))
			return 1;
		return 0;
	}
};
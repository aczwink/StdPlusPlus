/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
#include "../QuickTime/QuickTimeDemuxer.hpp"
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

class QuickTimeFormat : public ContainerFormat
{
public:
	Demuxer *CreateDemuxer(SeekableInputStream &inputStream) const override
	{
		return new QuickTimeDemuxer(*this, inputStream);
	}

	Muxer *CreateMuxer(SeekableOutputStream &outputStream) const override
	{
		return nullptr;
	}

	String GetExtension() const override
	{
		return u8"mov";
	}

	void GetFormatInfo(FormatInfo &formatInfo) const override
	{
		formatInfo.supportsByteSeeking = false;
	}

	String GetName() const override
	{
		return u8"Apple QuickTime";
	}

	DynamicArray<const CodingFormat *> GetSupportedCodingFormats(DataType dataType) const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: imple
		return DynamicArray<const CodingFormat *>();
	}

	float32 Probe(BufferInputStream &inputStream) const override
	{
		DataReader dataReader(false, inputStream);

		dataReader.Skip(4);
		bool test1 = dataReader.ReadUInt32() == FOURCC(u8"ftyp");
		bool test2 = dataReader.ReadUInt32() == FOURCC(u8"qt  ");
		if(test1 and test2)
			return 1;
		return 0;
	}
};
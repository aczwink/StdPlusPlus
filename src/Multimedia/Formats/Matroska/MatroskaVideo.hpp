/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

class MatroskaVideo : public ContainerFormat
{
public:
	//Methods
	Demuxer *CreateDemuxer(SeekableInputStream &refInput) const;
	Muxer *CreateMuxer(SeekableOutputStream &refOutput) const;
	String GetExtension() const;
	void GetFormatInfo(FormatInfo &refFormatInfo) const;
	String GetName() const;
	DynamicArray<const CodingFormat *> GetSupportedCodingFormats(DataType dataType) const override;
	float32 Probe(BufferInputStream &buffer) const;
};
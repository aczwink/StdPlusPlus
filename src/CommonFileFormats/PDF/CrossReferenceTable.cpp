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
//Class header
#include <Std++/CommonFileFormats/PDF/CrossReferenceTable.hpp>
//Local
#include <Std++/Streams/Writers/TextWriter.hpp>
//Namespaces
using namespace StdXX::CommonFileFormats::PDF;

//Public methods
uint32 CrossReferenceTable::Write(SeekableOutputStream &outputStream)
{
	uint8 i;
	String tmp;

	TextWriter writer(outputStream, TextCodecType::ASCII);

	for(auto &refKV : this->indirectObjects)
	{
		this->WriteIndirectObject(refKV.value, outputStream);
	}

	uint32 offset = outputStream.QueryCurrentOffset();
	writer.WriteString("xref\n");

	//7.5.4: For a file that has never been incrementally updated, the cross-reference section shall contain only one subsection, whose object numbering begins at 0.
	writer.WriteString("0 ");
	writer.WriteString(String::Number(this->indirectObjects.GetNumberOfElements() + 1));
	writer.WriteString(u8"\n");

	//means first entry is always free
	writer.WriteString("0000000000 65535 f \n");

	for(const auto &refKV : this->indirectObjects)
	{
		tmp = String::Number(refKV.value.offset);
		for(i = 0; i < 10 - tmp.GetLength(); i++)
		{
			writer << u8'0';
		}
		writer << tmp << u8' ';

		tmp = String::Number(refKV.value.pObject->generationNumber);
		for(i = 0; i < 5 - tmp.GetLength(); i++)
		{
			writer << u8'0';
		}
		writer << tmp << " n \n";
	}

	return offset;
}

//Private methods
void CrossReferenceTable::WriteIndirectObject(XRefEntry &refEntry, SeekableOutputStream& outputStream)
{
	TextWriter writer(outputStream, TextCodecType::ASCII);

	refEntry.offset = outputStream.QueryCurrentOffset();
	writer << refEntry.pObject->objectNumber << u8' ' << refEntry.pObject->generationNumber << " obj\n";
	refEntry.pObject->Write(outputStream);
	writer << "endobj\n";
}
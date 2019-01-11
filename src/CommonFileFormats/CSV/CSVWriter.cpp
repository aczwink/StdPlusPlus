#include <Std++/Streams/OutputStream.hpp>

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
//Class header
#include <Std++/CommonFileFormats/CSV/CSVWriter.hpp>
//Local
#include <Std++/Streams/Writers/TextWriter.hpp>
#include <Std++/Streams/StdOut.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::CommonFileFormats;

//Public methods
void CSVWriter::TerminateRow()
{
	TextWriter writer(this->outputStream, TextCodecType::UTF8);

	writer.WriteString(this->dialect.lineSeparator);

	this->writeSeparator = false;
}

void CSVWriter::WriteCell(const String &string)
{
	TextWriter writer(this->outputStream, TextCodecType::UTF8);

	if(this->writeSeparator)
		writer.WriteString(this->dialect.separator);

	if(string.Find(this->dialect.separator) != Unsigned<uint32>::Max())
	{
		stdErr << string;
	}

	ASSERT(string.Find(this->dialect.separator) == Unsigned<uint32>::Max(), u8"TODO: escape cell");
	writer.WriteString(string);
	this->writeSeparator = true;
}
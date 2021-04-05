/*
* Copyright (c) 2018,2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/CommonFileFormats/CSV/CSVReader.hpp>
//Local
#include <Std++/Containers/Strings/ConstStringIterator.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::CommonFileFormats;

//Public methods
CSVReader::CellReadingResult CSVReader::ReadCell(String &cell)
{
	String tmp;
	while (!this->reader.IsAtEnd())
	{
		uint32 codePoint = this->reader.ReadCodePoint();
		tmp += codePoint;

		if(tmp == this->dialect.quote)
			return this->ReadQuotedString(cell);
		else if(tmp.EndsWith(this->dialect.separator))
		{
			cell = tmp.SubString(0, tmp.GetLength() - this->dialect.separator.GetLength());
			return CellReadingResult::NormalCell;
		}
		else if(tmp.EndsWith(this->dialect.lineSeparator))
		{
			cell = tmp.SubString(0, tmp.GetLength() - this->dialect.lineSeparator.GetLength());
			return CellReadingResult::LastCellInRow;
		}
	}

	NOT_IMPLEMENTED_ERROR;
}

DynamicArray<String> CSVReader::ReadRow()
{
	String cell;
	DynamicArray<String> result;

	while(this->ReadCell(cell) != CellReadingResult::LastCellInRow)
		result.Push(cell);
	result.Push(cell);


	return result;
}

//Private methods
CSVReader::CellReadingResult CSVReader::ReadCellEnding()
{
	String tmp;
	while (!this->reader.IsAtEnd())
	{
		uint32 codePoint = this->reader.ReadCodePoint();
		tmp += codePoint;

		if(tmp == this->dialect.separator)
			return CellReadingResult::NormalCell;
		else if(tmp == this->dialect.lineSeparator)
			return CellReadingResult::LastCellInRow;
	}

	NOT_IMPLEMENTED_ERROR;
}

CSVReader::CellReadingResult CSVReader::ReadQuotedString(String &cell)
{
	String tmp;
	while (!this->reader.IsAtEnd())
	{
		uint32 codePoint = this->reader.ReadCodePoint();
		tmp += codePoint;

		if(tmp.EndsWith(this->dialect.quote))
		{
			cell = tmp.SubString(0, tmp.GetLength() - this->dialect.quote.GetLength());
			return this->ReadCellEnding();
		}
	}

	NOT_IMPLEMENTED_ERROR;
}
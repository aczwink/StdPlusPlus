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
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::CommonFileFormats;

//Public methods
bool CSVReader::ReadCell(String &cell)
{
	uint32 minLength = Math::Min(this->dialect.lineSeparator.GetLength(), this->dialect.quote.GetLength());
	minLength = Math::Min(minLength, this->dialect.separator.GetLength());

	String tmp;
	for(uint32 i = 0; i < minLength; i++)
		tmp += this->reader.ReadCodePoint();
	//TODO: FIX THIS
	TODO FIX THIS






	String tmp;
	bool sepMatch = false;
	auto sepIterator = this->dialect.separator.begin();
	bool lsepMatch = false;
	auto lsepIterator = this->dialect.lineSeparator.begin();
	while (!this->reader.IsAtEnd())
	{
		uint32 codePoint = this->reader.ReadCodePoint();
		//check if tmp is still valid
		if (sepMatch)
		{
			if (codePoint != *sepIterator)
			{
				cell += tmp;
				tmp = String();
				sepMatch = false;
				sepIterator = this->dialect.separator.begin();
			}
		}
		if (lsepMatch)
		{
			if (codePoint != *lsepIterator)
			{
				cell += tmp;
				tmp = String();
				lsepMatch = false;
				lsepIterator = this->dialect.lineSeparator.begin();
			}
		}

		//check if codePoint is part of iterator		
		bool consumed = false;
		if (codePoint == *sepIterator)
		{
			++sepIterator;
			if (sepIterator == this->dialect.separator.end())
				return true;
			consumed = true;
		}
		if (codePoint == *lsepIterator)
		{
			++lsepIterator;
			if (lsepIterator == this->dialect.lineSeparator.end())
				return true;
			consumed = true;
		}

		if (!consumed)
			cell += codePoint;
	}

	return false;
}
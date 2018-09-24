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
#include <Std++/CommonFileFormats/CSV/CSVReader.hpp>
//Local
#include <Std++/Containers/Strings/ConstStringIterator.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::CommonFileFormats;

//Public methods
bool CSVReader::ReadCell(String &cell)
{
	cell = String();

	String tmp;
	bool sepMatch = false;
	auto sepIterator = this->dialect.separator.begin();
	bool lsepMatch = false;
	auto lsepIterator = this->dialect.lineSeparator.begin();
	while (true)
	{
		uint32 codePoint = this->reader.ReadCodePoint();
		if (this->reader.IsAtEnd())
			break;

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
			if (sepIterator == this->dialect.separator.end())
				return true;
			consumed = true;
		}

		if (!consumed)
			cell += codePoint;
	}

	return false;
}
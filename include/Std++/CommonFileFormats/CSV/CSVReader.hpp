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
#pragma once
//Local
#include <Std++/Streams/Readers/TextReader.hpp>
#include "CSVDialect.hpp"

namespace StdXX
{
	namespace CommonFileFormats
	{
		class STDPLUSPLUS_API CSVReader
		{
		public:
			//Constructor
			inline CSVReader(TextReader &reader, const CSVDialect &dialect) : reader(reader), dialect(dialect)
			{
			}

			//Methods
			bool ReadCell(String &cell);

		private:
			//Members
			TextReader &reader;
			const CSVDialect &dialect;
		};
	}
}
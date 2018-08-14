/*
* Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
namespace StdXX
{
	enum EBMLClassId
	{
		EBML_ID_EBML = 0x1A45DFA3,
	};

	struct EBMLHeader
	{
		uint64 version = 1;
		uint8 maxIdLength = 4;
		uint8 maxSizeLength = 8;
		String docType;
		uint32 docTypeVersion = 1;
	};
}
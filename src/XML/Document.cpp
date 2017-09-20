/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include <ACStdLib/XML/Document.hpp>
//Local
#include "CXMLParser.hpp"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::XML;

//Constructor
Document::Document()
{
	this->pRootElement = nullptr;
}

//Destructor
Document::~Document()
{
	if (this->pRootElement)
		delete this->pRootElement;
}

//Class Functions
Document *Document::Parse(InputStream &refInput)
{
	CXMLParser parser(refInput);

	return parser.Parse();
}
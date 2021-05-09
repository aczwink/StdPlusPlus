/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
//Class Header
#include "DOMReader.hpp"
//Local
#include <Std++/Debug.hpp>
#include <Std++/Memory.hpp>
#include <Std++/Mathematics.hpp>
//Namespaces
using namespace StdXX::Math;

//Constructor
DOMReader::DOMReader(InputStream &inputStream) : input(inputStream)
{
}

//Public methods
Document *DOMReader::Parse()
{
	Document *document = new Document;
	this->document = document;

	Parser parser(this->input);
	parser.Parse(*this);

	return document;
}

//Parser callbacks
void DOMReader::OnBeginElement(String &&elementName, BinaryTreeMap<String, String> &&attributes)
{
	Element* element = new Element(Move(elementName), Move(attributes));

	this->elementStack.Push(element);
}

void DOMReader::OnEndElement(String &&elementName)
{
	Element* element = this->elementStack.Pop();
	ASSERT_EQUALS(element->Name(), elementName);

	if(this->elementStack.IsEmpty())
		this->document->SetRootElement(element);
	else
		this->elementStack.Last()->AppendChild(element);
}

void DOMReader::OnText(String &&text)
{
	this->elementStack.Last()->AppendChild(new TextNode(Move(text)));
}

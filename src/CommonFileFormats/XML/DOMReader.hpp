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
#pragma once
//Local
#include <Std++/Streams/BufferedInputStream.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>
#include <Std++/CommonFileFormats/XML/Document.hpp>
#include <Std++/CommonFileFormats/XML/TextNode.hpp>
#include <Std++/CommonFileFormats/XML/Parser.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::CommonFileFormats::XML;

class DOMReader : private ParserCallbacks
{
public:
	//Constructor
	DOMReader(InputStream &inputStream);

	//Methods
	Document *Parse();

private:
	//Members
	Document *document;
	BufferedInputStream input;
	DynamicArray<Element*> elementStack;

	//Parser callbacks
	void OnBeginElement(String &&elementName, BinaryTreeMap<String, String> &&attributes) override;
	void OnEndElement(String &&elementName) override;
	void OnText(String &&text) override;
};
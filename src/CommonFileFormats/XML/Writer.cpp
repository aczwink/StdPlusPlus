/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/CommonFileFormats/XML/Writer.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::CommonFileFormats::XML;

//Public methods
void Writer::BeginElement(const String &tagName)
{
	ASSERT(!tagName.Contains(u8" "), u8"Spaces are not allowed inside element names");

	this->OnAddingChild(NodeType::Element);

	this->Indent();
	textWriter.WriteString(u8"<" + tagName);

	this->elementStack.Push({ElementStateType::AfterBegin, tagName});
}

void Writer::EndElement()
{
	ElementState& state = this->elementStack.Last();

	switch(state.type)
	{
		case ElementStateType::AfterBegin:
			this->textWriter.WriteLine(u8" />");
			break;
		case ElementStateType::AtChildren:
			this->indention--;
			this->Indent();
			this->WriteEndTag(state.tagName);
			break;
		case ElementStateType::HaveOneTextNodeChild:
			this->WriteStringWithEscaping(state.text);
			this->WriteEndTag(state.tagName);
			break;
	}

	this->elementStack.Pop();
}

//Private methods
void Writer::OnAddingChild(uint32 index, NodeType nodeType, const String& text)
{
	ElementState& state = this->elementStack[index];

	switch(state.type)
	{
		case ElementStateType::AfterBegin:
		{
			this->textWriter.WriteString(u8">");

			switch(nodeType)
			{
				case NodeType::Element:
					this->textWriter.WriteLine();
					this->indention++;
					state.type = ElementStateType::AtChildren;
					break;
				case NodeType::TextNode:
					state.text = text;
					state.type = ElementStateType::HaveOneTextNodeChild;
					break;
			}
		}
		break;
		case ElementStateType::AtChildren:
			if(nodeType == NodeType::TextNode)
			{
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			}
			break;
		case ElementStateType::HaveOneTextNodeChild:
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			break;
	}
}

void Writer::WriteStringWithEscaping(const String &text)
{
	for(uint32 codePoint : text)
	{
		switch(codePoint)
		{
			case u8'"':
				this->textWriter.WriteString(u8"&quot;");
				break;
			case u8'\'':
				this->textWriter.WriteString(u8"&apos;");
				break;
			case u8'<':
				this->textWriter.WriteString(u8"&lt;");
				break;
			case u8'>':
				this->textWriter.WriteString(u8"&gt;");
				break;
			case u8'&':
				this->textWriter.WriteString(u8"&amp;");
				break;
			default:
				this->textWriter.WriteCodePoint(codePoint);
		}
	}
}

/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Map/Map.hpp>
#include <Std++/Streams/OutputStream.hpp>
#include <Std++/Streams/Writers/TextWriter.hpp>
#include "Node.hpp"

namespace StdXX::CommonFileFormats::XML
{
	class Writer
	{
		enum class ElementStateType
		{
			AfterBegin,
			AtChildren,
			HaveOneTextNodeChild,
		};
		struct ElementState
		{
			ElementStateType type;
			String tagName;
			String text;
		};
	public:
		//Constructor
		inline Writer(OutputStream& outputStream) : textWriter(outputStream, TextCodecType::UTF8)
		{
			this->indention = 0;

			this->WriteProlog();
		}

		//Methods
		void BeginElement(const String& tagName);
		void EndElement();

		//Inline
		inline void WriteAttribute(const String& key, const String& value)
		{
			this->textWriter.WriteString(u8" " + key + u8"=\"");
			this->WriteStringWithEscaping(value);
			this->textWriter.WriteString(u8"\"");
		}

		inline void WriteText(const String& text)
		{
			this->OnAddingChild(NodeType::TextNode, text);
		}

	private:
		//Members
		TextWriter textWriter;
		uint16 indention;
		DynamicArray<ElementState> elementStack;

		//Methods
		void OnAddingChild(uint32 index, NodeType nodeType, const String& text);
		void WriteStringWithEscaping(const String& text);

		//Inline
		inline void Indent()
		{
			this->textWriter.WriteTabs(this->indention);
		}

		inline void OnAddingChild(NodeType nodeType, const String& text = {})
		{
			if(this->elementStack.IsEmpty())
				return;
			this->OnAddingChild(this->elementStack.GetNumberOfElements()-1, nodeType, text);
		}

		inline void WriteEndTag(const String& tagName)
		{
			this->textWriter.WriteLine(u8"</" + tagName + u8">");
		}

		inline void WriteProlog()
		{
			this->textWriter.WriteLine(u8"<?xml version=\"1.0\" encoding=\"utf-8\" ?>");
		}
	};
}
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
#include <Std++/Tuple.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>
#include <Std++/CommonFileFormats/XML/TextNode.hpp>
#include "Std++/CommonFileFormats/XML/Document.hpp"
#include "Binding.hpp"

namespace StdXX::Serialization
{
	class XmlDeserializer
	{
	public:
		//Constructor
		inline XmlDeserializer(InputStream& inputStream) : inputStream(inputStream)
		{
			this->document = CommonFileFormats::XML::Document::Parse(this->inputStream);
		}

		//Operators
		template <typename T>
		inline XmlDeserializer& operator&(const Binding<T>& binding)
		{
			return *this >> binding;
		}

		template <typename T>
		XmlDeserializer& operator>>(const Binding<T>& binding)
		{
			this->EnterElement(binding.name);
			*this >> binding.value;
			this->LeaveElement();

			return *this;
		}

		inline void operator>>(String& value)
		{
			const auto& children = this->elementStack.Last()->Children();
			ASSERT(children.GetNumberOfElements() == 1, u8"REPORT THIS PLEASE");
			ASSERT(children.Last()->GetType() == CommonFileFormats::XML::NodeType::TextNode, u8"REPORT THIS PLEASE!");
			value = dynamic_cast<CommonFileFormats::XML::TextNode *>(children.Last())->Text();
		}

		//Inline
		inline void EnterElement(const String& tagName)
		{
			if(this->elementStack.IsEmpty())
			{
				ASSERT(tagName == this->document->Root().Name(), u8"REPORT THIS PLEASE!");
				this->elementStack.Push(&this->document->Root());
			}
			else
				this->elementStack.Push(this->FirstChildElementWithTagName(*this->elementStack.Last(), tagName));
		}

		inline bool MoreChildrenExistsAtCurrentLevel() const
		{
			return !this->elementStack.Last()->Children().IsEmpty();
		}

		inline void LeaveElement()
		{
			CommonFileFormats::XML::Element* elementToRemove = this->elementStack.Pop();

			if(!this->elementStack.IsEmpty())
			{
				this->elementStack.Last()->RemoveChild(*elementToRemove);
				delete elementToRemove;
			}
		}

	private:
		//Members
		InputStream& inputStream;
		UniquePointer<CommonFileFormats::XML::Document> document;
		DynamicArray<CommonFileFormats::XML::Element*> elementStack;

		//Operators
		inline void operator>>(int32& value)
		{
			String tmp;
			*this >> tmp;

			value = tmp.ToInt32();
		}

		template <typename T>
		inline void operator>>(T& obj)
		{
			Archive(*this, obj);
		}

		//Methods
		CommonFileFormats::XML::Element* FirstChildElementWithTagName(CommonFileFormats::XML::Element& element, const String& tagName);
	};
}
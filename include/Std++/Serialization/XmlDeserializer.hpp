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
			this->inAttributes = false;
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
			this->EnterElementOrAttribute(binding.name);
			*this >> binding.value;
			this->LeaveElementOrAttribute();

			return *this;
		}

		inline void operator>>(String& value)
		{
			if(this->inAttributes)
				value = this->elementStack.Last()->GetAttribute(this->currentAttributeName);
			else
			{
				const auto &children = this->elementStack.Last()->Children();
				ASSERT(children.GetNumberOfElements() == 1, u8"REPORT THIS PLEASE");
				ASSERT(children.Last()->GetType() == CommonFileFormats::XML::NodeType::TextNode,
					   u8"REPORT THIS PLEASE!");
				value = dynamic_cast<CommonFileFormats::XML::TextNode *>(children.Last())->Text();
			}
		}

		//Inline
		inline void EnterAttributes()
		{
			this->inAttributes = true;
		}

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

		inline bool HasChildElement(const String& tagName) const
		{
			return this->FirstChildElementWithTagName(*this->elementStack.Last(), tagName) != nullptr;
		}

		inline bool MoreChildrenExistsAtCurrentLevel() const
		{
			return !this->elementStack.Last()->Children().IsEmpty();
		}

		inline void LeaveAttributes()
		{
			this->inAttributes = false;
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
		bool inAttributes;
		String currentAttributeName;

		//Operators
		inline void operator>>(int32& value)
		{
			String tmp;
			*this >> tmp;

			value = tmp.ToInt32();
		}

		inline void operator>>(uint64& value)
		{
			String tmp;
			*this >> tmp;

			value = tmp.ToUInt();
		}

		template <typename T>
		inline void operator>>(T& obj)
		{
			Archive(*this, obj);
		}

		//Methods
		const CommonFileFormats::XML::Element* FirstChildElementWithTagName(const CommonFileFormats::XML::Element& element, const String& tagName) const;

		//Inline
		inline void EnterElementOrAttribute(const String& name)
		{
			if(this->inAttributes)
				this->currentAttributeName = name;
			else
				this->EnterElement(name);
		}

		inline CommonFileFormats::XML::Element* FirstChildElementWithTagName(CommonFileFormats::XML::Element& element, const String& tagName)
		{
			return const_cast<CommonFileFormats::XML::Element *>(this->FirstChildElementWithTagName(
					(const CommonFileFormats::XML::Element &) element, tagName));
		}

		inline void LeaveElementOrAttribute()
		{
			if(this->inAttributes)
				this->elementStack.Last()->RemoveAttribute(this->currentAttributeName);
			else
				this->LeaveElement();
		}
	};
}
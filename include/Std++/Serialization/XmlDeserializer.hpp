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
#pragma once
//Local
#include <Std++/Tuple.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>
#include <Std++/CommonFileFormats/XML/TextNode.hpp>
#include "Std++/CommonFileFormats/XML/Document.hpp"
#include <Std++/Optional.hpp>
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

		XmlDeserializer& operator>>(const Binding<bool>& binding)
		{
			this->EnterElementOrAttribute(binding.name);
			String string = this->FetchStringValue();
			if(string == u8"true")
				binding.value = true;
			else if (string == u8"false")
				binding.value = false;
			else
				NOT_IMPLEMENTED_ERROR; //TODO: do this correctly
			this->LeaveElementOrAttribute();

			return *this;
		}

		XmlDeserializer& operator>>(const Binding<int32>& binding)
		{
			this->EnterElementOrAttribute(binding.name);
			binding.value = this->FetchStringValue().ToInt32();
			this->LeaveElementOrAttribute();

			return *this;
		}

        XmlDeserializer& operator>>(const Binding<uint32>& binding)
        {
            this->EnterElementOrAttribute(binding.name);
            binding.value = this->FetchStringValue().ToUInt();
            this->LeaveElementOrAttribute();

            return *this;
        }

		XmlDeserializer& operator>>(const Binding<uint64>& binding)
		{
			this->EnterElementOrAttribute(binding.name);
			binding.value = this->FetchStringValue().ToUInt();
			this->LeaveElementOrAttribute();

			return *this;
		}

		XmlDeserializer& operator>>(const Binding<String>& binding)
		{
			this->EnterElementOrAttribute(binding.name);
			binding.value = this->FetchStringValue();
			this->LeaveElementOrAttribute();

			return *this;
		}

		template<typename T>
		XmlDeserializer& operator>>(const Binding<Optional<T>>& binding)
		{
			bool contain = false;
			if(this->inAttributes)
			{
				if(this->elementStack.Last()->HasAttribute(binding.name))
					contain = true;
			}
			else if(this->FirstChildElementWithTagName(*this->elementStack.Last(), binding.name))
				contain = true;
			if(contain)
			{
				binding.value = T();
				return *this >> Binding(binding.name, *binding.value);
			}
			return *this;
		}

		template <typename T>
		XmlDeserializer& operator>>(const Binding<T>& binding)
		{
			this->EnterElementOrAttribute(binding.name);
			Archive(*this, binding.value);
			this->LeaveElementOrAttribute();

			return *this;
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

		inline bool HasAttribute(const String& name) const
		{
			return this->elementStack.Last()->HasAttribute(name);
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

		//Methods
		const CommonFileFormats::XML::Element* FirstChildElementWithTagName(const CommonFileFormats::XML::Element& element, const String& tagName) const;

		String FetchStringValue()
		{
			if(this->inAttributes)
				return this->elementStack.Last()->GetAttribute(this->currentAttributeName);
			else
			{
				const auto &children = this->elementStack.Last()->Children();
				ASSERT(children.GetNumberOfElements() == 1, u8"REPORT THIS PLEASE");
				ASSERT(children.Last()->GetType() == CommonFileFormats::XML::NodeType::TextNode,
					   u8"REPORT THIS PLEASE!");
				return dynamic_cast<CommonFileFormats::XML::TextNode *>(children.Last())->Text();
			}
		}

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
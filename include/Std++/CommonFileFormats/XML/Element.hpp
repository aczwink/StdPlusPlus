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
#include "Node.hpp"
#include "Std++/Containers/Map/Map.hpp"
#include "Std++/Containers/LinkedList/LinkedList.hpp"
#include "Std++/Containers/Strings/String.hpp"

namespace StdXX::CommonFileFormats::XML
{
	class Element : public Node
	{
	public:
		//Constructor
		inline Element(const String &refName)
		{
			this->name = refName;
		}

		//Destructor
		~Element();

		//Properties
		inline LinkedList<Node *> &Children()
		{
			return this->children;
		}

		inline const LinkedList<Node *> &Children() const
		{
			return this->children;
		}

		inline const String &Name() const
		{
			return this->name;
		}

		//Methods
		NodeType GetType() const override;
		void Write(Writer& writer) const override;

		//Inline
		inline void AppendChild(Node *pNode)
		{
			this->children.InsertTail(pNode);
		}

		inline const String &GetAttribute(const String &refKey) const
		{
			return this->attributes[refKey];
		}

		inline Map<String, String> &GetAttributes()
		{
			return this->attributes;
		}

		inline bool HasAttribute(const String &refKey) const
		{
			return this->attributes.Contains(refKey);
		}

		inline void RemoveAttribute(const String& key)
		{
			this->attributes.Remove(key);
		}

		inline void RemoveChild(const Node& child)
		{
			this->children.Remove(this->children.Find((Node*)&child));
		}

		inline void SetAttribute(const String &refKey, const String &refValue)
		{
			this->attributes[refKey] = refValue;
		}

		//For range-based loops
		inline LinkedListIterator<Node *> begin()
		{
			return this->children.begin();
		}

		inline LinkedListConstIterator<Node *> begin() const
		{
			return this->children.begin();
		}

		inline LinkedListIterator<Node *> end()
		{
			return this->children.end();
		}

		inline LinkedListConstIterator<Node *> end() const
		{
			return this->children.end();
		}

	private:
		//Members
		String name;
		LinkedList<Node *> children;
		Map<String, String> attributes;
	};
}
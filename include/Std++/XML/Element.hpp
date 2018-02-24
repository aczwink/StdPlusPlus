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
#pragma once
//Local
#include "Node.hpp"
#include "../Containers/Map/Map.hpp"
#include "../Containers/LinkedList/LinkedList.hpp"
#include "../Containers/Strings/OldString.hpp"
#include "../Containers/Strings/String.hpp"

namespace StdPlusPlus
{
    namespace XML
    {
        class Element : public Node
        {
        private:
            //Members
            String name;
            LinkedList<Node *> children;
            Map<String, String> attributes;

        public:
            //Constructor
            inline Element(const String &refName)
            {
                this->name = refName;
            }

            //Destructor
            ~Element();

            //Methods
            NodeType GetType() const;

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

            inline LinkedList<Node *> &GetChildren()
            {
                return this->children;
            }

            inline const LinkedList<Node *> &GetChildren() const
            {
                return this->children;
            }

            inline const String &GetName() const
            {
                return this->name;
            }

            inline bool HasAttribute(const String &refKey) const
            {
                return this->attributes.Contains(refKey);
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
        };
    }
}
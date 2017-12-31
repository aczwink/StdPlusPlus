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
#pragma once
//Local
#include "Node.hpp"
#include "../Containers/Map/Map.hpp"
#include "../Containers/LinkedList/LinkedList.hpp"
#include "ACStdLib/Containers/Strings/OldString.hpp"

namespace ACStdLib
{
    namespace XML
    {
        class Element : public Node
        {
        private:
            //Members
            OldString name;
            LinkedList<Node *> children;
            Map<OldString, OldString> attributes;

        public:
            //Constructor
            inline Element(const OldString &refName)
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

            inline const OldString &GetAttribute(const OldString &refKey) const
            {
                return this->attributes[refKey];
            }

            inline Map<OldString, OldString> &GetAttributes()
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

            inline const OldString &GetName() const
            {
                return this->name;
            }

            inline bool HasAttribute(const OldString &refKey) const
            {
                return this->attributes.Contains(refKey);
            }

            inline void SetAttribute(const OldString &refKey, const OldString &refValue)
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
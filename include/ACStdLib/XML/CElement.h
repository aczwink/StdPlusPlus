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
#include "ANode.h"
#include "../Containers/Map/Map.hpp"
#include "../Containers/LinkedList/LinkedList.hpp"
#include "../Containers/Strings/String.h"

namespace ACStdLib
{
    namespace XML
    {
        class CElement : public ANode
        {
        private:
            //Members
            String name;
            LinkedList<ANode *> children;
            Map<String, String> attributes;

        public:
            //Constructor
            inline CElement(const String &refName)
            {
                this->name = refName;
            }

            //Destructor
            ~CElement();

            //Methods
            ENodeType GetType() const;

            //Inline
            inline void AppendChild(ANode *pNode)
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

            inline LinkedList<ANode *> &GetChildren()
            {
                return this->children;
            }

            inline const LinkedList<ANode *> &GetChildren() const
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
            inline LinkedListIterator<ANode *> begin()
            {
                return this->children.begin();
            }

            inline LinkedListConstIterator<ANode *> begin() const
            {
                return this->children.begin();
            }

            inline LinkedListIterator<ANode *> end()
            {
                return this->children.end();
            }

            inline LinkedListConstIterator<ANode *> end() const
            {
                return this->children.end();
            }
        };
    }
}
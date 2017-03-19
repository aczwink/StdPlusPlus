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
#include "../Containers/CMap/CMap.h"
#include "../Containers/CLinkedList/CLinkedList.h"
#include "../Containers/Strings/CString.h"

namespace ACStdLib
{
    namespace XML
    {
        class CElement : public ANode
        {
        private:
            //Members
            CString name;
            CLinkedList<ANode *> children;
            CMap<CString, CString> attributes;

        public:
            //Constructor
            inline CElement(const CString &refName)
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

            inline const CString &GetAttribute(const CString &refKey) const
            {
                return this->attributes[refKey];
            }

            inline CMap<CString, CString> &GetAttributes()
            {
                return this->attributes;
            }

            inline CLinkedList<ANode *> &GetChildren()
            {
                return this->children;
            }

            inline const CLinkedList<ANode *> &GetChildren() const
            {
                return this->children;
            }

            inline const CString &GetName() const
            {
                return this->name;
            }

            inline bool HasAttribute(const CString &refKey) const
            {
                return this->attributes.Contains(refKey);
            }

            inline void SetAttribute(const CString &refKey, const CString &refValue)
            {
                this->attributes[refKey] = refValue;
            }

            //For range-based loops
            inline CLinkedListIterator<ANode *> begin()
            {
                return this->children.begin();
            }

            inline CLinkedListConstIterator<ANode *> begin() const
            {
                return this->children.begin();
            }

            inline CLinkedListIterator<ANode *> end()
            {
                return this->children.end();
            }

            inline CLinkedListConstIterator<ANode *> end() const
            {
                return this->children.end();
            }
        };
    }
}
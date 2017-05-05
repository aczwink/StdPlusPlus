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
#include "LinkedListNode.hpp"

namespace ACStdLib
{
    //Forward Declarations
    template<typename T>
    class LinkedList;

    template<typename T>
    class LinkedListConstIterator
    {
        friend class LinkedList<T>;
        typedef LinkedList<T> List;
        typedef LinkedListNode<T> Node;
    protected:
        //Variables
        List *pList;
        Node *pNode;
        uint32 index;
        //Constructor
        LinkedListConstIterator(List *pList, Node *pNode)
        {
            this->pList = pList;
            this->pNode = pNode;
            this->index = 0;
        }
    public:
        //Operators
        LinkedListConstIterator &operator++() //Prefix ++
        {
            this->pNode = this->pNode->pNext;
            this->index++;

            return *this;
        }

        LinkedListConstIterator &operator--() //Prefix --
        {
            this->pNode = this->pNode->pPrevious;
            this->index--;

            return *this;
        }

        const T &operator*() const
        {
            return this->pNode->data;
        }

        bool operator!=(const LinkedListConstIterator &refRight) const
        {
            return !(this->pList == refRight.pList && this->pNode == refRight.pNode);
        }

        //Functions
        uint32 GetIndex() const
        {
            return this->index;
        }

        bool IsValid() const
        {
            return this->pNode != NULL;
        }
    };
}
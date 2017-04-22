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
#include "CLinkedListConstIterator.h"

namespace ACStdLib
{
    template<typename T>
    class CLinkedListIterator : public CLinkedListConstIterator<T>
    {
        friend class CLinkedList<T>;
        typedef CLinkedList<T> List;
        typedef CLinkedListNode<T> Node;
    private:
        //Constructor
        CLinkedListIterator(List *pList, Node *pNode) : CLinkedListConstIterator<T>(pList, pNode)
        {
        }
    public:
        //Operators
        T &operator*()
        {
            return this->pNode->data;
        }
        //Methods
        void Delete()
        {
            --(*this);
            this->pList->Delete(this->index + 1);
        }
    };
}
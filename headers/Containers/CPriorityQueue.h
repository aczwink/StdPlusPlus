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
//Local
#include "AContainer.h"

namespace ACStdLib
{
    template<typename DataType>
    class CPriorityQueue : public AContainer
    {
        struct SNode
        {
            uint32 priority;
            DataType data;
            SNode *pNext;
        };
    private:
        //Members
        SNode *pHead;

    public:
        //Constructor
        CPriorityQueue()
        {
            this->pHead = nullptr;
        }

        //Destructor
        ~CPriorityQueue()
        {
            this->Release();
        }

        //Methods
        const DataType &GetFirst() const
        {
            return this->pHead->data;
        }

        uint32 GetFirstPriority() const
        {
            return this->pHead->priority;
        }

        void Insert(uint32 priority, const DataType &refValue)
        {
            SNode *pNode, *pNode2;

            this->nElements++;

            if(!this->pHead)
            {
                this->pHead = (SNode *)MemAlloc(sizeof(*this->pHead));
                this->pHead->priority = priority;
                this->pHead->data = refValue;
                this->pHead->pNext = NULL;

                return;
            }
            if(priority <= this->pHead->priority)
            {
                pNode = (SNode *)MemAlloc(sizeof(*pNode));
                pNode->priority = priority;
                pNode->data = refValue;
                pNode->pNext = this->pHead;
                this->pHead = pNode;

                return;
            }

            pNode = this->pHead;
            while(pNode->pNext && pNode->pNext->priority < priority)
                pNode = pNode->pNext;

            pNode2 = (SNode *)MemAlloc(sizeof(*pNode2));
            pNode2->priority = priority;
            pNode2->data = refValue;
            pNode2->pNext = pNode->pNext;
            pNode->pNext = pNode2;
        }

        DataType PopFirst()
        {
            SNode *pNode;
            DataType tmp;

            ASSERT(this->nElements);

            tmp = this->pHead->data;
            pNode = this->pHead;
            this->pHead = this->pHead->pNext;
            this->nElements--;
            MemFree(pNode);

            return tmp;
        }

        void Release()
        {
            while(this->nElements)
                this->PopFirst();
        }
    };
}
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
#include "../../Debug.h"
#include "../AContainer.h"
#include "LinkedListConstIterator.hpp"
#include "LinkedListIterator.hpp"

namespace ACStdLib
{
    template<typename DataType>
    class LinkedList : public AContainer
    {
        typedef LinkedListConstIterator<DataType> ConstIterator;
        typedef LinkedListIterator<DataType> Iterator;
        typedef LinkedListNode<DataType> Node;
    private:
        //Variables
        Node *head;
        Node *tail;

        //Functions
        Node *FindNode(uint32 index) const
        {
            Node *pNode;

            if(index >= this->nElements)
                return NULL;

            //search forward
            if(this->nElements - index > index)
            {
                pNode = this->head;

                while(index--)
                {
                    pNode = pNode->pNext;
                }

                return pNode;
            }
            else
            {
                pNode = this->tail;
                index = this->nElements - index - 1;

                while(index--)
                {
                    pNode = pNode->pPrevious;
                }

                return pNode;
            }

            return NULL;
        }
    public:
        //Constructors
        LinkedList()
        {
            this->head = NULL;
            this->tail = NULL;
            this->nElements = 0;
        }

        LinkedList(const LinkedList<DataType> &refList) //copy ctor
        {
            this->head = NULL;
            this->tail = NULL;
            this->nElements = 0;
            *this = refList;
        }

        LinkedList(LinkedList<DataType> &&list) //move ctor
		{
			this->head = nullptr;
			this->tail = nullptr;
			*this = (LinkedList<DataType> &&)list;
		}

        //Destructor
        ~LinkedList()
        {
            this->Release();
        }

        //Operators
		DataType &operator[](uint32 index)
		{
			Node *pNode = this->FindNode(index);

			ASSERT(pNode);

			return pNode->data;
		}

		const DataType &operator[](uint32 index) const
		{
			Node *pNode = ((LinkedList<DataType> *)this)->FindNode(index);

			ASSERT(pNode);

			return pNode->data;
		}

        LinkedList &operator=(const LinkedList &refList) //copy assign
        {
			this->Release();

            for(const DataType &refValue : refList)
                this->InsertTail(refValue);

            return *this;
        }

		LinkedList &operator=(LinkedList &&list) //move assign
		{
			this->Release();

			this->head = list.head;
			this->tail = list.tail;
			this->nElements = list.nElements;
			list.head = nullptr;
			list.tail = nullptr;
			list.nElements = 0;

			return *this;
		}

        bool operator==(const LinkedList<DataType> &refOther) const
        {
            Node *pHead1, *pHead2;

            if(this->nElements != refOther.nElements)
                return false;

            pHead1 = this->head;
            pHead2 = refOther.head;

            for(uint32 i = 0; i < this->nElements; i++)
            {
                if(!(pHead1->data == pHead2->data))
                    return false;
                pHead1 = pHead1->pNext;
                pHead2 = pHead2->pNext;
            }

            return true;
        }

        //Methods
        int32 Find(const DataType &refItem) const
        {
            int32 counter;
            Node *pNode;

            counter = 0;
            pNode = this->head;
            while (pNode)
            {
                if(pNode->data == refItem)
                    return counter;
                pNode = pNode->pNext;
            }

            return -1;
        }

        inline const DataType &Get(uint32 index) const
        {
            Node *pNode;

            pNode = this->FindNode(index);
            ASSERT(pNode);

            return pNode->data;
        }

        void InsertAfter(uint32 index, const DataType &refData)
        {
            Node *pNew, *pPrevious;

            if(index + 1 >= this->GetNumberOfElements())
            {
                this->InsertTail(refData);
                return;
            }
            else if(this->GetNumberOfElements() - index > index)
            {
                //start search from front
                pPrevious = this->head;
                while(index--)
                    pPrevious = pPrevious->pNext;
            }
            else
            {
                //start search from tail
                pPrevious = this->tail;
                index = this->GetNumberOfElements() - index;
                while(--index)
                    pPrevious = pPrevious->pPrevious;
            }

            pNew = new Node(refData);
            pNew->pPrevious = pPrevious;
            pNew->pNext = pPrevious->pNext;
            pPrevious->pNext->pPrevious = pNew;
            pPrevious->pNext = pNew;
            this->nElements++;
        }

        void InsertFront(const DataType &refData)
        {
            if(!this->head)
            {
                this->head = new Node(refData);
                this->tail = this->head;
                this->nElements = 1;
                return;
            }

            this->head->pPrevious = new Node(refData, NULL, this->head);
            this->head = this->head->pPrevious;
            this->nElements++;
        }

        void InsertTail(const DataType &refData)
        {
            if(!this->head)
            {
                this->head = new Node(refData);
                this->tail = this->head;
                this->nElements = 1;
                return;
            }

            this->tail->pNext = new Node(refData, this->tail, NULL);
            this->tail = this->tail->pNext;
            this->nElements++;
        }

        DataType PopFront()
        {
            DataType data;
            Node *pNode;

            if(!this->nElements)
            {
                //throw exception
                ASSERT(0);
                return DataType();
            }

            pNode = this->head;
            data = pNode->data;
            if(this->head == this->tail)
            {
                this->head = NULL;
                this->tail = NULL;
            }
            else
            {
                this->head = pNode->pNext;
                this->head->pPrevious = nullptr;
            }

            delete pNode;
            this->nElements--;

            return data;
        }

        DataType PopTail()
        {
            DataType data;
            Node *pNode;

            if(!this->nElements)
            {
                //throw exception
                return DataType();
            }

            pNode = this->tail;
            data = pNode->data;
            if(this->tail->pPrevious)
            {
                this->tail = this->tail->pPrevious;
                this->tail->pNext = NULL;
            }
            else
            {
                this->head = NULL;
                this->tail = NULL;
            }

            delete pNode;
            this->nElements--;

            return data;
        }

        void Release()
        {
            while(this->nElements)
            {
                this->PopTail();
            }
        }

        void Remove(uint32 index)
        {
            Node *pNode;

            pNode = this->FindNode(index);
            if(pNode)
            {
                if(pNode == this->head && pNode == this->tail)
                {
                    this->head = NULL;
                    this->tail = NULL;
                }
                else if(pNode == this->head)
                {
                    this->head = this->head->pNext;
                }
                else if(pNode == this->tail)
                {
                    this->tail = this->tail->pPrevious;
                    this->tail->pNext = nullptr;
                }
                else
                {
                    pNode->pPrevious->pNext = pNode->pNext;
                    pNode->pNext->pPrevious = pNode->pPrevious;
                }

                this->nElements--;
                delete pNode;
            }

            //throw exception
        }


        //For range-based loops
        Iterator begin()
        {
            return Iterator(this, this->head);
        }

        ConstIterator begin() const
        {
            return ConstIterator((LinkedList *)this, this->head);
        }

        Iterator end()
        {
            return Iterator(this, NULL);
        }

        ConstIterator end() const
        {
            return ConstIterator((LinkedList *)this, NULL);
        }
    };
}
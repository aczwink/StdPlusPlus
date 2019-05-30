/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Debug.hpp>
#include <Std++/Utility.hpp>
#include "../ResizeableSequenceContainer.hpp"
#include "LinkedListConstIterator.hpp"
#include "LinkedListIterator.hpp"

namespace StdXX
{
    template<typename DataType>
    class LinkedList : public Container
    {
        typedef LinkedListConstIterator<DataType> ConstIterator;
        typedef LinkedListIterator<DataType> Iterator;
        typedef LinkedListNode<DataType> Node;

		friend class LinkedListIterator<DataType>;

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

			ASSERT(pNode, "Index out of bounds");

			return pNode->data;
		}

		const DataType &operator[](uint32 index) const
		{
			Node *pNode = ((LinkedList<DataType> *)this)->FindNode(index);

			ASSERT(pNode, "Index out of bounds");

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
                pHead1 = pHead1->next;
                pHead2 = pHead2->next;
            }

            return true;
        }

        //Methods
		bool Contains(const DataType& item) const
		{
			return this->Find(item) != -1;
		}

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
                pNode = pNode->next;
            }

            return -1;
        }

        inline const DataType &Get(uint32 index) const
        {
            Node *pNode;

            pNode = this->FindNode(index);
            ASSERT(pNode, "Index out of bounds");

            return pNode->data;
        }

        inline const DataType &GetFront() const
        {
            ASSERT(this->head, "Can't get front from empty list");

            return this->head->data;
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
                    pPrevious = pPrevious->next;
            }
            else
            {
                //start search from tail
                pPrevious = this->tail;
                index = this->GetNumberOfElements() - index;
                while(--index)
                    pPrevious = pPrevious->prev;
            }

            pNew = new Node(refData);
            pNew->prev = pPrevious;
            pNew->next = pPrevious->next;
            pPrevious->next->prev = pNew;
            pPrevious->next = pNew;
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

            this->head->prev = new Node(refData);
            this->head->prev->next = this->head;
            this->head = this->head->prev;
            this->nElements++;
        }

		inline DataType &Last()
		{
			ASSERT(this->tail, "Can't get last from empty list");

			return this->tail->data;
		}

		inline const DataType &Last() const
		{
			ASSERT(this->tail, "Can't get last from empty list");

			return this->tail->data;
		}

        DataType PopFront()
        {
			if(!this->nElements)
            {
                //throw exception
                NOT_IMPLEMENTED_ERROR;
                return DataType();
            }

            Node *pNode = this->head;
			DataType data = Move(pNode->data);
            if(this->head == this->tail)
            {
                this->head = NULL;
                this->tail = NULL;
            }
            else
            {
                this->head = pNode->next;
                this->head->prev = nullptr;
            }

            delete pNode;
            this->nElements--;

            return data;
        }

        DataType PopTail()
        {
            Node *pNode;

			ASSERT(this->nElements, u8"Can't pop from empty list.");

            pNode = this->tail;
			DataType data = Move(pNode->data); //the node will be destroyed anyways
            if(this->tail->prev)
            {
                this->tail = this->tail->prev;
                this->tail->next = NULL;
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

		void Remove(uint32 index)
		{
			this->RemoveNode(this->FindNode(index));
		}

        void Release()
        {
            while(this->nElements)
            {
                this->PopTail();
            }
        }

		//Inline
		template <typename... ArgTypes>
		inline void EmplaceTail(ArgTypes&&... args)
		{
			this->InsertNodeAtTail(new Node(Forward<ArgTypes>(args)...));
		}

		inline void InsertTail(const DataType &data)
		{
			this->InsertNodeAtTail(new Node(data));
		}

		inline void InsertTail(DataType&& data)
		{
			this->InsertNodeAtTail(new Node(Forward<DataType>(data)));
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

    private:
        //Variables
        Node *head;
        Node *tail;

        //Methods
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
                    pNode = pNode->next;
                }

                return pNode;
            }
            else
            {
                pNode = this->tail;
                index = this->nElements - index - 1;

                while(index--)
                {
                    pNode = pNode->prev;
                }

                return pNode;
            }

            return NULL;
        }

		void InsertNodeAtTail(Node *node)
		{
			if(!this->head)
			{
				this->head = node;
				node->next = node->prev = nullptr;
				this->tail = this->head;
				this->nElements = 1;
				return;
			}

			this->tail->next = node;
			node->prev = this->tail;
			node->next = nullptr;
			this->tail = this->tail->next;
			this->nElements++;
		}

		void RemoveNode(Node *node)
		{
			ASSERT(node, u8"Precondition for this method: node exists.");

			if(node == this->head && node == this->tail)
			{
				this->head = nullptr;
				this->tail = nullptr;
			}
			else if(node == this->head)
			{
				this->head = this->head->next;
				if(this->head)
					this->head->prev = nullptr;
			}
			else if(node == this->tail)
			{
				this->tail = this->tail->prev;
				if(this->tail)
					this->tail->next = nullptr;
			}
			else
			{
				//prev and next must exist, else node would be either head or tail
				node->prev->next = node->next;
				node->next->prev = node->prev;
			}

			this->nElements--;
			delete node;
		}
    };
}
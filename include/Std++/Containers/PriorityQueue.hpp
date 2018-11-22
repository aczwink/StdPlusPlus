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
#include "Std++/Debug.hpp"
#include "Container.hpp"

namespace StdXX
{
	/**
	 * A min-priority queue implementation e.g. the highest priority has the smallest priority value according to <.
	 * @tparam DataType
	 */
    template<typename DataType>
    class PriorityQueue : public Container
    {
        struct Node
        {
        	DataType data;
            Node *next;
        };

    public:
        //Constructor
        PriorityQueue()
        {
            this->head = nullptr;
        }

        //Destructor
        ~PriorityQueue()
        {
            this->Release();
        }

        //Methods
        const DataType &GetFirst() const
        {
			ASSERT(this->head, u8"Can't access empty queue.");

            return this->head->data;
        }

        void Insert(const DataType &value)
        {
            Node *pNode, *pNode2;

            this->nElements++;

            if(!this->head)
            {
                this->head = (Node *)MemAlloc(sizeof(*this->head));
                this->head->data = value;
                this->head->next = NULL;

                return;
            }
            if(value <= this->head->data)
            {
                pNode = (Node *)MemAlloc(sizeof(*pNode));
                pNode->data = value;
                pNode->next = this->head;
                this->head = pNode;

                return;
            }

            pNode = this->head;
            while(pNode->next && pNode->next->data < value)
                pNode = pNode->next;

            pNode2 = (Node *)MemAlloc(sizeof(*pNode2));
            pNode2->data = value;
            pNode2->next = pNode->next;
            pNode->next = pNode2;
        }

        DataType PopFirst()
        {
            Node *pNode;
            DataType tmp;

            ASSERT(this->nElements, "Can't pop from empty queue");

            tmp = this->head->data;
            pNode = this->head;
            this->head = this->head->next;
            this->nElements--;
            MemFree(pNode);

            return tmp;
        }

        void Release()
        {
            while(this->nElements)
                this->PopFirst();
        }

		void Remove(const DataType &value)
		{
			if (this->head == nullptr)
				return;

			Node *node = this->head;
			if(this->head->data == value)
			{
				this->head = this->head->next;
				this->nElements--;
				delete node;
				return;
			}

			while(node->next)
			{
				if(node->next->data == value)
				{
					node->next = node->next->next;
					this->nElements--;
					delete node->next;
					return;
				}
				node = node->next;
			}
		}

	private:
		//Members
		Node *head;
    };
}
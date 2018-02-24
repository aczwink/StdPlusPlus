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
#include "LinkedListConstIterator.hpp"

namespace StdPlusPlus
{
    template<typename T>
    class LinkedListIterator : public LinkedListConstIterator<T>
    {
        friend class LinkedList<T>;
        typedef LinkedList<T> List;
        typedef LinkedListNode<T> Node;

	public:
		//Operators
		T &operator*()
		{
			return this->node->data;
		}

		//Methods
		/**
		 * Removes the element that this iterator is currently at from the list.
		 * The index of the iterator remains the same.
		 * After the operation the iterator will be at the element that was after the element to be deleted.
		 *
		 * Deleting by this method is faster, because the node is already known and does not has to be searched by index.
		 */
		void Remove()
		{
			Node *toDelete = this->node;
			this->node = this->node->next;
			this->list->Remove(toDelete);
		}

    private:
        //Constructor
        LinkedListIterator(List *pList, Node *pNode) : LinkedListConstIterator<T>(pList, pNode)
        {
        }
    };
}
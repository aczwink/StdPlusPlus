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

namespace StdXX
{
    template<typename T>
    class LinkedListNode
    {
        typedef LinkedListNode<T> Node;
    public:
        //Variables
        T data;
        Node *prev;
        Node *next;

        //Constructors
        LinkedListNode(const T &data) : data(data)
        {
            this->prev = NULL;
            this->next = NULL;
        }

        LinkedListNode(T&& data) : data(Forward<T>(data))
		{
			this->prev = nullptr;
			this->next = nullptr;
		}

        LinkedListNode(const T &data, Node *pPrevious, Node *pNext) : data(data)
        {
            this->prev = pPrevious;
            this->next = pNext;
        }
    };
}
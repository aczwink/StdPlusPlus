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
    //Move declarations
    template<typename KeyType, typename ValueType>
    class MapNode;
    template<typename KeyType, typename ValueType>
    class Map;

    template<typename KeyType, typename ValueType>
    class ConstMapIterator
    {
        friend class Map < KeyType, ValueType > ;
        typedef MapNode<KeyType, ValueType> Node;
    public:
        //Operators
        inline bool operator==(const ConstMapIterator<KeyType, ValueType> &it) const
        {
            return (&this->map == &it.map) && (this->currentNode == it.currentNode);
        }

        inline bool operator!=(const ConstMapIterator<KeyType, ValueType> &refIt) const
        {
            return (&this->map != &refIt.map) || (this->currentNode != refIt.currentNode);
        }

        ConstMapIterator<KeyType, ValueType> &operator++() //Prefix ++
        {
            Node *pParent;

            if(this->currentNode)
            {
                if(this->currentNode->pRight)
                {
                    this->currentNode = (Node *)this->currentNode->pRight->GetFirst();
                }
                else
                {
                    while((pParent = (Node *)this->currentNode->pParent) && pParent->pRight == this->currentNode)
                        this->currentNode = pParent;
                    this->currentNode = pParent;
                }
            }

            return *this;
        }

		ConstMapIterator<KeyType, ValueType> &operator--() //Prefix --
		{
			if (this->currentNode)
			{
				if (this->currentNode->pLeft)
				{
					this->currentNode = (Node *)this->currentNode->pLeft->GetFirst();
				}
				else
				{
					Node *pParent;
					while ((pParent = (Node *)this->currentNode->pParent) && pParent->pLeft == this->currentNode)
						this->currentNode = pParent;
					this->currentNode = pParent;
				}
			}
			else
			{
				if(this->map.root)
					this->currentNode = this->map.root->GetLast();
			}

			return *this;
		}

        const KeyValuePair<KeyType, ValueType> &operator*() const
        {
            return this->currentNode->keyValuePair;
        }

	private:
		//Members
		const Map<KeyType, ValueType> &map;
		const Node *currentNode;

		//Constructor
		ConstMapIterator(const Map<KeyType, ValueType> &refMap, const Node *pNode) : map(refMap)
		{
			this->currentNode = pNode;
		}
    };



    template<typename KeyType, typename ValueType>
    class MapIterator
	{
		friend class Map < KeyType, ValueType > ;
		typedef MapNode<KeyType, ValueType> Node;
	private:
		//Members
		Map<KeyType, ValueType> &map;
		Node *currentNode;

		//Constructor
		MapIterator(Map<KeyType, ValueType> &refMap, Node *pNode) : map(refMap)
		{
			this->currentNode = pNode;
		}
	public:

		//Operators
		inline bool operator==(const MapIterator<KeyType, ValueType> &it) const
		{
			return (&this->map == &it.map) && (this->currentNode == it.currentNode);
		}

		inline bool operator!=(const MapIterator<KeyType, ValueType> &refIt) const
		{
			return (&this->map != &refIt.map) || (this->currentNode != refIt.currentNode);
		}

		MapIterator<KeyType, ValueType> &operator++() //Prefix ++
		{
			Node *pParent;

			if(this->currentNode)
			{
				if(this->currentNode->pRight)
				{
					this->currentNode = (Node *)this->currentNode->pRight->GetFirst();
				}
				else
				{
					while((pParent = (Node *)this->currentNode->pParent) && pParent->pRight == this->currentNode)
						this->currentNode = pParent;
					this->currentNode = pParent;
				}
			}

			return *this;
		}

		KeyValuePair<KeyType, ValueType> &operator*()
		{
			return this->currentNode->keyValuePair;
		}
	};
}
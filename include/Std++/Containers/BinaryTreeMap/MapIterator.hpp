/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
    //Forward declarations
    template<typename KeyType, typename ValueType>
    class MapNode;
    template<typename KeyType, typename ValueType>
    class BinaryTreeMap;

    template<typename KeyType, typename ValueType>
    class ConstMapIterator
    {
        friend class BinaryTreeMap < KeyType, ValueType > ;
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
                if(this->currentNode->right)
                {
                    this->currentNode = (Node *)this->currentNode->right->GetFirst();
                }
                else
                {
                    while((pParent = (Node *)this->currentNode->parent) && pParent->right == this->currentNode)
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
				if (this->currentNode->left)
				{
					this->currentNode = (Node *)this->currentNode->left->GetFirst();
				}
				else
				{
					Node *pParent;
					while ((pParent = (Node *)this->currentNode->parent) && pParent->left == this->currentNode)
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

		ConstMapIterator<KeyType, ValueType> operator--(int) //Postfix --
		{
			ConstMapIterator<KeyType, ValueType> tmp(*this);
			Node *pParent;

			if(this->pCurrentNode->pLeft)
			{
				this->pCurrentNode = this->pCurrentNode->pLeft->GetLast();
			}
			else
			{
				while((pParent = this->pCurrentNode->pParent) && pParent->pLeft == this->pCurrentNode)
					this->pCurrentNode = pParent;
				this->pCurrentNode = pParent;
			}

			return tmp;
		}

        const KeyValuePair<KeyType, ValueType> &operator*() const
        {
            return this->currentNode->keyValuePair;
        }

		ValueType& operator->()
		{
			return this->pCurrentNode->value;
		}

	private:
		//Members
		const BinaryTreeMap<KeyType, ValueType> &map;
		const Node *currentNode;

		//Constructor
		ConstMapIterator(const BinaryTreeMap<KeyType, ValueType> &refMap, const Node *pNode) : map(refMap)
		{
			this->currentNode = pNode;
		}
    };



    template<typename KeyType, typename ValueType>
    class MapIterator
	{
		friend class BinaryTreeMap < KeyType, ValueType > ;
		typedef MapNode<KeyType, ValueType> Node;
	private:
		//Members
		BinaryTreeMap<KeyType, ValueType> &map;
		Node *currentNode;

		//Constructor
		MapIterator(BinaryTreeMap<KeyType, ValueType> &refMap, Node *pNode) : map(refMap)
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
				if(this->currentNode->right)
				{
					this->currentNode = (Node *)this->currentNode->right->GetFirst();
				}
				else
				{
					while((pParent = (Node *)this->currentNode->parent) && pParent->right == this->currentNode)
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

		const KeyValuePair<KeyType, ValueType>* operator->() const
		{
			return &this->currentNode->keyValuePair;
		}
	};
}
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
//Local
#include <Std++/Containers/Enumeration/EnumeratorBuilder.hpp>
#include "../Container.hpp"
#include "MapNode.hpp"
#include "MapIterator.hpp"
#include "Std++/Debug.hpp"
#include "BinaryTreeMapEnumerator.hpp"

namespace StdXX
{
    //Implemented as Red-Black Tree
    template<typename KeyType, typename ValueType>
    class BinaryTreeMap : public Container
    {
        //typedef BinaryTreeMap<KeyType, ValueType> BinaryTreeMap;
        typedef MapNode<KeyType, ValueType> Node;

		friend class ConstMapIterator<KeyType, ValueType>;

    public:
		typedef MapIterator<KeyType, ValueType> Iterator;
		typedef ConstMapIterator<KeyType, ValueType> ConstIterator;

        //Constructors
        BinaryTreeMap()
        {
            this->root = nullptr;
        }

        BinaryTreeMap(const BinaryTreeMap &refOther) //copy ctor
        {
			this->root = nullptr;
            *this = refOther;
        }

        BinaryTreeMap(BinaryTreeMap &&refOther) //move ctor
        {
            this->root = nullptr;
            *this = (BinaryTreeMap &&)refOther; //forward
        }

        //Destructor
        ~BinaryTreeMap()
        {
            this->Release();
        }

        //Operators
        ValueType &operator[](const KeyType &refKey)
        {
            Node *pNode;

            pNode = this->FindNode(refKey);
            if(!pNode)
            {
                this->Insert(refKey, ValueType());
                pNode = this->FindNode(refKey);
            }

            return pNode->keyValuePair.value;
        }

        inline const ValueType &operator[](const KeyType& key) const
        {
            return this->Get(key);
        }

        BinaryTreeMap &operator=(const BinaryTreeMap &refMap) //copy assign
        {
            this->Release();

            for(const auto &refKV : refMap)
            {
                this->Insert(refKV.key, refKV.value);
            }

            return *this;
        }

        BinaryTreeMap &operator=(BinaryTreeMap&& other) //move assign
        {
            this->Release();

            this->root = other.root;
			other.root = nullptr;
			this->nElements = other.nElements;
			other.nElements = 0;

            return *this;
        }

	    //Logical operators
	    bool operator==(const BinaryTreeMap& other) const
	    {
		    if (this->nElements != other.nElements)
			    return false;

		    for(const KeyValuePair<KeyType, ValueType>& kv : *this)
		    {
		    	if(!other.Contains(kv.key))
		    		return false;
		    	if(kv.value != other[kv.key])
		    		return false;
		    }
		    return true;
	    }

        //Methods
		inline void Insert(const KeyType& key, const ValueType& value)
		{
			this->InsertNode(KeyValuePair<KeyType, ValueType>(key, value));
		}

		inline void Insert(const KeyType& key, ValueType&& value)
		{
			this->InsertNode(KeyValuePair<KeyType, ValueType>(key, Forward<ValueType>(value)));
		}

		inline void Insert(KeyType&& key, ValueType&& value)
		{
			this->InsertNode(KeyValuePair<KeyType, ValueType>(Forward<KeyType>(key), Forward<ValueType>(value)));
		}

        void Release()
        {
            if(this->root)
                delete this->root;
            this->nElements = 0;
            this->root = nullptr;
        }

        //Inline
        inline bool Contains(const KeyType &refKey) const
        {
            return this->FindNode(refKey) != nullptr;
        }

        inline _stdxx_::BinaryTreeMapEnumeratorBuilder<KeyType, ValueType> Entries() const
		{
        	return {*this};
		}

        inline Iterator Find(const KeyType &key)
        {
            return Iterator(*this, this->FindNode(key));
        }

        inline ConstIterator Find(const KeyType &refKey) const
        {
            return ConstIterator(*this, this->FindNode(refKey));
        }

		inline ValueType& Get(const KeyType& key)
		{
			Node* node = this->FindNode(key);
			ASSERT(node, u8"A non-existant key of map was accessed.");

			return node->keyValuePair.value;
		}

		inline const ValueType& Get(const KeyType& key) const
		{
			Node* node = this->FindNode(key);
			ASSERT(node, u8"A non-existant key of map was accessed.");

			return node->keyValuePair.value;
		}

		inline void Remove(const KeyType& key)
		{
			Node* node = this->FindNode(key);
			if(node == nullptr)
				return;
			this->RemoveNodeLinks(node);

			this->nElements--;
			while(this->root && this->root->parent)
				this->root = this->root->parent;
			node->left = nullptr;
			node->right = nullptr;
			delete node;
		}

        //For range-based loop
        Iterator begin()
        {
            if(!this->root)
                return Iterator(*this, nullptr);

            return Iterator(*this, (Node *)this->root->GetFirst());
        }

        ConstIterator begin() const
        {
            if(!this->root)
                return ConstIterator(*this, nullptr);

            return ConstIterator(*this, (Node *)this->root->GetFirst());
        }

		Iterator end()
		{
			return Iterator(*this, nullptr);
		}

        ConstIterator end() const
        {
            return ConstIterator(*this, nullptr);
        }

    private:
        //Members
        Node *root;

        //Methods
        Node *FindNode(const KeyType &refKey) const
        {
            Node *pNode;

            pNode = this->root;
            while(pNode)
            {
                if(refKey < pNode->keyValuePair.key)
                    pNode = (Node *)pNode->left;
                else if(refKey > pNode->keyValuePair.key)
                    pNode = (Node *)pNode->right;
                else
                    break;
            }

            return pNode;
        }

		void InsertNode(KeyValuePair<KeyType, ValueType>&& kv)
		{
			Node *pNode, *pInsertedNode;

			pInsertedNode = NULL;

			//insert root
			if (this->root == NULL)
			{
				this->root = new Node(Forward<KeyValuePair<KeyType, ValueType>>(kv), true, NULL);
				this->nElements = 1;
				return;
			}

			//binary-search tree insertion
			pNode = this->root;
			while (pNode)
			{
				if (pNode->keyValuePair.key < kv.key) //insert in right subtree
				{
					if (pNode->right) //continue in right subtree
					{
						pNode = (Node *)pNode->right;
					}
					else
					{
						//insert (key,value) as leaf
						pNode->right = new Node(Forward<KeyValuePair<KeyType, ValueType>>(kv), false, pNode);
						pInsertedNode = (Node *)pNode->right;
						break;
					}
				}
				else if (pNode->keyValuePair.key > kv.key) //insert in left subtree
				{
					if (pNode->left) //continue in left subtree
					{
						pNode = (Node *)pNode->left;
					}
					else
					{
						//insert (key,value) as leaf
						pNode->left = new Node(Forward<KeyValuePair<KeyType, ValueType>>(kv), false, pNode);
						pInsertedNode = (Node *)pNode->left;
						break;
					}
				}
				else //overwrite old node and return it, no need to rebalance
				{
					pNode->keyValuePair.value = Forward<ValueType>(kv.value);
					return;
				}
			}

			this->nElements++;

			//recolor the tree
			this->Recolorize(pInsertedNode);

			if (this->root->parent)
				this->root = (Node *)this->root->parent;
			this->root->isBlack = true;
		}

        /*
         * The implementation of this method is a modified version of a piece of code of the following work,
         * which was released under GNU Lesser General Public License
         * as published by the Free Software Foundation version 2.1:
         *
         * avltree - Implements an AVL tree with parent pointers.
         *
         * Copyright (C) 2010-2014 Franck Bui-Huu <fbuihuu@gmail.com>
         *
         * https://github.com/fbuihuu/libtree
         */
        void Recolorize(Node *pNode)
        {
            Node *pParent, *pGrandParent, *pUncle;

            while((pParent = (Node *)pNode->parent) && pParent->IsRed())
            {
                pGrandParent = (Node *)pParent->parent;

                if(pParent == pGrandParent->left)
                {
                    pUncle = (Node *)pGrandParent->right;

                    if(pUncle && pUncle->IsRed())
                    {
                        pParent->isBlack = true;
                        pUncle->isBlack = true;
                        pGrandParent->isBlack = false;
                        pNode = pGrandParent;
                    }
                    else
                    {
                        if(pNode == pParent->right)
                        {
                            pParent->RotateLeft();
                            pNode = pParent;
                            pParent = (Node *)pNode->parent;
                        }

                        pParent->isBlack = true;
                        pGrandParent->isBlack = false;
                        pGrandParent->RotateRight();
                    }
                }
                else
                {
                    pUncle = (Node *)pGrandParent->left;

                    if(pUncle && pUncle->IsRed())
                    {
                        pParent->isBlack = true;
                        pUncle->isBlack = true;
                        pGrandParent->isBlack = false;
                        pNode = pGrandParent;
                    }
                    else
                    {
                        if(pNode == pParent->left)
                        {
                            pParent->RotateRight();
                            pNode = pParent;
                            pParent = (Node *)pNode->parent;
                        }

                        pParent->isBlack = true;
                        pGrandParent->isBlack = false;
                        pGrandParent->RotateLeft();
                    }
                }
            }
        }

		/*
		 * The implementation of this method is a modified version of a piece of code of the following work,
		 * which was released under GNU Lesser General Public License
		 * as published by the Free Software Foundation version 2.1:
		 *
		 * avltree - Implements an AVL tree with parent pointers.
		 *
		 * Copyright (C) 2010-2014 Franck Bui-Huu <fbuihuu@gmail.com>
		 *
		 * https://github.com/fbuihuu/libtree
		 */
        void RemoveNodeLinks(Node* node)
		{
			Node* parent = node->parent;
			Node *left = node->left;
			Node *right = node->right;
			Node *next;
			bool isBlack;

			if (!left)
				next = right;
			else if (!right)
				next = left;
			else
				next = right->GetFirst();

			if (parent)
			{
				if(parent->left == node)
					parent->left = next;
				else
					parent->right = next;
			}
			else
				this->root = next;

			if (left && right) {
				isBlack = next->isBlack;
				next->isBlack = node->isBlack;

				next->left = left;
				left->parent = next;

				if (next != right) {
					parent = next->parent;
					next->parent = node->parent;

					node = next->right;
					parent->left = node;

					next->right = right;
					right->parent = next;
				} else {
					next->parent = parent;
					parent = next;
					node = next->right;
				}
			} else {
				isBlack = node->isBlack;
				node = next;
			}
			/*
			 * 'node' is now the sole successor's child and 'parent' its
			 * new parent (since the successor can have been moved).
			 */
			if (node)
				node->parent = parent;

			/*
			 * The 'easy' cases.
			 */
			if (!isBlack)
				return;
			if (node && node->IsRed()) {
				node->isBlack = true;
				return;
			}

			do {
				if (node == this->root)
					break;

				if (node == parent->left) {
					Node *sibling = parent->right;

					if (sibling->IsRed()) {
						sibling->isBlack = true;
						parent->isBlack = false;
						parent->RotateLeft();
						sibling = parent->right;
					}
					if ((!sibling->left || sibling->left->isBlack) &&
						(!sibling->right || sibling->right->isBlack)) {
						sibling->isBlack = false;
						node = parent;
						parent = parent->parent;
						continue;
					}
					if (!sibling->right || sibling->right->isBlack) {
						sibling->left->isBlack = true;
						sibling->isBlack = false;
						sibling->RotateRight();
						sibling = parent->right;
					}
					sibling->isBlack = parent->isBlack;
					parent->isBlack = true;
					sibling->right->isBlack = true;
					parent->RotateLeft();
					node = this->root;
					break;
				} else {
					Node *sibling = parent->left;

					if (sibling->IsRed()) {
						sibling->isBlack = true;
						parent->isBlack = false;
						parent->RotateRight();
						sibling = parent->left;
					}
					if ((!sibling->left || sibling->left->isBlack) &&
						(!sibling->right || sibling->right->isBlack)) {
						sibling->isBlack = false;
						node = parent;
						parent = parent->parent;
						continue;
					}
					if (!sibling->left || sibling->left->isBlack) {
						sibling->right->isBlack = true;
						sibling->isBlack = false;
						sibling->RotateLeft();
						sibling = parent->left;
					}
					sibling->isBlack = parent->isBlack;
					parent->isBlack = true;
					sibling->left->isBlack = true;
					parent->RotateRight();
					node = this->root;
					break;
				}
			} while (node->isBlack);

			if (node)
				node->isBlack = true;
		}
    };
}
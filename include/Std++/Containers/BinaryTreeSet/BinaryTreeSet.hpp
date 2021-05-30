/*
 * Copyright (c) 2017-2019,2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Utility.hpp>
#include "../Container.hpp"
#include "BinaryTreeSetNode.hpp"
#include "FiniteSetIterator.hpp"
//Definitions
#define BALANCE(pNode) ((Node *)pNode)->balance

namespace StdXX
{
    //Implementation as AVL tree
    template<typename DataType>
    class BinaryTreeSet : public Container
    {
        typedef BinaryTreeSet<DataType> Set;
        typedef BinaryTreeSetNode<DataType> Node;
        typedef CConstFiniteSetIterator<DataType> ConstIterator;
    public:
        //Constructors
        BinaryTreeSet()
        {
            this->pRoot = nullptr;
        }

        BinaryTreeSet(const Set &refSet)
        {
            this->pRoot = nullptr;

            *this = refSet;
        }

        BinaryTreeSet(Set&& set)
        {
            this->pRoot = nullptr;

            *this = Forward<Set>(set);
        }

        //Destructor
        ~BinaryTreeSet()
        {
            this->Release();
        }

        //Operators
        Set &operator=(const Set &refSet)
        {
            this->Release();

            for(const DataType &refValue : refSet)
            {
                this->Insert(refValue);
            }

            return *this;
        }

        Set &operator=(Set&& other)
        {
            this->Release();

            this->pRoot = other.pRoot;
			other.pRoot = nullptr;

			this->nElements = other.nElements;
			other.nElements = 0;

            return *this;
        }

        inline bool operator==(const BinaryTreeSet<DataType> &refRight) const
        {
            return this->GetNumberOfElements() == refRight.GetNumberOfElements() && this->IsSubsetOf(refRight);
        }

        //Methods
        bool Contains(const DataType &refElement) const
        {
            return this->FindNode(refElement) != nullptr;
        }

        void Insert(const DataType& value)
		{
        	this->InsertNode(value);
		}

		void Insert(DataType&& value)
		{
        	this->InsertNode(Move(value));
		}

        bool IsSubsetOf(const Set &refSet) const
        {
            for(const DataType &refData : *this)
            {
                if(!refSet.Contains(refData))
                    return false;
            }

            return true;
        }

        void Release()
        {
            if(this->pRoot)
                delete this->pRoot;
            this->nElements = 0;
            this->pRoot = nullptr;
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
        void Remove(const DataType &value)
        {
            Node *pNode, *pNodeToDelete;

            pNode = this->pRoot;
            while(pNode)
            {
                if(value < pNode->value)
                    pNode = pNode->left;
                else if(value > pNode->value)
                    pNode = pNode->right;
                else
                    break;
            }
            if(pNode != NULL)
            {
                Node *pParent, *pLeft, *pRight, *pNext;
                bool isLeftChild;

                pNodeToDelete = pNode;
                pParent = pNode->parent;
                pLeft = pNode->left;
                pRight = pNode->right;

                if(!pLeft)
                    pNext = pRight;
                else if(!pRight)
                    pNext = pLeft;
                else
                    pNext = pRight->GetFirst();

                if(pParent)
                {
                    isLeftChild = pParent->left == pNode;
                    if(isLeftChild)
                        pParent->left = pNext;
                    else
                        pParent->right = pNext;
                }
                else
                {
                    this->pRoot = pNext;
                }

                if(pLeft && pRight) //Node has 2 subtrees
                {
                    pNext->balance = pNode->balance;
                    pNext->left = pLeft;
                    pLeft->parent = pNext;

                    if(pNext == pRight)
                    {
                        pNext->parent = pParent;
                        pParent = pNext;
                        pNode = pParent->right;
                        isLeftChild = false;
                    }
                    else
                    {
                        pParent = pNext->parent;
                        pNext->parent = pNode->parent;
                        pNode = pNext->right;
                        pParent->left = pNode;
                        isLeftChild = true;
                        pNext->right = pRight;
                        pRight->parent = pNext;
                    }
                }
                else
                {
                    pNode = pNext;
                }

                if(pNode)
                    pNode->parent = pParent;

                while(pParent)
                {
                    pNode = pParent;
                    pParent = pParent->parent;

                    if(isLeftChild)
                    {
                        isLeftChild = pParent && pParent->left == pNode;

                        pNode->balance++;
                        if(pNode->balance == 0)
                            continue;
                        if(pNode->balance == 1)
                        {
                            if(this->pRoot->parent)
                                this->pRoot = this->pRoot->parent;
                            goto end;
                        }
                        pRight = pNode->right;

                        if(pRight->balance == 0)
                        {
                            pNode->balance = 1;
                            pRight->balance = -1;
                            pNode->RotateLeft();
                            if(this->pRoot->parent)
                                this->pRoot = this->pRoot->parent;
                            goto end;
                        }
                        else if(pRight->balance == 1)
                        {
                            pNode->balance = 0;
                            pRight->balance = 0;
                        }
                        else if(pRight->balance == -1)
                        {
                            if(pRight->left->balance == 1)
                            {
                                pNode->balance = -1;
                                pRight->balance = 0;
                            }
                            else if(pRight->left->balance == 0)
                            {
                                pNode->balance = 0;
                                pRight->balance = 0;
                            }
                            else if(pRight->left->balance == -1)
                            {
                                pNode->balance = 0;
                                pRight->balance = 1;
                            }
                            pRight->left->balance = 0;

                            pRight->RotateRight();
                        }
                        pNode->RotateLeft();
                    }
                    else
                    {
                        isLeftChild = pParent && pParent->left == pNode;

                        pNode->balance--;
                        if(pNode->balance == 0)
                            continue;
                        if(pNode->balance == -1)
                        {
                            if(this->pRoot->parent)
                                this->pRoot = this->pRoot->parent;
                            goto end;
                        }
                        pLeft = pNode->left;

                        if(pLeft->balance == 0)
                        {
                            pNode->balance = -1;
                            pLeft->balance = 1;
                            pNode->RotateRight();
                            if(this->pRoot->parent)
                                this->pRoot = this->pRoot->parent;
                            goto end;
                        }
                        else if(pLeft->balance == -1)
                        {
                            pNode->balance = 0;
                            pLeft->balance = 0;
                        }
                        else if(pLeft->balance == 1)
                        {
                            if(pLeft->right->balance == 1)
                            {
                                pNode->balance = 0;
                                pLeft->balance = -1;
                            }
                            else if(pLeft->right->balance == 0)
                            {
                                pNode->balance = 0;
                                pLeft->balance = 0;
                            }
                            else if(pLeft->right->balance == -1)
                            {
                                pNode->balance = 1;
                                pLeft->balance = 0;
                            }
                            pLeft->right->balance = 0;

                            pLeft->RotateLeft();
                        }
                        pNode->RotateRight();
                    }
                }
                end:;
                this->nElements--;
                pNodeToDelete->left = NULL;
                pNodeToDelete->right = NULL;
                delete pNodeToDelete;
                if(this->pRoot && this->pRoot->parent)
                    this->pRoot = this->pRoot->parent;
            }
        }

        void Union(const Set &refSet)
        {
            for(const DataType &refData : refSet)
                this->Insert(refData);
        }

        //For range-based loop
        ConstIterator begin() const
        {
            if(!this->pRoot)
                return ConstIterator(*this, nullptr);

            return ConstIterator(*this, (Node *)this->pRoot->GetFirst());
        }

        ConstIterator end() const
        {
            return ConstIterator(*this, nullptr);
        }

    private:
    	//Members
    	Node *pRoot;

        //Methods
        Node *FindNode(const DataType &refValue) const
        {
			Node *pNode;

			pNode = this->pRoot;
			while (pNode)
			{
				if (refValue < pNode->value)
					pNode = (Node *)pNode->left;
				else if (refValue > pNode->value)
					pNode = (Node *)pNode->right;
				else
					break;
			}

			return pNode;
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
		template<typename T>
		void InsertNode(T&& value)
		{
			Node *pNode, *pInsertedNode, *pParent, *pUnbalanced;

			if(this->pRoot == NULL)
			{
				this->pRoot = new Node(Forward<T>(value), NULL);
				this->nElements = 1;
				return;
			}

			pNode = this->pRoot;
			pInsertedNode = NULL;
			pUnbalanced = pNode;

			while(pNode)
			{
				if(pNode->balance != 0)
					pUnbalanced = pNode;

				if(pNode->value < value) //insert in right subtree
				{
					if(pNode->right) //continue in right subtree
					{
						pNode = (Node *)pNode->right;
					}
					else
					{
						//insert (key,value) as leaf
						pNode->right = new Node(Forward<T>(value), pNode);
						pInsertedNode = (Node *)pNode->right;
						break;
					}
				}
				else if(pNode->value > value) //insert in left subtree
				{
					if(pNode->left) //continue in left subtree
					{
						pNode = (Node *)pNode->left;
					}
					else
					{
						//insert (key,value) as leaf
						pNode->left = new Node(Forward<T>(value), pNode);
						pInsertedNode = (Node *)pNode->left;
						break;
					}
				}
				else //overwrite old node and return it, no need to rebalance
				{
					pNode->value = value;
					return;
				}
			}

			this->nElements++;

			//Update Balances
			pNode = pInsertedNode;
			pParent = (Node *)pNode->parent;
			while(true)
			{
				if(pParent->left == pNode)
					pParent->balance--;
				else
					pParent->balance++;

				if(pParent == pUnbalanced)
					break;

				pNode = pParent;
				pParent = (Node *)pParent->parent;
			}

			if(pUnbalanced->balance == -2)
			{
				if(BALANCE(pUnbalanced->left) == -1)
				{
					pUnbalanced->balance = 0;
					BALANCE(pUnbalanced->left) = 0;
				}
				else
				{
					if(BALANCE(pUnbalanced->left->right) == 1)
					{
						pUnbalanced->balance = 0;
						BALANCE(pUnbalanced->left) = -1;
					}
					else if(BALANCE(pUnbalanced->left->right) == 0)
					{
						pUnbalanced->balance = 0;
						BALANCE(pUnbalanced->left) = 0;
					}
					else if(BALANCE(pUnbalanced->left->right) == -1)
					{
						pUnbalanced->balance = 1;
						BALANCE(pUnbalanced->left) = 0;
					}
					BALANCE(pUnbalanced->left->right) = 0;

					pUnbalanced->left->RotateLeft();
				}
				pUnbalanced->RotateRight();
			}
			else if(pUnbalanced->balance == 2)
			{
				if(BALANCE(pUnbalanced->right) == 1)
				{
					pUnbalanced->balance = 0;
					BALANCE(pUnbalanced->right) = 0;
				}
				else
				{
					if(BALANCE(pUnbalanced->right->left) == 1)
					{
						pUnbalanced->balance = -1;
						BALANCE(pUnbalanced->right) = 0;
					}
					else if(BALANCE(pUnbalanced->right->left) == 0)
					{
						pUnbalanced->balance = 0;
						BALANCE(pUnbalanced->right) = 0;
					}
					else if(BALANCE(pUnbalanced->right->left) == -1)
					{
						pUnbalanced->balance = 0;
						BALANCE(pUnbalanced->right) = 1;
					}
					BALANCE(pUnbalanced->right->left) = 0;
					pUnbalanced->right->RotateRight();
				}
				pUnbalanced->RotateLeft();
			}

			if(this->pRoot->parent)
				this->pRoot = (Node *)this->pRoot->parent;
		}
    };
}

#undef BALANCE
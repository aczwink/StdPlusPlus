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

namespace ACStdLib
{
    //Forward declarations
    template<typename KeyType, typename ValueType>
    class MapNode;
    template<typename KeyType, typename ValueType>
    class Map;

    template<typename KeyType, typename ValueType>
    class CConstMapIterator
    {
        friend class Map < KeyType, ValueType > ;
        typedef MapNode<KeyType, ValueType> Node;
    private:
        //Members
        const Map<KeyType, ValueType> &refMap;
        const Node *pCurrentNode;

        //Constructor
        CConstMapIterator(const Map<KeyType, ValueType> &refMap, const Node *pNode) : refMap(refMap)
        {
            this->pCurrentNode = pNode;
        }
    public:

        //Operators
        inline bool operator!=(const CConstMapIterator<KeyType, ValueType> &refIt) const
        {
            return (&this->refMap != &refIt.refMap) || (this->pCurrentNode != refIt.pCurrentNode);
        }

        CConstMapIterator<KeyType, ValueType> &operator++() //Prefix ++
        {
            Node *pParent;

            if(this->pCurrentNode)
            {
                if(this->pCurrentNode->pRight)
                {
                    this->pCurrentNode = (Node *)this->pCurrentNode->pRight->GetFirst();
                }
                else
                {
                    while((pParent = (Node *)this->pCurrentNode->pParent) && pParent->pRight == this->pCurrentNode)
                        this->pCurrentNode = pParent;
                    this->pCurrentNode = pParent;
                }
            }

            return *this;
        }

        const CKeyValuePair<KeyType, ValueType> &operator*() const
        {
            return this->pCurrentNode->keyValuePair;
        }
    };
}
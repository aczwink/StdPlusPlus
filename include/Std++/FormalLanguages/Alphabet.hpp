/*
 * Copyright (c) 2023 Amir Czwink (amir130@hotmail.de)
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

namespace StdXX::FormalLanguages
{
    class Alphabet
    {
    public:
        //Properties
        inline uint32 EpsilonValue() const
        {
            return Unsigned<uint32>::Max() - 1;
        }

        inline uint32 FirstSymbol() const
        {
            return 0;
        }

        inline uint32 InvalidValue() const
        {
            return Unsigned<uint32>::Max();
        }

        inline uint32 LastSymbol() const
        {
            if(this->characterClasses.IsEmpty())
                return 0;
            return this->characterClasses.GetNumberOfElements() - 1;
        }

        //Methods
        inline void Add(const CharacterClass& characterClassToInsert)
        {
            CharacterClass ccCopy = characterClassToInsert;
            for(auto& cc : this->characterClasses)
            {
                if(cc == ccCopy)
                    return;

                auto intersection = cc.Intersect(ccCopy);
                if(intersection.IsEmpty())
                    continue;

                if(intersection == cc)
                {
                    ccCopy -= intersection;
                }
                else if(intersection == ccCopy)
                {
                    cc -= intersection;
                }
                else
                {
                    NOT_IMPLEMENTED_ERROR; //TODO: implement me
                }
            }
            this->characterClasses.Push(ccCopy);
        }

        inline uint32 Map(uint32 codePoint) const
        {
            for(uint32 i = 0; i < this->characterClasses.GetNumberOfElements(); i++)
            {
                const auto& cc = this->characterClasses[i];
                if(cc.Contains(codePoint))
                    return i;
            }
            return this->InvalidValue();
        }

        inline DynamicArray<uint32> MapAllPossible(const CharacterClass& characterClass) const
        {
            DynamicArray<uint32> symbols;

            for(uint32 i = 0; i < this->characterClasses.GetNumberOfElements(); i++)
            {
                const auto& cc = this->characterClasses[i];
                if(cc.Intersects(characterClass))
                    symbols.Push(i);
            }

            return symbols;
        }

    private:
        //State
        DynamicArray<FormalLanguages::CharacterClass> characterClasses;
    };
}
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
//Local
#include "ACStdLib/Containers/Array/DynamicArray.hpp"
#include "../Containers/FiniteSet/FiniteSet.hpp"
#include "../Containers/Map/Map.hpp"
#include "ACStdLib/Filesystem/Path.hpp"
#include "../Debug.h"

namespace ACStdLib
{
    class CDFAState
    {
    private:
        //Members
        bool isAccepting;
        Map<uint32, CDFAState *> transitions;
    public:
        //Constructor
        inline CDFAState()
        {
            this->isAccepting = false;
        }

        //Inline
        inline void AddTransition(uint32 inputSymbol, CDFAState *pStateTo)
        {
            ASSERT(!this->transitions.Contains(inputSymbol));

            this->transitions[inputSymbol] = pStateTo;
        }

        inline void GetTransitionsTo(CDFAState *pStateTo, DynamicArray<uint32> &refOutput)
        {
            for(const auto &refKV : this->transitions)
            {
                if(refKV.value == pStateTo)
                    refOutput.Push(refKV.key);
            }
        }

        inline bool HasTransitionTo(CDFAState *pStateTo)
        {
            for(const auto &refKV : this->transitions)
            {
                if(refKV.value == pStateTo)
                    return true;
            }

            return false;
        }

        inline bool IsAccepting() const
        {
            return this->isAccepting;
        }

        inline void SetAccepting(bool value)
        {
            this->isAccepting = value;
        }
    };

    class ACSTDLIB_API CDFA
    {
    private:
        //Members
        FiniteSet<CDFAState *> states;
    public:
        //Destructor
        inline ~CDFA()
        {
            for(CDFAState * const& prefState : states)
                delete prefState;
        }

        //Methods
        void Reduce();

        //Inline
        inline void AddState(CDFAState *pState)
        {
            this->states.Insert(pState);
        }

#ifdef _DEBUG
        //For debugging
		void SaveGraph(const Path &refPath);
#endif
    };
}
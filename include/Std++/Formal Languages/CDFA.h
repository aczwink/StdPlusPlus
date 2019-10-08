/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
#include "../Containers/Array/DynamicArray.hpp"
#include "Std++/Containers/BinaryTreeSet/BinaryTreeSet.hpp"
#include "../Containers/Map/Map.hpp"
#include "../Filesystem/Path.hpp"
#include "Std++/Debug.hpp"

namespace StdXX
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
            ASSERT(!this->transitions.Contains(inputSymbol), "If you see this, report to Std++");

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

    class STDPLUSPLUS_API CDFA
    {
    private:
        //Members
        BinaryTreeSet<CDFAState *> states;
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

#ifdef XPC_BUILDTYPE_DEBUG
        //For debugging
		void SaveGraph(const Path &refPath);
#endif
    };
}
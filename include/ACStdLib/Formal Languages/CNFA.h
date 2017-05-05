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
#include "../Definitions.h"
#include "../Containers/CArray/CArray.h"
#include "../Containers/CFiniteSet/CFiniteSet.h"
#include "../Containers/CMap/CMap.h"
#include "ACStdLib/Containers/Strings/UTF-8/UTF8String.hpp"
#include "ACStdLib/Filesystem/Path.hpp"
#include "CCharSet.h"
#include "CDFA.h"
//Definitions
#define EMPTYWORD_EPSILON UINT32_MAX

namespace ACStdLib
{
    class CNFAState
    {
    private:
        //Members
        bool isAccepting;
        CMap<uint32, CFiniteSet<CNFAState *>> transitions;
    public:
        //Constructor
        inline CNFAState()
        {
            this->isAccepting = false;
        }

        //Methods
        void GetEpsilonClosure(CFiniteSet<CNFAState *> &refEpsilonClosure) const;

        //Inline
        inline void AddTransition(uint32 input, CNFAState *pStateTo)
        {
            this->transitions[input].Insert(pStateTo);
        }

        inline const CMap<uint32, CFiniteSet<CNFAState *>> &GetTransitions() const
        {
            return this->transitions;
        }

        inline const CFiniteSet<CNFAState *> &GetTransitions(uint32 input) const
        {
            return this->transitions[input];
        }

        inline void GetTransitionsTo(CNFAState *pStateTo, CArray<uint32> &refInputs) const
        {
            for(const auto &refKV : this->transitions)
            {
                for(const CNFAState *const& prefState : refKV.value)
                {
                    if(prefState == pStateTo)
                        refInputs.Push(refKV.key);
                }
            }
        }

        inline bool HasTransition(uint32 input) const
        {
            return this->transitions.Contains(input);
        }

        inline bool HasTransitionTo(CNFAState *pStateTo) const
        {
            for(const auto &refKV : this->transitions)
            {
                for(const CNFAState *const& prefState : refKV.value)
                {
                    if(prefState == pStateTo)
                        return true;
                }
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

    class ACSTDLIB_API CNFA
    {
    private:
        //Members
        uint32 nInputSymbols;
        LinkedList<CNFAState *> states;

        //Methods
        void GetEpsilonClosure(const CFiniteSet<CNFAState *> &refInput, CFiniteSet<CNFAState *> &refOutput) const;
        void GetTransitions(uint32 inputSymbol, const CFiniteSet<CNFAState *> &refInput, CFiniteSet<CNFAState *> &refOutput) const;
    public:
        //Constructor
        inline CNFA(uint32 nInputSymbols)
        {
            this->nInputSymbols = nInputSymbols;
        }

        //Destructor
        inline ~CNFA()
        {
            for(const CNFAState *prefState : states)
                delete prefState;
        }

        //Methods
        CDFA *ConvertToDFA() const;
        CNFA *Copy() const;

        //Inline
        inline void AddState(CNFAState *pState)
        {
            this->states.InsertTail(pState);
        }

        inline uint32 GetNumberOfStates() const
        {
            return this->states.GetNumberOfElements();
        }

        inline CNFAState *GetState(uint32 index)
        {
            return this->states[index];
        }

        inline const LinkedList<CNFAState *> GetStates() const
        {
            return this->states;
        }

        inline void InsertState(int32 idx, CNFAState *pState)
        {
            if(idx == -1)
                this->states.InsertFront(pState);
            else
                this->states.InsertAfter(idx, pState);
        }

        inline void RemoveState(CNFAState *pState)
        {
            bool found;
            uint32 idx;

            found = false;
            idx = 0;
            for(const CNFAState *const& refpState : this->states)
            {
                if(pState == refpState)
                {
                    found = true;
                    break;
                }
                idx++;
            }

            if(found)
                this->states.Remove(idx);
        }

        inline void RemoveStates()
        {
            this->states.Release();
        }

#ifdef _DEBUG
        //For debugging
		void SaveGraph(const Path &refPath);
#endif
    };
}
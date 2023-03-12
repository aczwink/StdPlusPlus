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
//Local
#include <Std++/Containers/BinaryTreeMap/BinaryTreeMap.hpp>
#include <Std++/Containers/BinaryTreeSet/BinaryTreeSet.hpp>

namespace StdXX::FormalLanguages
{
    class DFAState
    {
    public:
        //Constructor
        inline DFAState()
        {
            this->isAccepting = false;
        }

        //Inline
        inline void AddTransition(uint32 inputSymbol, DFAState *pStateTo)
        {
            ASSERT(!this->transitions.Contains(inputSymbol), u8"If you see this, report to Std++");

            this->transitions[inputSymbol] = pStateTo;
        }

        inline const DFAState* GetTransition(uint32 inputSymbol) const
        {
            if(this->transitions.Contains(inputSymbol))
                return this->transitions.Get(inputSymbol);
            return nullptr;
        }

        inline bool IsAccepting() const
        {
            return this->isAccepting;
        }

        inline void SetAccepting(bool value)
        {
            this->isAccepting = value;
        }

        //Methods
        /**
         * Returns true if there is no way out of this state and its not accepting
         */
        bool IsDeadEnd() const;

    private:
        //State
        bool isAccepting;
        StdXX::BinaryTreeMap<uint32, DFAState *> transitions;
    };

    class DFA
    {
    public:
        //Destructor
        inline ~DFA()
        {
            for(DFAState* const& prefState : this->states)
                delete prefState;
        }

        //Inline
        inline void AddState(DFAState* state)
        {
            this->states.Insert(state);
        }

        inline DFAState* GetStartState() const
        {
            return this->startState;
        }

        inline void SetStartState(DFAState* state)
        {
            this->startState = state;
        }

        //Methods
        void Reduce();

    private:
        //Members
        StdXX::BinaryTreeSet<DFAState*> states;
        DFAState* startState;
    };
}

/*
 * class CDFAState
    {
    public:
        //Inline
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
};
*/
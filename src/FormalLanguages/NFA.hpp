/*
 * Copyright (c) 2019,2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/LinkedList/LinkedList.hpp>
#include <Std++/Containers/BinaryTreeMap/BinaryTreeMap.hpp>
#include "DFA.hpp"

namespace _stdxx_
{
    template<typename InputSymbolType>
    class NFAState
    {
    public:
        //Methods
        StdXX::BinaryTreeSet<NFAState<InputSymbolType> *> ComputeEpsilonClosure() const
        {
            NOT_IMPLEMENTED_ERROR; //TODO: implement me
            return {};
        }

        //Inline
        inline void AddTransition(InputSymbolType&& input, NFAState<InputSymbolType> *stateTo)
        {
            this->transitions[Move(input)].Insert(stateTo);
        }

    private:
        //Members
        StdXX::BinaryTreeMap<InputSymbolType, StdXX::BinaryTreeSet<NFAState<InputSymbolType> *>> transitions;
    };

    template<typename InputSymbolType>
    class NFA
    {
    public:
        //Destructor
        inline ~NFA()
        {
            for(const NFAState<InputSymbolType> *state : states)
                delete state;
        }

        //Methods
        DFA<InputSymbolType> *ToDFA()
        {
            DFA<InputSymbolType>* dfa = new DFA<InputSymbolType>();
            StdXX::DynamicArray<DFAState<InputSymbolType> *> unmarkedStates;
            StdXX::BinaryTreeMap<DFAState<InputSymbolType> *, StdXX::BinaryTreeSet<NFAState<InputSymbolType> *>> dfaToNFAStatesMap;

            //create dfa start state
            DFAState<InputSymbolType> *dfaStartState = new DFAState<InputSymbolType>();

            dfa->AddState(dfaStartState);
            unmarkedStates.Push(dfaStartState);

            //get all reachable start states from nfa
            NFAState<InputSymbolType>* nfaStartState = this->states[0]; //TODO: this is really bad... in theory all states could be start states
            StdXX::BinaryTreeSet<NFAState<InputSymbolType> *> dfaStartStateSet = nfaStartState->ComputeEpsilonClosure();
            dfaToNFAStatesMap[dfaStartState] = Move(dfaStartStateSet);

            //process states
            while(!unmarkedStates.IsEmpty())
            {
                DFAState<InputSymbolType>* processingState = unmarkedStates.Pop();

                NOT_IMPLEMENTED_ERROR; //TODO: continue here
            }

            return dfa;
        }

        //Inline
        inline void AddState(NFAState<InputSymbolType> *pState)
        {
            this->states.InsertTail(pState);
        }

    private:
        //Members
        StdXX::LinkedList<NFAState<InputSymbolType> *> states;
    };
}
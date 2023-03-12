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
#include <Std++/Containers/LinkedList/LinkedList.hpp>
#include <Std++/Containers/BinaryTreeMap/BinaryTreeMap.hpp>
#include "DFA.hpp"
#include "Alphabet.hpp"

namespace StdXX::FormalLanguages
{
    class NFAState
    {
    public:
        //Constructor
        inline NFAState()
        {
            this->isAccepting = false;
        }

        //Methods
        BinaryTreeSet<NFAState*> ComputeEpsilonClosure(uint32 epsilonValue) const
        {
            NFAState *pCurrentState;
            DynamicArray<NFAState *> unprocessedStates;
            BinaryTreeSet<NFAState*> epsilonClosure;

            epsilonClosure.Insert((NFAState *)this);
            unprocessedStates.Push((NFAState *)this);

            while(!unprocessedStates.IsEmpty())
            {
                pCurrentState = unprocessedStates.Pop();

                if(pCurrentState->HasTransition(epsilonValue))
                {
                    const BinaryTreeSet<NFAState *> &refTransitions = pCurrentState->GetTransitions(epsilonValue);
                    for(NFAState * const& prefState : refTransitions)
                    {
                        if(!epsilonClosure.Contains(prefState))
                        {
                            epsilonClosure.Insert(prefState);
                            unprocessedStates.Push(prefState);
                        }
                    }
                }
            }

            return epsilonClosure;
        }

        //Inline
        inline void AddTransition(uint32 input, NFAState* stateTo)
        {
            this->transitions[input].Insert(stateTo);
        }

        inline const BinaryTreeMap<uint32, BinaryTreeSet<NFAState *>> &GetTransitions() const
        {
            return this->transitions;
        }

        inline const BinaryTreeSet<NFAState *> &GetTransitions(uint32 input) const
        {
            return this->transitions[input];
        }

        inline bool HasTransition(uint32 input) const
        {
            return this->transitions.Contains(input);
        }

        inline bool IsAccepting() const
        {
            return this->isAccepting;
        }

        inline void SetAccepting(bool value)
        {
            this->isAccepting = value;
        }

    private:
        //State
        bool isAccepting;
        BinaryTreeMap<uint32, BinaryTreeSet<NFAState*>> transitions;
    };

    class NFA
    {
    public:
        //Destructor
        inline ~NFA()
        {
            for(const NFAState* state : this->states)
                delete state;
        }

        //Methods
        NFA *Copy() const
        {
            NFA *pNFA;
            NFAState *pNewState;
            BinaryTreeMap<const NFAState *, NFAState *> stateMap;

            pNFA = new NFA();

            //copy states
            for(const NFAState * const& prefState : this->states)
            {
                pNewState = new NFAState();

                pNFA->AddState(pNewState);
                stateMap[prefState] = pNewState;
            }

            //copy transitions
            for(const NFAState * const& prefState : this->states)
            {
                for(const auto &refKV : prefState->GetTransitions())
                {
                    for(const NFAState *const& prefEndState : refKV.value)
                    {
                        stateMap[prefState]->AddTransition(refKV.key, stateMap[prefEndState]);
                    }
                }
            }

            return pNFA;
        }

        DFA *ToDFA(const Alphabet& alphabet)
        {
            DFA* dfa = new DFA();
            DynamicArray<DFAState*> unmarkedStates;
            BinaryTreeMap<DFAState*, BinaryTreeSet<NFAState*>> dfaToNFAStatesMap;

            //create dfa start state
            DFAState *dfaStartState = new DFAState();

            dfa->AddState(dfaStartState);
            dfa->SetStartState(dfaStartState);
            unmarkedStates.Push(dfaStartState);

            //get all reachable start states from nfa
            NFAState* nfaStartState = this->states[0]; //TODO: this is really bad... in theory all states could be start states
            BinaryTreeSet<NFAState*> dfaStartStateSet = nfaStartState->ComputeEpsilonClosure(alphabet.EpsilonValue());
            dfaToNFAStatesMap[dfaStartState] = Move(dfaStartStateSet);

            //process states
            while(!unmarkedStates.IsEmpty())
            {
                DFAState* processingState = unmarkedStates.Pop();

                for(uint32 inputSymbol = alphabet.FirstSymbol(); inputSymbol <= alphabet.LastSymbol(); inputSymbol++)
                {
                    //get the states we can reach on this symbol
                    BinaryTreeSet<NFAState*> moveResult;
                    this->GetTransitions(inputSymbol, dfaToNFAStatesMap[processingState], moveResult);
                    if(moveResult.IsEmpty())
                        continue;

                    //get the states that can be reached by epsilon
                    BinaryTreeSet<NFAState*> epsilonClosureSet;
                    this->GetEpsilonClosure(moveResult, epsilonClosureSet, alphabet.EpsilonValue());

                    //check if that state exists already
                    bool found = false;
                    DFAState* pFoundState = nullptr;
                    for(const auto &refKV : dfaToNFAStatesMap)
                    {
                        if(refKV.value == epsilonClosureSet)
                        {
                            found = true;
                            pFoundState = refKV.key;
                            break;
                        }
                    }

                    if(found)
                    {
                        processingState->AddTransition(inputSymbol, pFoundState);
                    }
                    else
                    {
                        //create a new state
                        pFoundState = new DFAState();

                        dfa->AddState(pFoundState);
                        dfaToNFAStatesMap[pFoundState] = epsilonClosureSet;

                        processingState->AddTransition(inputSymbol, pFoundState);

                        unmarkedStates.Push(pFoundState);
                    }
                }
            }

            //set accepting states
            for(const auto &refKV : dfaToNFAStatesMap)
            {
                for(const NFAState* const& prefState : refKV.value)
                {
                    if(prefState->IsAccepting())
                        refKV.key->SetAccepting(true);
                }
            }

            return dfa;
        }

        //Inline
        inline void AddState(NFAState* pState)
        {
            this->states.InsertTail(pState);
        }

        inline uint32 GetNumberOfStates() const
        {
            return this->states.GetNumberOfElements();
        }

        inline NFAState* GetState(uint32 index)
        {
            return this->states[index];
        }

        inline const LinkedList<NFAState*>& GetStates() const
        {
            return this->states;
        }

        inline void InsertState(int32 idx, NFAState *pState)
        {
            if(idx == -1)
                this->states.InsertFront(pState);
            else
                this->states.InsertAfter(idx, pState);
        }

        inline void RemoveStates()
        {
            this->states.Release();
        }

    private:
        //Members
        LinkedList<NFAState*> states;

        //Methods
        void GetEpsilonClosure(const BinaryTreeSet<NFAState*> &refInput, BinaryTreeSet<NFAState*> &refOutput, uint32 epsilonValue) const
        {
            for(const NFAState *const& prefState : refInput)
                refOutput.Union(prefState->ComputeEpsilonClosure(epsilonValue));
        }

        void GetTransitions(uint32 inputSymbol, const BinaryTreeSet<NFAState*> &refInput, BinaryTreeSet<NFAState*> &refOutput) const
        {
            for(const NFAState* const& prefState : refInput)
            {
                if(prefState->HasTransition(inputSymbol))
                    refOutput.Union(prefState->GetTransitions(inputSymbol));
            }
        }
    };
}
/*
namespace StdXX
{
    class CNFAState
    {
        inline void GetTransitionsTo(CNFAState *pStateTo, DynamicArray<uint32> &refInputs) const
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
    };

    class STDPLUSPLUS_API CNFA
    {
        //Methods
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
    };
}*/
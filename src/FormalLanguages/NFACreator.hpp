/*
 * Copyright (c) 2019-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/FormalLanguages/Alphabet.hpp>
#include <Std++/FormalLanguages/NFA.hpp>
//Namespaces
using namespace StdXX::FormalLanguages;

namespace _stdxx_
{
    class NFACreator : private RegExNodeVisitor
    {
    public:
        //Constructor
        inline NFACreator(const Alphabet& alphabet) : alphabet(alphabet)
        {
        }

        //Methods
        StdXX::UniquePointer<NFA> Create(const RegExNode& node)
        {
            node.Visit(*this);

            if(this->nfaStack.IsEmpty())
            {
                //special case for empty regex
                auto nfa = new NFA();
                auto state = new NFAState;
                state->SetAccepting(true);
                nfa->AddState(state);
                return nfa;
            }

            auto nfa = Move(this->nfaStack.Pop());
            nfa->GetState(nfa->GetNumberOfStates() - 1)->SetAccepting(true);
            return nfa;
        }

    private:
        //State
        const Alphabet& alphabet;
        StdXX::DynamicArray<StdXX::UniquePointer<NFA>> nfaStack;

        //Methods
        void Concat()
        {
            auto pNFA2 = this->nfaStack.Pop();
            auto& pNFA1 = this->nfaStack.Last();

            //epsilon transition
            pNFA1->GetState(pNFA1->GetNumberOfStates() - 1)->AddTransition(this->alphabet.EpsilonValue(), pNFA2->GetState(0));

            //all states from B to A
            for(NFAState * const& refpState : pNFA2->GetStates())
            {
                pNFA1->AddState(refpState);
            }
            pNFA2->RemoveStates();
        }

        void Star()
        {
            NFA *pNFA;
            NFAState *pStateFrom, *pStateTo, *pLastState;

            pStateFrom = new NFAState();
            pStateTo = new NFAState();

            pNFA = this->nfaStack.Last().operator->();
            pLastState = pNFA->GetState(pNFA->GetNumberOfStates() - 1);

            //epsilon transitions
            pStateFrom->AddTransition(this->alphabet.EpsilonValue(), pStateTo);
            pStateFrom->AddTransition(this->alphabet.EpsilonValue(), pNFA->GetState(0));

            pLastState->AddTransition(this->alphabet.EpsilonValue(), pStateTo);
            pLastState->AddTransition(this->alphabet.EpsilonValue(), pNFA->GetState(0));

            pNFA->AddState(pStateTo);
            pNFA->InsertState(-1, pStateFrom);
        }

        void Visit(const InputNode& inputNode) override
        {
            NFA* nfa = new NFA;

            NFAState* stateFrom = new NFAState();
            NFAState* stateTo = new NFAState();

            auto symbols = this->alphabet.MapAllPossible(inputNode.characterClass);
            for(uint32 symbol : symbols)
                stateFrom->AddTransition(symbol, stateTo);

            nfa->AddState(stateFrom);
            nfa->AddState(stateTo);

            this->nfaStack.Push(nfa);
        }

        void Visit(const OrNode &orNode) override
        {
            orNode.lhs->Visit(*this);
            orNode.rhs->Visit(*this);

            this->Union();
        }

        void Visit(const SequenceNode& sequenceNode) override
        {
            for(const auto& node : sequenceNode.nodes)
                node->Visit(*this);

            for(uint32 i = 1; i < sequenceNode.nodes.GetNumberOfElements(); i++)
                this->Concat();
        }

        void Visit(const UnaryNode &unaryNode) override
        {
            unaryNode.subExpr->Visit(*this);
            switch(unaryNode.operation)
            {
                case UnaryOperation::Plus:
                {
                    //handle as A+ = AA*
                    this->nfaStack.Push(this->nfaStack.Last()->Copy());
                    this->Star(); //star the top nfa
                    this->Concat();
                }
                break;
                case UnaryOperation::QuestionMark:
                    this->ZeroOrOne();
                    break;
                case UnaryOperation::Star:
                    this->Star();
                    break;
            }
        }

        void Union()
        {
            NFA *pNFA1;
            NFAState *pStart, *pEnd;

            auto pNFA2 = this->nfaStack.Pop();
            pNFA1 = this->nfaStack.Last().operator->();

            pStart = new NFAState();
            pEnd = new NFAState();

            pStart->AddTransition(this->alphabet.EpsilonValue(), pNFA1->GetState(0));
            pStart->AddTransition(this->alphabet.EpsilonValue(), pNFA2->GetState(0));
            pNFA1->GetState(pNFA1->GetNumberOfStates() - 1)->AddTransition(this->alphabet.EpsilonValue(), pEnd);
            pNFA2->GetState(pNFA2->GetNumberOfStates() - 1)->AddTransition(this->alphabet.EpsilonValue(), pEnd);

            pNFA1->InsertState(-1, pStart);
            //all states from B to A
            for(NFAState * const& refpState : pNFA2->GetStates())
            {
                pNFA1->AddState(refpState);
            }
            pNFA2->RemoveStates();
            pNFA1->InsertState(pNFA1->GetNumberOfStates() - 1, pEnd);
        }

        void ZeroOrOne()
        {
            NFA *pNFA;
            NFAState *pState1, *pState2;

            pState1 = new NFAState();
            pState2 = new NFAState();

            pNFA = this->nfaStack.Last().operator->();

            pState1->AddTransition(this->alphabet.EpsilonValue(), pState2);
            pState1->AddTransition(this->alphabet.EpsilonValue(), pNFA->GetState(0));

            pNFA->GetState(pNFA->GetNumberOfStates() - 1)->AddTransition(this->alphabet.EpsilonValue(), pState2);

            pNFA->InsertState(pNFA->GetNumberOfStates() - 1, pState2);
            pNFA->InsertState(-1, pState1);
        }
    };
}
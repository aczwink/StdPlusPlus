//Class Header
#include "__CRegExBuilder.h"
//Global
#include <iostream>
//Local
#include "__CRegExLexer.h"
#include "__CRegExParser.h"
//Namespaces
using namespace SJCLib;
using namespace SJCLibIntern;

//Constructor
CRegExBuilder::CRegExBuilder()
{
	this->nextStateId = 0;
}

//Destructor
CRegExBuilder::~CRegExBuilder()
{
	this->Release();
}

//Public Functions
void CRegExBuilder::BuildTables(uint8 *pTranslationTable, uint32 *pTransitionsTable, uint32 *pTokenTypeTable)
{
	uint8 nAlphabetElements, id = 2;
	CFiniteSet<CState *> transitions;

	//renumerate states
	this->nextStateId = 0;
	foreach(it, this->dfaTable)
	{
		(*it)->SetStateId(this->nextStateId++);
	}

	//Non-Greedy and accepting states shouldn't have transitions back
	foreach(it, this->dfaTable)
	{
		if(!(*it)->IsGreedy() && (*it)->IsAcceptingState())
		{
			foreach(it2, this->dfaTable)
			{
				if(*it != *it2 && (*it)->HasTransition(*it2) && (*it)->GetStateId() > (*it2)->GetStateId())
				{
					(*it)->RemoveTransition(*it2);
				}
			}
		}
	}

	//Build Translation table
	MemZero(pTranslationTable, 256);
	
	pTranslationTable[0] = 1; //nullbyte is special

	nAlphabetElements = this->inputAlphabet.GetNoOfElements();
	
	foreach(it, this->inputAlphabet)
	{
		pTranslationTable[it.GetValue()] = id++;
	}

	//special element nullbyte
	nAlphabetElements++;
	//special element rest
	nAlphabetElements++;

	//Transitions
	repeat(this->GetNumberOfDFAStates() * nAlphabetElements, i)
	{
		pTransitionsTable[i] = this->nextStateId; //all go to invalid state
	}
	
	foreach(it, this->dfaTable)
	{
		//if accepting state, each input (except invalid) goes to final state
		if((*it)->IsAcceptingState())
		{
			for(uint32 i = 1; i < nAlphabetElements; i++)
			{
				pTransitionsTable[it.GetIndex() * nAlphabetElements + i] = CLEXER_STATE_FINISHED;
			}
		}

		//handle all other inputs
		foreach(it2, this->inputAlphabet)
		{
			if((*it)->HasTransition(it2.GetValue()))
			{
				(*it)->GetTransitions(it2.GetValue(), transitions);
				foreach(it3, transitions)
				{
					pTransitionsTable[it.GetIndex() * nAlphabetElements + pTranslationTable[it2.GetValue()]] = this->dfaTable.Find(it3.GetValue());
				}
			}
		}
	}

	//invalid state accepts on each input
	repeat(nAlphabetElements, i)
	{
		pTransitionsTable[this->nextStateId * nAlphabetElements + i] = CLEXER_STATE_FINISHED;
	}

	pTransitionsTable[1] = CLEXER_STATE_END; //if we are in start state and we see the nullbyte we go to endstate

	//Fill out token type table
	MemZero(pTokenTypeTable, this->GetNumberOfDFAStates() * sizeof(*pTokenTypeTable));
	foreach(it, this->dfaTable)
	{
		if((*it)->IsAcceptingState())
		{
			pTokenTypeTable[(*it)->GetStateId()] = (*it)->GetAcceptingValue();
		}
	}
}

void CRegExBuilder::Combine(CRegExBuilder &refOther)
{
	CState *pStart, *pEnd;
	
	this->inputAlphabet.Union(refOther.inputAlphabet);

	foreach(it, refOther.nfaTable)
	{
		(*it)->SetStateId(this->nextStateId++);
	}
	
	if(this->nfaTable.IsEmpty())
	{
		this->nfaTable = refOther.nfaTable;
		refOther.nfaTable.Release();
		return;
	}
	
	pStart = new CState(this->nextStateId++);
	pEnd = new CState(this->nextStateId++);
	
	pStart->AddTransition(0, this->nfaTable.Get(0));
	pStart->AddTransition(0, refOther.nfaTable.Get(0));
	this->nfaTable.Get(this->nfaTable.GetNoOfElements()-1)->AddTransition(0, pEnd);
	refOther.nfaTable.Get(refOther.nfaTable.GetNoOfElements()-1)->AddTransition(0, pEnd);
	
	refOther.nfaTable.InsertTail(pEnd);
	this->nfaTable.InsertFront(pStart);
	
	foreach(it, refOther.nfaTable)
	{
		this->nfaTable.InsertTail(*it);
	}
	
	//this->nfaTable.Get(this->nfaTable.GetNoOfElements()-1)->SetAccepting(true);
	
	refOther.nfaTable.Release();
}

void CRegExBuilder::CreateNFA(CString regEx, uint32 acceptValue, bool caseSensitive)
{
	CRegExLexer lexer;
	CRegExParser parser(caseSensitive);
	
	lexer.SetInput((const byte *)regEx.GetC_Str());
	parser.Parse(lexer);
	
	this->nfaTable = parser.operandStack.Pop();

	this->nfaTable.Get(this->nfaTable.GetNoOfElements()-1)->SetAccepting(true);
	this->nfaTable.Get(this->nfaTable.GetNoOfElements()-1)->SetAcceptingValue(acceptValue);

	this->inputAlphabet = parser.inputAlphabet;
}

void CRegExBuilder::ReduceDFA()
{
	CFiniteSet<CState *> deadEndSet;

	foreach(it, this->dfaTable)
	{
		if((*it)->IsDeadEnd())
		{
			deadEndSet.Insert(*it);
		}
	}

	if(deadEndSet.GetNoOfElements() == 0)
		return;

	foreach(it, deadEndSet)
	{
		foreach(it2, this->dfaTable)
		{
			(*it2)->RemoveTransition(it.GetValue());
		}
		
		delete it.GetValue();
		it.Delete();
	}
}

void CRegExBuilder::Release()
{
	CFiniteSet<CState *> states;

	foreach(it, this->nfaTable)
	{
		states.Insert(*it);
	}
	foreach(it, this->dfaTable)
	{
		states.Insert(*it);
	}

	foreach(it, states)
	{
		delete it.GetValue();
	}

	this->nfaTable.Release();
	this->dfaTable.Release();

	this->nextStateId = 0;

	this->inputAlphabet.Release();
}

//Public Debugging Functions
#ifdef _DEBUG
void CRegExBuilder::SaveGraph(SJCLib::CString filename, SJCLib::CLinkedList<CState *> &refAutomaton)
{
	CState *pStart, *pEnd;
	CString transition;
	CFiniteSet<CState *> states, currentStateSet;
	CFileOutputStream file(filename + ".dot");

	file << "digraph{\n";
	
	foreach(it, refAutomaton)
	{
		if((*it)->IsAcceptingState())
		{
			file << "\t" << (*it)->GetStateId() << "\t[shape=doublecircle];\n";
		}
	}

	file << "\n";

	foreach(it, refAutomaton)
	{
		pStart = *it;

		currentStateSet.Insert(pStart);
		
		foreach(it2, refAutomaton)
		{
			pEnd = *it2;

			transition = "";
			states.Release();

			pStart->GetTransitions(0, states);
			if(pStart->HasTransition((byte)0) && states.Contains(pEnd))
			{
				transition += "epsilon, ";
			}

			foreach(it3, this->inputAlphabet)
			{
				states.Release();
				
				pStart->GetTransitions(it3.GetValue(), states);
				if(pStart->HasTransition(it3.GetValue()) && states.Contains(pEnd))
				{
					transition += CString((uint32)it3.GetValue()) + ", ";
				}
			}

			if(!transition.IsEmpty())
			{
				file << "\t" << pStart->GetStateId() << " -> " << pEnd->GetStateId() << "\t[label=\"" << transition.SubString(0, transition.GetLength()-2) << "\"];\n";
			}
		}
	}

	file << "}";
}
#endif
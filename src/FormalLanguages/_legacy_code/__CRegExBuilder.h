#pragma once
//Local
#include "__CState.h"
#include "CLinkedList.h"
#include "CString.h"

namespace SJCLibIntern
{
	class SJCLIB_API CRegExBuilder
	{
	private:
		//Variables
		uint16 nextStateId;
		SJCLib::CFiniteSet<byte> inputAlphabet;
		SJCLib::CLinkedList<CState *> nfaTable;
		SJCLib::CLinkedList<CState *> dfaTable;
	public:
		//Constructor
		CRegExBuilder();
		//Destructor
		~CRegExBuilder();
		//Functions
		void BuildTables(uint8 *pTranslationTable, uint32 *pTransitionsTable, uint32 *pTokenTypeTable);
		void Combine(CRegExBuilder &refOther);
		void CreateNFA(SJCLib::CString regEx, uint32 acceptValue, bool caseSensitive = true);
		void ReduceDFA();
		void Release();
		//Inline
		inline uint8 GetNumberOfAlphabetElements() const
		{
			return this->inputAlphabet.GetNoOfElements() + 2; //+ \0 and invalid
		}

		inline uint32 GetNumberOfDFAStates() const
		{
			return this->dfaTable.GetNoOfElements() + 1; //invalid state!
		}
	};
}
#pragma once
//Local
#include "CFiniteSet.h"

namespace SJCLibIntern
{
	class CState
	{
	private:
		//Variables
		uint32 stateId;
		uint32 acceptingValue;
		bool isGreedy;
		SJCLib::CFiniteSet<CState *> nfaStates;
	public:
		//Constructors
		CState(uint16 stateId);
		CState(uint16 stateId, const SJCLib::CFiniteSet<CState *> &refNFAState);
		//Functions
		void AddTransition(byte input, CState *pDestinationState);
		bool HasTransition(const CState *pEnd);
		void RemoveTransition(CState *pDestinationState);
		void RemoveTransition(byte input);
		//Inline
		inline uint32 GetAcceptingValue() const
		{
			return this->acceptingValue;
		}

		inline const SJCLib::CFiniteSet<CState *> &GetNFAState()
		{
			return this->nfaStates;
		}

		inline uint32 GetStateId() const
		{
			return this->stateId;
		}

		inline bool IsGreedy() const
		{
			return this->isGreedy;
		}

		inline void SetAcceptingValue(uint32 value)
		{
			this->acceptingValue = value;
		}

		inline void SetGreedy(bool greedy)
		{
			this->isGreedy = greedy;
		}

		inline void SetStateId(uint32 id)
		{
			this->stateId = id;
		}
	};
}

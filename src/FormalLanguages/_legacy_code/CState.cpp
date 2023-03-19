//Class Header
#include "__CState.h"
//Namespaces
using namespace SJCLibIntern;
using namespace SJCLib;

//Constructors
CState::CState(uint16 stateId)
{
	this->stateId = stateId;
	this->isGreedy = true;
}

CState::CState(uint16 stateId, const CFiniteSet<CState *> &refNFAState)
{
	this->stateId = stateId;
	this->nfaStates = refNFAState;
	this->isGreedy = true;

	foreach(it, refNFAState)
	{
		if(it.GetValue()->IsAcceptingState())
		{
			this->isAcceptingState = true;
			this->acceptingValue = it.GetValue()->acceptingValue;
			break;
		}
	}
	//Those loops can't be combined
	foreach(it, refNFAState)
	{
		if(!it.GetValue()->IsGreedy())
		{
			this->isGreedy = false;
		}
	}
}

//Public Functions
bool CState::HasTransition(const CState *pEnd)
{
	foreach(it, this->transitions)
	{
		if(it.GetValue().Contains((CState *)pEnd))
			return true;
	}

	return false;
}

void CState::RemoveTransition(CState *pDestinationState)
{
	foreach(it, this->transitions)
	{
		foreach(it2, it.GetValue())
		{
			if(it2.GetValue() == pDestinationState)
			{
				it2.Delete();
			}
		}
	}
}

void CState::RemoveTransition(byte input)
{
	this->transitions.Delete(input);
}

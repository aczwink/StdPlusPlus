//Class Header
#include "__Math.h"
//Namespaces
using namespace SJCLib;

//Namespace Functions
uint64 SJCMMLib::Rescale(uint64 a, uint64 b, uint64 c)
{
	uint64 r;

	r = c / 2;

	return (a * b + r) / c;
}

uint64 SJCMMLib::Rescale(uint64 a, const CFraction32 &refB, const CFraction32 &refC)
{
	return Rescale(a, refB.GetNumerator() * refC.GetDenominator(), refC.GetNumerator() * refB.GetDenominator());
}
//Class Header
#include "CIOContext.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;

//Protected Functions
bool CIOContext::IsCorrectFormat(EContainerFormatId format, const EContainerFormatId *pAllowedFormats, uint32 nAllowedFormats)
{
	if(!pAllowedFormats)
		return true;
	repeat(nAllowedFormats, i)
	{
		if(format == pAllowedFormats[i])
		{
			return true;
		}
	}

	return false;
}

//Public methods
CWString CIOContext::GetTag(const CString &refTagName) const
{
	auto &refIt = this->tags.Find(refTagName);
	if(refIt.IsValid())
	{
		return refIt.GetValue();
	}

	return CString();
}
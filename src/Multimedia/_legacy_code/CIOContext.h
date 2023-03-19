#pragma once
//Local
#include "AStream.h"
#include "IFormat.h"

namespace SJCMMLib
{
	//Forward Declarations
	class COutputContext;

	class SJCMMLIB_API CIOContext
	{
	protected:
		//Variables
		SJCLib::CMap<SJCLib::CString, SJCLib::CWString> tags;
		//Functions
		bool IsCorrectFormat(EContainerFormatId format, const EContainerFormatId *pAllowedFormats, uint32 nAllowedFormats);
	public:
		//Methods
		SJCLib::CWString GetTag(const SJCLib::CString &refTagName) const;

		//Inline
		inline const IFormat *GetFormat() const
		{
			return this->pFormat;
		}

		inline const SJCLib::CMap<SJCLib::CString, SJCLib::CWString> &GetTags() const
		{
			return this->tags;
		}

		inline void SetTag(const SJCLib::CString &refTagName, const SJCLib::CWString &refValue)
		{
			this->tags[refTagName] = refValue;
		}
	};
}
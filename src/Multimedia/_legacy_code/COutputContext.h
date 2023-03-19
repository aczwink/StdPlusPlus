#pragma once
//SJCLib
#include <SJCLib.h>
//Local
#include "CIOContext.h"

namespace SJCMMLib
{
	//Forward Declarations
	class IFormat;
	class IMuxer;

	class SJCMMLIB_API COutputContext : public CIOContext
	{
	private:
		//Variables
		SJCLib::ASeekableOutputStream *pOutput;
		IMuxer *pMux;
	public:
		//Constructor
		COutputContext();
		//Destructor
		~COutputContext();
		//Functions
		IMuxer *GetMuxer();
		SJCLib::ASeekableOutputStream *GetOutputStream();
		void Init(SJCLib::ASeekableOutputStream *pOutput, EContainerFormatId outputFormat);
		void SetMuxer(IMuxer *pMux);
		void SetOutputStream(SJCLib::ASeekableOutputStream *pStream);
	};
}
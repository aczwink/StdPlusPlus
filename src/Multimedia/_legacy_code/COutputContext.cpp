//Class Header
#include "COutputContext.h"
//Local
#include "__globals.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;

//Constructor
COutputContext::COutputContext()
{
	this->pFormat = NULL;
	this->pMux = NULL;
	this->pOutput = NULL;
}

//Destructor
COutputContext::~COutputContext()
{
	repeat(this->streams.GetNoOfElements(), i)
	{
		this->streams[i]->Release();
		delete this->streams[i];
	}
	if(this->pMux)
	{
		delete this->pMux;
	}
}

//Public Functions
IMuxer *COutputContext::GetMuxer()
{
	return this->pMux;
}

ASeekableOutputStream *COutputContext::GetOutputStream()
{
	return this->pOutput;
}

void COutputContext::Init(ASeekableOutputStream *pOutput, EContainerFormatId outputFormat)
{
	EContainerFormatId currentFormat;
	
	repeat(g_formats.GetNoOfElements(), i)
	{
		currentFormat = g_formats[i]->GetId();
		if(IsCorrectFormat(outputFormat, &currentFormat, 1))
		{
			this->pFormat = g_formats[i];
			break;
		}
	}

	this->pOutput = pOutput;
	this->pMux = this->pFormat->RequestMuxerInstance();
}

void COutputContext::SetMuxer(IMuxer *pMux)
{
	this->pMux = pMux;
}

void COutputContext::SetOutputStream(ASeekableOutputStream *pStream)
{
	this->pOutput = pStream;
}
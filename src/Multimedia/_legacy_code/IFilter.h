#pragma once
//Local
#include "AFrame.h"

namespace ACStdLib
{
	namespace Multimedia
	{
		class ACSTDLIB_API IFilter
		{
		public:
			//Destructor
			virtual ~IFilter(){}

			//Abstract
			virtual AFrame *Apply(const AFrame &refFrame) const = NULL;
		};
	}
}
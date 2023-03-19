#pragma once
//Local
#include "../AImage.h"

namespace ACStdLib
{
	namespace Multimedia
	{
		class ACSTDLIB_API ARGBAImage : public AImage
		{
		public:
			//Constructor
			inline ARGBAImage(uint16 width, uint16 height) : AImage(width, height)
			{
			}

			//Abstract
			virtual void GetPixel(uint32 index, byte &refR, byte &refG, byte &refB, byte &refA) const = NULL;

			//Methods
			EColorSpace GetColorSpace() const;

			//Inline
			inline void GetPixel(uint16 x, uint16 y, byte &refR, byte &refG, byte &refB, byte &refA) const
			{
				this->GetPixel(y * this->GetWidth() + x, refR, refG, refB, refA);
			}
		};
	}
}
#pragma once
//Local
#include "../AImage.h"

namespace ACStdLib
{
	namespace Multimedia
	{
		class ACSTDLIB_API ARGBImage : public AImage
		{
		public:
			//Constructor
			inline ARGBImage(uint16 width, uint16 height) : AImage(width, height)
			{
			}

			//Abstract
			virtual void GetPixel(uint32 index, byte &refR, byte &refG, byte &refB) const = NULL;
			virtual void SetPixel(uint32 index, byte r, byte g, byte b) = NULL;
			
			//Methods
			EColorSpace GetColorSpace() const;

			//Inline
			inline void GetPixel(uint16 x, uint16 y, byte &refR, byte &refG, byte &refB) const
			{
				this->GetPixel(y * this->GetWidth() + x, refR, refG, refB);
			}

			inline void SetPixel(uint16 x, uint16 y, byte r, byte g, byte b)
			{
				this->SetPixel(y * this->GetWidth() + x, r, g, b);
			}
		};
	}
}
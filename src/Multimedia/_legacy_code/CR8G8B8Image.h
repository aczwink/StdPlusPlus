#pragma once
//Local
#include "../ARGBImage.h"

namespace ACStdLib
{
	namespace Multimedia
	{
		class ACSTDLIB_API CR8G8B8Image : public ARGBImage
		{
		private:
			//Members
			byte *pRedChannel;
			byte *pGreenChannel;
			byte *pBlueChannel;
		public:
			//Constructor
			CR8G8B8Image(uint16 width, uint16 height);

			//Destructor
			~CR8G8B8Image();

			//Derived methods
			using ARGBImage::SetPixel;

			//Methods
			void GetPixel(uint32 index, byte &refR, byte &refG, byte &refB) const;
			void SetPixel(uint32 index, byte r, byte g, byte b);

			//Inline
			inline byte *GetBlueChannel()
			{
				return this->pBlueChannel;
			};

			inline byte *GetGreenChannel()
			{
				return this->pGreenChannel;
			};

			inline byte *GetRedChannel()
			{
				return this->pRedChannel;
			};
		};
	}
}
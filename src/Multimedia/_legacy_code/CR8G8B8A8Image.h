#pragma once
//Local
#include "../ARGBAImage.h"

namespace ACStdLib
{
	namespace Multimedia
	{
		class ACSTDLIB_API CR8G8B8A8Image : public ARGBAImage
		{
		private:
			//Members
			byte *pRedChannel;
			byte *pGreenChannel;
			byte *pBlueChannel;
			byte *pAlphaChannel;
		public:
			//Constructor
			CR8G8B8A8Image(uint16 width, uint16 height);

			//Destructor
			~CR8G8B8A8Image();

			//Methods
			void GetPixel(uint32 index, byte &refR, byte &refG, byte &refB, byte &refA) const;

			//Inline
			inline byte *GetAlphaChannel()
			{
				return this->pAlphaChannel;
			}
			
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
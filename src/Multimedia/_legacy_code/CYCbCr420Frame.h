//Local
#include "AVideoFrame.h"
#include "EPixelFormat.h"

namespace SJCMMLib
{
	//Forward Declarations
	class CR8G8B8Frame;

	class CYCbCr420Frame : public AVideoFrame
	{
	public:
		//Constructor
		CYCbCr420Frame(uint16 width, uint16 height);
		
		//Functions
		AVideoFrame *Convert(EPixelFormat pixelFormat) const;
		CYCbCr420Frame *Copy();
		EPixelFormat GetPixelFormat() const;
		

		inline uint32 GetChromaBlueChannelLineSize() const
		{
			return this->width / 4;
		}

		inline uint32 GetChromaBlueChannelSize() const
		{
			return this->width * this->height / 4;
		}

		inline uint32 GetChromaRedChannelLineSize() const
		{
			return this->width / 4;
		}

		inline uint32 GetChromaRedChannelSize() const
		{
			return this->width * this->height / 4;
		}

		inline uint32 GetLumaChannelLineSize() const
		{
			return this->width;
		}

		inline uint32 GetLumaChannelSize() const
		{
			return this->width * this->height;
		}
	};
}
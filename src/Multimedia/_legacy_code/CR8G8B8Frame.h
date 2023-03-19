//Local
#include "AVideoFrame.h"

namespace SJCMMLib
{
	class CR8G8B8Frame : public AVideoFrame
	{
		//Constructor
		CR8G8B8Frame(uint16 width, uint16 height);
		//Methods
		AVideoFrame *Convert(EPixelFormat pixelFormat) const;
		EPixelFormat GetPixelFormat() const;
	};
}
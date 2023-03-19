//Local
#include "AVideoDecoder.h"

namespace SJCMMLib
{
	class CBGR24Decoder : public AVideoDecoder
	{
	public:
		//Functions
		EPixelFormat GetFramePixelFormat() const;
	};
}
//Class Header
#include "__CBGR24Decoder.h"
//Local
#include "CR8G8B8Frame.h"
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;


EPixelFormat CBGR24Decoder::GetFramePixelFormat() const
{
	return PIXELFORMAT_R8G8B8;
}
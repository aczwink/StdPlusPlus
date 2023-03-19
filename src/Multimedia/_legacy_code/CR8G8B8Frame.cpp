//Class Header
#include "CR8G8B8Frame.h"
//Global
#include <iostream>
//Namespaces
using namespace SJCLib;
using namespace SJCMMLib;

//Constructor
CR8G8B8Frame::CR8G8B8Frame(uint16 width, uint16 height)
{	
	this->aspectRatio.Init(width, height);
	this->aspectRatio.Reduce();
}

//Public methods
AVideoFrame *CR8G8B8Frame::Convert(EPixelFormat pixelFormat) const
{
	return NULL;
}

EPixelFormat CR8G8B8Frame::GetPixelFormat() const
{
	return PIXELFORMAT_R8G8B8;
}
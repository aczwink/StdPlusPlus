#pragma once
//SJCLib
#include <SJCLib.h>

#ifdef BUILD_MMLIB
#define SJCMMLIB_API    __declspec(dllexport)
#else
#define SJCMMLIB_API    __declspec(dllimport)
#endif

namespace SJCMMLib
{
	//Enums
	enum EChannelLayout
	{
		CHANNELLAYOUT_INVALID,
		CHANNELLAYOUT_MONO,
		CHANNELLAYOUT_STEREO, //left, right
	};

	enum ESampleFormat
	{
		SAMPLEFORMAT_INVALID,
		SAMPLEFORMAT_FLOAT_32BIT, //in range [-1, 1]
		SAMPLEFORMAT_SIGNED_16BIT
	};
}
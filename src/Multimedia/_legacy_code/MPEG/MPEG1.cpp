//Main header
#include "MPEG1.h"

bool FindNextStartCode(uint32 &refState, ASeekableInputStream &refInput)
{
	while(true)
	{
		if((refState & 0xFFFFFF00) == (MPEG1_STARTCODE_PREFIX << 8))
			return true;

		refState = (refState << 8) | refInput.ReadByte();
		if(refInput.HitEnd())
		{
			refState >>= 8;
			break;
		}
	}

	return false;
}
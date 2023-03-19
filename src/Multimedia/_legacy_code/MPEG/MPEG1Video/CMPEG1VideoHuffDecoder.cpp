//Class header
#include "CMPEG1VideoDecoder.h"

bool CMPEG1VideoDecoder::ReadDCTCoeff(bool first, uint8 &refRun, int16 &refLevel, CBitInputStream &refBitInput) const
{
	//2-B.5c

#define LEVEL(x) (refBitInput.Read(1) ? -x : x)

	if(refBitInput.Read(1))
	{
		//Read: 1
		if(first)
		{
			refRun = 0;
			refLevel = LEVEL(1);
			return true;
		}

		if(refBitInput.Read(1))
		{
			//Read: 11

			ASSERT(!first);

			refRun = 0;
			refLevel = LEVEL(1);
			return true;
		}

		//Read: 10 -> end of block
		return false;
	}

	//Read: 0

	if(refBitInput.Read(1))
	{
		//Read: 01

		if(refBitInput.Read(1))
		{
			//Read: 011
			refRun = 1;
			refLevel = LEVEL(1);
			return true;
		}

		//Read: 010
		if(refBitInput.Read(1))
		{
			//Read: 0101
			refRun = 2;
			refLevel = LEVEL(1);
			return true;
		}

		//Read: 0100
		refRun = 0;
		refLevel = LEVEL(2);
		return true;
	}

	//Read: 00

	if(refBitInput.Read(1))
	{
		//Read: 001

		if(refBitInput.Read(1))
		{
			//Read: 0011
			if(refBitInput.Read(1))
			{
				//Read: 0011 1
				refRun = 3;
				refLevel = LEVEL(1);
				return true;
			}

			//Read: 0011 0
			refRun = 4;
			refLevel = LEVEL(1);
			return true;
		}

		//Read: 0010

		if(refBitInput.Read(1))
		{
			//Read: 0010 1

			refRun = 0;
			refLevel = LEVEL(3);
			return true;
		}

		//Read: 0010 0

		if(refBitInput.Read(1))
		{
			//Read: 0010 01

			if(refBitInput.Read(1))
			{
				//Read: 0010 011

				if(refBitInput.Read(1))
				{
					//Read: 0010 0111
					refRun = 10;
					refLevel = LEVEL(1);
					return true;
				}

				//Read: 0010 0110
				refRun = 0;
				refLevel = LEVEL(5);
				return true;
			}

			//Read: 0010 010
			if(refBitInput.Read(1))
			{
				//Read: 0010 0101
				refRun = 1;
				refLevel = LEVEL(3);
				return true;
			}

			//Read: 0010 0100
			refRun = 3;
			refLevel = LEVEL(2);
			return true;
		}

		//Read: 0010 00
		if(refBitInput.Read(1))
		{
			//Read: 0010 001
			if(refBitInput.Read(1))
			{
				//Read: 0010 0011
				refRun = 11;
				refLevel = LEVEL(1);
				return true;
			}

			//Read: 0010 0010
			refRun = 12;
			refLevel = LEVEL(1);
			return true;
		}

		//Read: 0010 000
		if(refBitInput.Read(1))
		{
			//Read: 0010 0001
			refRun = 0;
			refLevel = LEVEL(6);
			return true;
		}

		//Read: 0010 0000
		refRun = 13;
		refLevel = LEVEL(1);
		return true;
	}

	//Read: 000

	if(refBitInput.Read(1))
	{
		//Read: 0001

		if(refBitInput.Read(1))
		{
			//Read: 0001 1

			if(refBitInput.Read(1))
			{
				//Read: 0001 11

				refRun = 5;
				refLevel = LEVEL(1);
				return true;
			}

			//Read: 0001 10
			refRun = 1;
			refLevel = LEVEL(2);
			return true;
		}

		//Read: 0001 0
		if(refBitInput.Read(1))
		{
			//Read: 0001 01
			refRun = 6;
			refLevel = LEVEL(1);
			return true;
		}

		//Read: 0001 00
		refRun = 7;
		refLevel = LEVEL(1);
		return true;
	}

	//Read: 0000

	if(refBitInput.Read(1))
	{
		//Read: 0000 1

		if(refBitInput.Read(1))
		{
			//Read: 0000 11
			if(refBitInput.Read(1))
			{
				//Read: 0000 111
				refRun = 8;
				refLevel = LEVEL(1);
				return true;
			}

			//Read: 0000 110
			refRun = 0;
			refLevel = LEVEL(4);
			return true;
		}

		//Read: 0000 10
		if(refBitInput.Read(1))
		{
			//Read: 0000 101
			refRun = 9;
			refLevel = LEVEL(1);
			return true;
		}

		//Read: 0000 100
		refRun = 2;
		refLevel = LEVEL(2);
		return true;
	}

	//Read: 0000 0

	if(refBitInput.Read(1))
	{
		uint8 buf;

		//Read: 0000 01 -> Escape (see table 2-B.5g)

		refRun = (uint8)refBitInput.Read(6);

		//first byte indicates length of level
		buf = (uint8)refBitInput.Read(8);

		if(buf == 0x80) //1000 0000
		{
			//16 bit
			refLevel = (int16)refBitInput.Read(8) - 256;

			ASSERT(refLevel >= -255 && refLevel <= -128); //-255 to -128, rest forbidden
		}
		else if(buf == 0)
		{
			//16 bit
			refLevel = (int16)refBitInput.Read(8);

			ASSERT(refLevel >= 128 && refLevel <= 255);
		}
		else
		{
			//buf = level

			refLevel = (buf < 128) ? buf : (buf - 256);

			ASSERT((int8)buf >= -127 && (int8)buf <= 127); //-127 to 127, but not 0, buf can't be 0 in this case
		}

		return true;
	}

	//Read: 0000 00

	if(refBitInput.Read(1))
	{
		//Read: 0000 001

		if(refBitInput.Read(1))
		{
			//Read: 0000 0011
			if(refBitInput.Read(1))
			{
				//Read: 0000 0011 1
				if(refBitInput.Read(1))
				{
					//Read: 0000 0011 11
					refRun = 4;
					refLevel = LEVEL(2);
					return true;
				}

				//Read: 0000 0011 10
				refRun = 14;
				refLevel = LEVEL(1);
				return true;
			}

			//Read: 0000 0011 0
			if(refBitInput.Read(1))
			{
				//Read: 0000 0011 01
				refRun = 15;
				refLevel = LEVEL(1);
				return true;
			}

			//Read: 0000 0011 00
			refRun = 1;
			refLevel = LEVEL(4);
			return true;
		}

		//Read: 0000 0010

		if(refBitInput.Read(1))
		{
			//Read: 0000 0010 1

			if(refBitInput.Read(1))
			{
				//Read: 0000 0010 11
				refRun = 2;
				refLevel = LEVEL(3);
				return true;
			}

			//Read: 0000 0010 10
			refRun = 0;
			refLevel = LEVEL(7);
			return true;
		}

		//Read: 0000 0010 0
		if(refBitInput.Read(1))
		{
			//Read: 0000 0010 01
			refRun = 5;
			refLevel = LEVEL(2);
			return true;
		}

		//Read: 0000 0010 00
		refRun = 16;
		refLevel = LEVEL(1);
		return true;
	}

	//Read: 0000 000

	if(refBitInput.Read(1))
	{
		//Read: 0000 0001
		if(refBitInput.Read(1))
		{
			//Read: 0000 0001 1
			if(refBitInput.Read(1))
			{
				//Read: 0000 0001 11
				if(refBitInput.Read(1))
				{
					//Read: 0000 0001 111
					if(refBitInput.Read(1))
					{
						//Read: 0000 0001 1111
						refRun = 17;
						refLevel = LEVEL(1);
						return true;
					}

					//Read: 0000 0001 1110
					refRun = 6;
					refLevel = LEVEL(2);
					return true;
				}

				//Read: 0000 0001 110
				if(refBitInput.Read(1))
				{
					//Read: 0000 0001 1101
					refRun = 0;
					refLevel = LEVEL(8);
					return true;
				}

				//Read: 0000 0001 1100
				refRun = 3;
				refLevel = LEVEL(3);
				return true;
			}

			//Read: 0000 0001 10
			if(refBitInput.Read(1))
			{
				//Read: 0000 0001 101
				if(refBitInput.Read(1))
				{
					//Read: 0000 0001 1011
					refRun = 1;
					refLevel = LEVEL(5);
					return true;
				}

				//Read: 0000 0001 1010
				refRun = 18;
				refLevel = LEVEL(1);
				return true;
			}

			//Read: 0000 0001 100
			if(refBitInput.Read(1))
			{
				//Read: 0000 0001 1001
				refRun = 19;
				refLevel = LEVEL(1);
				return true;
			}

			//Read: 0000 0001 1000
			refRun = 0;
			refLevel = LEVEL(9);
			return true;
		}

		//Read: 0000 0001 0
		if(refBitInput.Read(1))
		{
			//Read: 0000 0001 01
			if(refBitInput.Read(1))
			{
				//Read: 0000 0001 011
				if(refBitInput.Read(1))
				{
					//Read: 0000 0001 0111
					refRun = 20;
					refLevel = LEVEL(1);
					return true;
				}

				//Read: 0000 0001 0110
				refRun = 21;
				refLevel = LEVEL(1);
				return true;
			}

			//Read: 0000 0001 010
			if(refBitInput.Read(1))
			{
				//Read: 0000 0001 0101
				refRun = 7;
				refLevel = LEVEL(2);
				return true;
			}

			//Read: 0000 0001 0100
			refRun = 2;
			refLevel = LEVEL(4);
			return true;
		}

		//Read: 0000 0001 00
		if(refBitInput.Read(1))
		{
			//Read: 0000 0001 001
			if(refBitInput.Read(1))
			{
				//Read: 0000 0001 0011
				refRun = 0;
				refLevel = LEVEL(10);
				return true;
			}

			//Read: 0000 0001 0010
			refRun = 4;
			refLevel = LEVEL(3);
			return true;
		}

		//Read: 0000 0001 000
		if(refBitInput.Read(1))
		{
			//Read: 0000 0001 0001
			refRun = 8;
			refLevel = LEVEL(2);
			return true;
		}

		//Read: 0000 0001 0000
		refRun = 0;
		refLevel = LEVEL(11);
		return true;
	}

	//Read: 0000 0000

	if(refBitInput.Read(1))
	{
		//Read: 0000 0000 1

		if(refBitInput.Read(1))
		{
			//Read: 0000 0000 11
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 111
				if(refBitInput.Read(1))
				{
					//Read: 0000 0000 1111
					if(refBitInput.Read(1))
					{
						//Read: 0000 0000 1111 1
						refRun = 22;
						refLevel = LEVEL(1);
						return true;
					}

					//Read: 0000 0000 1111 0
					refRun = 23;
					refLevel = LEVEL(1);
					return true;
				}

				//Read: 0000 0000 1110
				if(refBitInput.Read(1))
				{
					//Read: 0000 0000 1110 1
					refRun = 24;
					refLevel = LEVEL(1);
					return true;
				}

				//Read: 0000 0000 1110 0
				refRun = 25;
				refLevel = LEVEL(1);
				return true;
			}

			//Read: 0000 0000 110
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 1101
				if(refBitInput.Read(1))
				{
					//Read: 0000 0000 1101 1
					refRun = 26;
					refLevel = LEVEL(1);
					return true;
				}

				//Read: 0000 0000 1101 0
				refRun = 0;
				refLevel = LEVEL(12);
				return true;
			}

			//Read: 0000 0000 1100
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 1100 1
				refRun = 0;
				refLevel = LEVEL(13);
				return true;
			}

			//Read: 0000 0000 1100 0
			refRun = 0;
			refLevel = LEVEL(14);
			return true;
		}

		//Read: 0000 0000 10
		if(refBitInput.Read(1))
		{
			//Read: 0000 0000 101
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 1011
				if(refBitInput.Read(1))
				{
					//Read: 0000 0000 1011 1
					refRun = 0;
					refLevel = LEVEL(15);
					return true;
				}

				//Read: 0000 0000 1011 0
				refRun = 1;
				refLevel = LEVEL(6);
				return true;
			}

			//Read: 0000 0000 1010
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 1010 1
				refRun = 1;
				refLevel = LEVEL(7);
				return true;
			}

			//Read: 0000 0000 1010 0
			refRun = 2;
			refLevel = LEVEL(5);
			return true;
		}

		//Read: 0000 0000 100
		if(refBitInput.Read(1))
		{
			//Read: 0000 0000 1001
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 1001 1
				refRun = 3;
				refLevel = LEVEL(4);
				return true;
			}

			//Read: 0000 0000 1001 0
			refRun = 5;
			refLevel = LEVEL(3);
			return true;
		}

		//Read: 0000 0000 1000
		if(refBitInput.Read(1))
		{
			//Read: 0000 0000 1000 1
			refRun = 9;
			refLevel = LEVEL(2);
			return true;
		}

		//Read: 0000 0000 1000 0
		refRun = 10;
		refLevel = LEVEL(2);
		return true;
	}

	//Read: 0000 0000 0

	if(refBitInput.Read(1))
	{
		//Read: 0000 0000 01
		if(refBitInput.Read(1))
		{
			//Read: 0000 0000 011
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 0111
				if(refBitInput.Read(1))
				{
					//Read: 0000 0000 0111 1
					if(refBitInput.Read(1))
					{
						//Read: 0000 0000 0111 11
						refRun = 0;
						refLevel = LEVEL(16);
						return true;
					}

					//Read: 0000 0000 0111 10
					refRun = 0;
					refLevel = LEVEL(17);
					return true;
				}

				//Read: 0000 0000 0111 0
				if(refBitInput.Read(1))
				{
					//Read: 0000 0000 0111 01
					refRun = 0;
					refLevel = LEVEL(18);
					return true;
				}

				//Read: 0000 0000 0111 00
				refRun = 0;
				refLevel = LEVEL(19);
				return true;
			}

			//Read: 0000 0000 0110
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 0110 1
				if(refBitInput.Read(1))
				{
					//Read: 0000 0000 0110 11
					refRun = 0;
					refLevel = LEVEL(20);
					return true;
				}

				//Read: 0000 0000 0110 10
				refRun = 0;
				refLevel = LEVEL(21);
				return true;
			}

			//Read: 0000 0000 0110 0
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 0110 01
				refRun = 0;
				refLevel = LEVEL(22);
				return true;
			}

			//Read: 0000 0000 0110 00
			refRun = 0;
			refLevel = LEVEL(23);
			return true;
		}

		//Read: 0000 0000 010
		if(refBitInput.Read(1))
		{
			//Read: 0000 0000 0101
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 0101 1
				if(refBitInput.Read(1))
				{
					//Read: 0000 0000 0101 11
					refRun = 0;
					refLevel = LEVEL(24);
					return true;
				}

				//Read: 0000 0000 0101 10
				refRun = 0;
				refLevel = LEVEL(25);
				return true;
			}

			//Read: 0000 0000 0101 0
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 0101 01
				refRun = 0;
				refLevel = LEVEL(26);
				return true;
			}

			//Read: 0000 0000 0101 00
			refRun = 0;
			refLevel = LEVEL(27);
			return true;
		}

		//Read: 0000 0000 0100
		if(refBitInput.Read(1))
		{
			//Read: 0000 0000 0100 1
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 0100 11
				refRun = 0;
				refLevel = LEVEL(28);
				return true;
			}

			//Read: 0000 0000 0100 10
			refRun = 0;
			refLevel = LEVEL(29);
			return true;
		}

		//Read: 0000 0000 0100 0
		if(refBitInput.Read(1))
		{
			//Read: 0000 0000 0100 01
			refRun = 0;
			refLevel = LEVEL(30);
			return true;
		}

		//Read: 0000 0000 0100 00
		refRun = 0;
		refLevel = LEVEL(31);
		return true;
	}

	//Read: 0000 0000 00

	if(refBitInput.Read(1))
	{
		//Read: 0000 0000 001

		if(refBitInput.Read(1))
		{
			//Read: 0000 0000 0011

			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 0011 1

				if(refBitInput.Read(1))
				{
					//Read: 0000 0000 0011 11

					if(refBitInput.Read(1))
					{
						//Read: 0000 0000 0011 111

						refRun = 1;
						refLevel = LEVEL(8);
						return true;
					}

					//Read: 0000 0000 0011 110
					refRun = 1;
					refLevel = LEVEL(9);
					return true;
				}

				//Read: 0000 0000 0011 10
				if(refBitInput.Read(1))
				{
					//Read: 0000 0000 0011 101
					refRun = 1;
					refLevel = LEVEL(10);
					return true;
				}

				//Read: 0000 0000 0011 100
				refRun = 1;
				refLevel = LEVEL(11);
				return true;
			}

			//Read: 0000 0000 0011 0
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 0011 01
				if(refBitInput.Read(1))
				{
					//Read: 0000 0000 0011 011
					refRun = 1;
					refLevel = LEVEL(12);
					return true;
				}

				//Read: 0000 0000 0011 010
				refRun = 1;
				refLevel = LEVEL(13);
				return true;
			}

			//Read: 0000 0000 0011 00
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 0011 001
				refRun = 1;
				refLevel = LEVEL(14);
				return true;
			}

			//Read: 0000 0000 0011 000
			refRun = 0;
			refLevel = LEVEL(32);
			return true;
		}

		//Read: 0000 0000 0010
		if(refBitInput.Read(1))
		{
			//Read: 0000 0000 0010 1
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 0010 11
				if(refBitInput.Read(1))
				{
					//Read: 0000 0000 0010 111
					refRun = 0;
					refLevel = LEVEL(33);
					return true;
				}

				//Read: 0000 0000 0010 110
				refRun = 0;
				refLevel = LEVEL(34);
				return true;
			}

			//Read: 0000 0000 0010 10
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 0010 101
				refRun = 0;
				refLevel = LEVEL(35);
				return true;
			}

			//Read: 0000 0000 0010 100
			refRun = 0;
			refLevel = LEVEL(36);
			return true;
		}

		//Read: 0000 0000 0010 0
		if(refBitInput.Read(1))
		{
			//Read: 0000 0000 0010 01
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 0010 011
				refRun = 0;
				refLevel = LEVEL(37);
				return true;
			}

			//Read: 0000 0000 0010 010
			refRun = 0;
			refLevel = LEVEL(38);
			return true;
		}

		//Read: 0000 0000 0010 00
		if(refBitInput.Read(1))
		{
			//Read: 0000 0000 0010 001
			refRun = 0;
			refLevel = LEVEL(39);
			return true;
		}

		//Read: 0000 0000 0010 000
		refRun = 0;
		refLevel = LEVEL(40);
		return true;
	}

	//Read: 0000 0000 000
	if(refBitInput.Read(1))
	{
		//Read: 0000 0000 0001
		if(refBitInput.Read(1))
		{
			//Read: 0000 0000 0001 1
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 0001 11
				if(refBitInput.Read(1))
				{
					//Read: 0000 0000 0001 111
					if(refBitInput.Read(1))
					{
						//Read: 0000 0000 0001 1111
						refRun = 27;
						refLevel = LEVEL(1);
						return true;
					}

					//Read: 0000 0000 0001 1110
					refRun = 28;
					refLevel = LEVEL(1);
					return true;
				}

				//Read: 0000 0000 0001 110
				if(refBitInput.Read(1))
				{
					//Read: 0000 0000 0001 1101
					refRun = 29;
					refLevel = LEVEL(1);
					return true;
				}

				//Read: 0000 0000 0001 1100
				refRun = 30;
				refLevel = LEVEL(1);
				return true;
			}

			//Read: 0000 0000 0001 10
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 0001 101

				if(refBitInput.Read(1))
				{
					//Read: 0000 0000 0001 1011
					refRun = 31;
					refLevel = LEVEL(1);
					return true;
				}

				//Read: 0000 0000 0001 1010
				refRun = 11;
				refLevel = LEVEL(2);
				return true;
			}

			//Read: 0000 0000 0001 100
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 0001 1001
				refRun = 12;
				refLevel = LEVEL(2);
				return true;
			}

			//Read: 0000 0000 0001 1000
			refRun = 13;
			refLevel = LEVEL(2);
			return true;
		}

		//Read: 0000 0000 0001 0
		if(refBitInput.Read(1))
		{
			//Read: 0000 0000 0001 01
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 0001 011
				if(refBitInput.Read(1))
				{
					//Read: 0000 0000 0001 0111
					refRun = 14;
					refLevel = LEVEL(2);
					return true;
				}

				//Read: 0000 0000 0001 0110
				refRun = 15;
				refLevel = LEVEL(2);
				return true;
			}

			//Read: 0000 0000 0001 010
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 0001 0101
				refRun = 16;
				refLevel = LEVEL(2);
				return true;
			}

			//Read: 0000 0000 0001 0100
			refRun = 6;
			refLevel = LEVEL(3);
			return true;
		}

		//Read: 0000 0000 0001 00
		if(refBitInput.Read(1))
		{
			//Read: 0000 0000 0001 001
			if(refBitInput.Read(1))
			{
				//Read: 0000 0000 0001 0011
				refRun = 1;
				refLevel = LEVEL(15);
				return true;
			}

			//Read: 0000 0000 0001 0010
			refRun = 1;
			refLevel = LEVEL(16);
			return true;
		}

		//Read: 0000 0000 0001 000
		if(refBitInput.Read(1))
		{
			//Read: 0000 0000 0001 0001
			refRun = 1;
			refLevel = LEVEL(17);
			return true;
		}

		//Read: 0000 0000 0001 0000
		refRun = 1;
		refLevel = LEVEL(18);
		return true;
	}

	//Read: 0000 0000 0000

#undef LEVEL

	ASSERT(0);
	return false;
}

uint8 CMPEG1VideoDecoder::ReadDCTDcSizeChrominance(CBitInputStream &refBitInput) const
{
	//2-B.5b

	if(refBitInput.Read(1))
	{
		//Read: 1
		if(refBitInput.Read(1))
		{
			//Read: 11
			if(refBitInput.Read(1))
			{
				//Read: 111
				if(refBitInput.Read(1))
				{
					//Read: 1111

					if(refBitInput.Read(1))
					{
						//Read: 1111 1

						if(refBitInput.Read(1))
						{
							//Read: 1111 11

							if(refBitInput.Read(1))
							{
								//Read: 1111 111

								if(refBitInput.Read(1))
								{
									//Read: 1111 1111 ILLEGAL
									ASSERT(0);
								}

								//Read: 1111 1110
								return 8;
							}

							//Read: 1111 110
							return 7;
						}

						//Read: 1111 10
						return 6;
					}

					//Read: 1111 0
					return 5;
				}

				//Read: 1110
				return 4;
			}

			//Read: 110
			return 3;
		}

		//Read: 10
		return 2;
	}

	//Read: 0
	if(refBitInput.Read(1))
	{
		//Read: 01
		return 1;
	}

	//Read: 00 -> End
	return 0;
}

uint8 CMPEG1VideoDecoder::ReadDCTDcSizeLuminance(CBitInputStream &refBitInput) const
{
	//2-B.5a

	if(refBitInput.Read(1))
	{
		//Read: 1

		if(refBitInput.Read(1))
		{
			//Read: 11

			if(refBitInput.Read(1))
			{
				//Read: 111

				if(refBitInput.Read(1))
				{
					//Read: 1111

					if(refBitInput.Read(1))
					{
						//Read: 1111 1

						if(refBitInput.Read(1))
						{
							//Read: 1111 11

							if(refBitInput.Read(1))
							{
								//Read: 1111 111 ILLEGAL
								ASSERT(0);
							}

							//Read: 1111 110
							return 8;
						}

						//Read: 1111 10
						return 7;
					}

					//Read: 1111 0
					return 6;
				}

				//Read: 1110
				return 5;
			}

			//Read: 110
			return 4;
		}

		//Read: 10
		if(refBitInput.Read(1))
		{
			//Read: 101
			return 3;
		}

		//Read: 100
		return 0;
	}

	//Read: 0

	if(refBitInput.Read(1))
	{
		//Read: 01
		return 2;
	}

	//Read: 00
	return 1;
}

uint8 CMPEG1VideoDecoder::ReadMacroblockAddressIncrement(CBitInputStream &refBitInput) const
{
	//2-B.1

	if(refBitInput.Read(1))
	{
		//Read: 1
		return 1;
	}

	//Read: 0
	if(refBitInput.Read(1))
	{
		//Read: 01

		if(refBitInput.Read(1))
		{
			//Read: 011
			return 2;
		}

		//Read: 010
		return 3;
	}

	//Read: 00
	if(refBitInput.Read(1))
	{
		//Read: 001

		if(refBitInput.Read(1))
		{
			//Read: 0011
			return 4;
		}

		//Read: 0010
		return 5;
	}

	//Read: 000
	if(refBitInput.Read(1))
	{
		//Read: 0001
		if(refBitInput.Read(1))
		{
			//Read: 0001 1
			return 6;
		}

		//Read: 0001 0
		return 7;
	}

	//Read: 0000
	if(refBitInput.Read(1))
	{
		//Read: 0000 1
		if(refBitInput.Read(1))
		{
			//Read: 0000 11
			if(refBitInput.Read(1))
			{
				//Read: 0000 111
				return 8;
			}

			//Read: 0000 110
			return 9;
		}

		//Read: 0000 10
		if(refBitInput.Read(1))
		{
			//Read: 0000 101
			if(refBitInput.Read(1))
			{
				//Read: 0000 1011
				return 10;
			}

			//Read: 0000 1010
			return 11;
		}

		//Read: 0000 100
		if(refBitInput.Read(1))
		{
			//Read: 0000 1001
			return 12;
		}

		//Read: 0000 1000
		return 13;
	}

	//Read: 0000 0
	if(refBitInput.Read(1))
	{
		//Read: 0000 01

		if(refBitInput.Read(1))
		{
			//Read: 0000 011
			if(refBitInput.Read(1))
			{
				//Read: 0000 0111
				return 14;
			}

			//Read: 0000 0110
			return 15;
		}

		//Read: 0000 010
		if(refBitInput.Read(1))
		{
			//Read: 0000 0101
			if(refBitInput.Read(1))
			{
				//Read: 0000 0101 1
				if(refBitInput.Read(1))
				{
					//Read: 0000 0101 11
					return 16;
				}

				//Read: 0000 0101 10
				return 17;
			}

			//Read: 0000 0101 0
			if(refBitInput.Read(1))
			{
				//Read: 0000 0101 01
				return 18;
			}

			//Read: 0000 0101 00
			return 19;
		}

		//Read: 0000 0100
		if(refBitInput.Read(1))
		{
			//Read: 0000 0100 1
			if(refBitInput.Read(1))
			{
				//Read: 0000 0100 11
				return 20;
			}

			//Read: 0000 0100 10
			return 21;
		}

		//Read: 0000 0100 0
		if(refBitInput.Read(1))
		{
			//Read: 0000 0100 01
			if(refBitInput.Read(1))
			{
				//Read: 0000 0100 011
				return 22;
			}

			//Read: 0000 0100 010
			return 23;
		}

		//Read: 0000 0100 00
		if(refBitInput.Read(1))
		{
			//Read: 0000 0100 001
			return 24;
		}

		//Read: 0000 0100 000
		return 25;
	}

	//Read: 0000 00
	if(refBitInput.Read(1))
	{
		//Read: 0000 001
		if(refBitInput.Read(1))
		{
			//Read: 0000 0011
			if(refBitInput.Read(1))
			{
				//Read: 0000 0011 1
				if(refBitInput.Read(1))
				{
					//Read: 0000 0011 11
					if(refBitInput.Read(1))
					{
						//Read: 0000 0011 111
						return 26;
					}

					//Read: 0000 0011 110
					return 27;
				}

				//Read: 0000 0011 10
				if(refBitInput.Read(1))
				{
					//Read: 0000 0011 101
					return 28;
				}

				//Read: 0000 0011 100
				return 29;
			}

			//Read: 0000 0011 0
			if(refBitInput.Read(1))
			{
				//Read: 0000 0011 01
				if(refBitInput.Read(1))
				{
					//Read: 0000 0011 011
					return 30;
				}

				//Read: 0000 0011 010
				return 31;
			}

			//Read: 0000 0011 00
			if(refBitInput.Read(1))
			{
				//Read: 0000 0011 001
				return 32;
			}

			//Read: 0000 0011 000
			return 33;
		}

		//Read: 0000 0010 -> ILLEGAL
		ASSERT(0);
	}

	//Read: 0000 000
	if(refBitInput.Read(1))
	{
		//Read: 0000 0001
		if(refBitInput.Get(3) == 0x7)
		{
			//0000 0001 111
			refBitInput.Skip(3);
			ASSERT(0);
		}

		if(refBitInput.Get(3) == 0)
		{
			//0000 0001 000 -> macroblock_escape
			refBitInput.Skip(3);

			//2.4.3.6
			return 33 + this->ReadMacroblockAddressIncrement(refBitInput);
		}
	}

	//Read: 0000 0000 -> ILLEGAL
	ASSERT(0);

	return 0;
}

uint8 CMPEG1VideoDecoder::ReadMacroblockPattern(CBitInputStream &refBitInput) const
{
	//2-B.3
	if(refBitInput.Read(1))
	{
		//Read: 1
		if(refBitInput.Read(1))
		{
			//Read: 11
			if(refBitInput.Read(1))
			{
				//Read: 111
				return 60;
			}

			//Read: 110
			if(refBitInput.Read(1))
			{
				//Read: 1101
				return 4;
			}

			//Read: 1100
			return 8;
		}

		//Read: 10
		if(refBitInput.Read(1))
		{
			//Read: 101
			if(refBitInput.Read(1))
			{
				//Read: 1011
				return 16;
			}

			//Read: 1010
			return 32;
		}

		//Read: 100
		if(refBitInput.Read(1))
		{
			//Read: 1001
			if(refBitInput.Read(1))
			{
				//Read: 1001 1
				return 12;
			}

			//Read: 1001 0
			return 48;
		}

		//Read: 1000
		if(refBitInput.Read(1))
		{
			//Read: 1000 1
			return 20;
		}

		//Read: 1000 0
		return 40;
	}

	//Read: 0
	if(refBitInput.Read(1))
	{
		//Read: 01
		if(refBitInput.Read(1))
		{
			//Read: 011
			if(refBitInput.Read(1))
			{
				//Read: 0111
				if(refBitInput.Read(1))
				{
					//Read: 0111 1
					return 28;
				}

				//Read: 0111 0
				return 44;
			}

			//Read: 0110
			if(refBitInput.Read(1))
			{
				//Read: 0110 1
				return 52;
			}

			//Read: 0110 0
			return 56;
		}

		//Read: 010
		if(refBitInput.Read(1))
		{
			//Read: 0101
			if(refBitInput.Read(1))
			{
				//Read: 0101 1
				return 1;
			}

			//Read: 0101 0
			return 61;
		}

		//Read: 0100
		if(refBitInput.Read(1))
		{
			//Read: 0100 1
			return 2;
		}

		//Read: 0100 0
		return 62;
	}

	//Read: 00
	if(refBitInput.Read(1))
	{
		//Read: 001
		if(refBitInput.Read(1))
		{
			//Read: 0011
			if(refBitInput.Read(1))
			{
				//Read: 0011 1
				if(refBitInput.Read(1))
				{
					//Read: 0011 11
					return 24;
				}

				//Read: 0011 10
				return 36;
			}

			//Read: 0011 0
			if(refBitInput.Read(1))
			{
				//Read: 0011 01
				return 3;
			}

			//Read: 0011 00
			return 63;
		}

		//Read: 0010
		if(refBitInput.Read(1))
		{
			//Read: 0010 1
			if(refBitInput.Read(1))
			{
				//Read: 0010 11
				if(refBitInput.Read(1))
				{
					//Read: 0010 111
					return 5;
				}

				//Read: 0010 110
				return 9;
			}

			//Read: 0010 10
			if(refBitInput.Read(1))
			{
				//Read: 0010 101
				return 17;
			}

			//Read: 0010 100
			return 33;
		}

		//Read: 0010 0
		if(refBitInput.Read(1))
		{
			//Read: 0010 01
			if(refBitInput.Read(1))
			{
				//Read: 0010 011
				return 6;
			}

			//Read: 0010 010
			return 10;
		}

		//Read: 0010 00
		if(refBitInput.Read(1))
		{
			//Read: 0010 001
			return 18;
		}

		//Read: 0010 000
		return 34;
	}


	//Read: 000
	if(refBitInput.Read(1))
	{
		//Read: 0001
		if(refBitInput.Read(1))
		{
			//Read: 0001 1
			if(refBitInput.Read(1))
			{
				//Read: 0001 11
				if(refBitInput.Read(1))
				{
					//Read: 0001 111
					if(refBitInput.Read(1))
					{
						//Read: 0001 1111
						return 7;
					}

					//Read: 0001 1110
					return 11;
				}

				//Read: 0001 110
				if(refBitInput.Read(1))
				{
					//Read: 0001 1101
					return 19;
				}

				//Read: 0001 1100
				return 35;
			}

			//Read: 0001 10
			if(refBitInput.Read(1))
			{
				//Read: 0001 101
				if(refBitInput.Read(1))
				{
					//Read: 0001 1011
					return 13;
				}

				//Read: 0001 1010
				return 49;
			}

			//Read: 0001 100
			if(refBitInput.Read(1))
			{
				//Read: 0001 1001
				return 21;
			}

			//Read: 0001 1000
			return 41;
		}

		//Read: 0001 0
		if(refBitInput.Read(1))
		{
			//Read: 0001 01
			if(refBitInput.Read(1))
			{
				//Read: 0001 011
				if(refBitInput.Read(1))
				{
					//Read: 0001 0111
					return 14;
				}

				//Read: 0001 0110
				return 50;
			}

			//Read: 0001 010
			if(refBitInput.Read(1))
			{
				//Read: 0001 0101
				return 22;
			}

			//Read: 0001 0100
			return 42;
		}

		//Read: 0001 00
		if(refBitInput.Read(1))
		{
			//Read: 0001 001
			if(refBitInput.Read(1))
			{
				//Read: 0001 0011
				return 15;
			}

			//Read: 0001 0010
			return 51;
		}

		//Read: 0001 000
		if(refBitInput.Read(1))
		{
			//Read: 0001 0001
			return 23;
		}

		//Read: 0001 0000
		return 43;
	}

	//Read: 0000
	if(refBitInput.Read(1))
	{
		//Read: 0000 1
		if(refBitInput.Read(1))
		{
			//Read: 0000 11
			if(refBitInput.Read(1))
			{
				//Read: 0000 111
				if(refBitInput.Read(1))
				{
					//Read: 0000 1111
					return 25;
				}

				//Read: 0000 1110
				return 37;
			}

			//Read: 0000 110
			if(refBitInput.Read(1))
			{
				//Read: 0000 1101
				return 26;
			}

			//Read: 0000 1100
			return 38;
		}

		//Read: 0000 10
		if(refBitInput.Read(1))
		{
			//Read: 0000 101
			if(refBitInput.Read(1))
			{
				//Read: 0000 1011
				return 29;
			}

			//Read: 0000 1010
			return 45;
		}

		//Read: 0000 100
		if(refBitInput.Read(1))
		{
			//Read: 0000 1001
			return 53;
		}

		//Read: 0000 1000
		return 57;
	}

	//Read: 0000 0
	if(refBitInput.Read(1))
	{
		//Read: 0000 01
		if(refBitInput.Read(1))
		{
			//Read: 0000 011
			if(refBitInput.Read(1))
			{
				//Read: 0000 0111
				return 30;
			}

			//Read: 0000 0110
			return 46;
		}

		//Read: 0000 010
		if(refBitInput.Read(1))
		{
			//Read: 0000 0101
			return 54;
		}

		//Read: 0000 0100
		return 58;
	}

	//Read: 0000 00
	if(refBitInput.Read(1))
	{
		//Read: 0000 001
		if(refBitInput.Read(1))
		{
			//Read: 0000 0011
			if(refBitInput.Read(1))
			{
				//Read: 0000 0011 1
				return 31;
			}

			//Read: 0000 0011 0
			return 47;
		}

		//Read: 0000 0010
		if(refBitInput.Read(1))
		{
			//Read: 0000 0010 1
			return 55;
		}

		//Read: 0000 0010 0
		return 59;
	}

	//Read: 0000 000
	if(refBitInput.Read(1))
	{
		//Read: 0000 0001
		if(refBitInput.Read(1))
		{
			//Read: 0000 0001 1
			return 27;
		}

		//Read: 0000 0001 0
		return 39;
	}

	//Read: 0000 0000
	ASSERT(0);

	return -1;
}

void CMPEG1VideoDecoder::ReadMacroblockType(SMacroblockTypeInfo &refMBTypeInfo, CBitInputStream &refBitInput) const
{
	//2-B.2

	switch(this->currentPicType)
	{
	case PICTURETYPE_I:
		{
			//2-B.2a
			if(refBitInput.Read(1))
			{
				//Read: 1
				refMBTypeInfo.quant = false;
				refMBTypeInfo.motion_forward = false;
				refMBTypeInfo.motion_backward = false;
				refMBTypeInfo.pattern = false;
				refMBTypeInfo.intra = true;
				return;
			}

			//Read: 0
			if(refBitInput.Read(1))
			{
				//Read: 01
				refMBTypeInfo.quant = true;
				refMBTypeInfo.motion_forward = false;
				refMBTypeInfo.motion_backward = false;
				refMBTypeInfo.pattern = false;
				refMBTypeInfo.intra = true;
				return;
			}

			//Read: 00 -> ILLEGAL
			ASSERT(false);
		}
		break;
	case PICTURETYPE_P:
		{
			//2-B.2b

			if(refBitInput.Read(1))
			{
				//Read: 1
				refMBTypeInfo.quant = false;
				refMBTypeInfo.motion_forward = true;
				refMBTypeInfo.motion_backward = false;
				refMBTypeInfo.pattern = true;
				refMBTypeInfo.intra = false;
				return;
			}

			//Read: 0
			if(refBitInput.Read(1))
			{
				//Read: 01
				refMBTypeInfo.quant = false;
				refMBTypeInfo.motion_forward = false;
				refMBTypeInfo.motion_backward = false;
				refMBTypeInfo.pattern = true;
				refMBTypeInfo.intra = false;
				return;
			}

			//Read: 00
			if(refBitInput.Read(1))
			{
				//Read: 001
				refMBTypeInfo.quant = false;
				refMBTypeInfo.motion_forward = true;
				refMBTypeInfo.motion_backward = false;
				refMBTypeInfo.pattern = false;
				refMBTypeInfo.intra = false;
				return;
			}

			//Read: 000
			if(refBitInput.Read(1))
			{
				//Read: 0001
				if(refBitInput.Read(1))
				{
					//Read: 00011
					refMBTypeInfo.quant = false;
					refMBTypeInfo.motion_forward = false;
					refMBTypeInfo.motion_backward = false;
					refMBTypeInfo.pattern = false;
					refMBTypeInfo.intra = true;
					return;
				}

				//Read: 00010
				refMBTypeInfo.quant = true;
				refMBTypeInfo.motion_forward = true;
				refMBTypeInfo.motion_backward = false;
				refMBTypeInfo.pattern = true;
				refMBTypeInfo.intra = false;
				return;
			}

			//Read: 0000
			if(refBitInput.Read(1))
			{
				//Read: 0000 1
				refMBTypeInfo.quant = true;
				refMBTypeInfo.motion_forward = false;
				refMBTypeInfo.motion_backward = false;
				refMBTypeInfo.pattern = true;
				refMBTypeInfo.intra = false;
				return;
			}

			//Read: 0000 0
			if(refBitInput.Read(1))
			{
				//Read: 0000 01
				refMBTypeInfo.quant = true;
				refMBTypeInfo.motion_forward = false;
				refMBTypeInfo.motion_backward = false;
				refMBTypeInfo.pattern = false;
				refMBTypeInfo.intra = true;
				return;
			}

			//Read: 0000 00 -> ILLEGAL
			ASSERT(0);
		}
		break;
	case PICTURETYPE_B:
		{
			//2-B.2c

			if(refBitInput.Read(1))
			{
				//Read: 1
				if(refBitInput.Read(1))
				{
					//Read: 11
					refMBTypeInfo.quant = false;
					refMBTypeInfo.motion_forward = true;
					refMBTypeInfo.motion_backward = true;
					refMBTypeInfo.pattern = true;
					refMBTypeInfo.intra = false;
					return;
				}

				//Read: 10
				refMBTypeInfo.quant = false;
				refMBTypeInfo.motion_forward = true;
				refMBTypeInfo.motion_backward = true;
				refMBTypeInfo.pattern = false;
				refMBTypeInfo.intra = false;
				return;
			}

			//Read: 0
			if(refBitInput.Read(1))
			{
				//Read: 01
				if(refBitInput.Read(1))
				{
					//Read: 011
					refMBTypeInfo.quant = false;
					refMBTypeInfo.motion_forward = false;
					refMBTypeInfo.motion_backward = true;
					refMBTypeInfo.pattern = true;
					refMBTypeInfo.intra = false;
					return;
				}

				//Read: 010
				refMBTypeInfo.quant = false;
				refMBTypeInfo.motion_forward = false;
				refMBTypeInfo.motion_backward = true;
				refMBTypeInfo.pattern = false;
				refMBTypeInfo.intra = false;
				return;
			}

			//Read: 00
			if(refBitInput.Read(1))
			{
				//Read: 001
				if(refBitInput.Read(1))
				{
					//Read: 0011
					refMBTypeInfo.quant = false;
					refMBTypeInfo.motion_forward = true;
					refMBTypeInfo.motion_backward = false;
					refMBTypeInfo.pattern = true;
					refMBTypeInfo.intra = false;
					return;
				}

				//Read: 0010
				refMBTypeInfo.quant = false;
				refMBTypeInfo.motion_forward = true;
				refMBTypeInfo.motion_backward = false;
				refMBTypeInfo.pattern = false;
				refMBTypeInfo.intra = false;
				return;
			}

			//Read: 000
			if(refBitInput.Read(1))
			{
				//Read: 0001
				if(refBitInput.Read(1))
				{
					//Read: 0001 1
					refMBTypeInfo.quant = false;
					refMBTypeInfo.motion_forward = false;
					refMBTypeInfo.motion_backward = false;
					refMBTypeInfo.pattern = false;
					refMBTypeInfo.intra = true;
					return;
				}

				//Read: 0001 0
				refMBTypeInfo.quant = true;
				refMBTypeInfo.motion_forward = true;
				refMBTypeInfo.motion_backward = true;
				refMBTypeInfo.pattern = true;
				refMBTypeInfo.intra = false;
				return;
			}

			//Read: 0000
			if(refBitInput.Read(1))
			{
				//Read: 0000 1
				if(refBitInput.Read(1))
				{
					//Read: 0000 11
					refMBTypeInfo.quant = true;
					refMBTypeInfo.motion_forward = true;
					refMBTypeInfo.motion_backward = false;
					refMBTypeInfo.pattern = true;
					refMBTypeInfo.intra = false;
					return;
				}

				//Read: 0000 10
				refMBTypeInfo.quant = true;
				refMBTypeInfo.motion_forward = false;
				refMBTypeInfo.motion_backward = true;
				refMBTypeInfo.pattern = true;
				refMBTypeInfo.intra = false;
				return;
			}

			//Read: 0000 0
			if(refBitInput.Read(1))
			{
				//Read: 0000 01
				refMBTypeInfo.quant = true;
				refMBTypeInfo.motion_forward = false;
				refMBTypeInfo.motion_backward = false;
				refMBTypeInfo.pattern = false;
				refMBTypeInfo.intra = true;
				return;
			}

			//Read: 0000 0 -> ILLEGAL
			ASSERT(0);
		}
		break;
	default:
		ASSERT(0);
	}
}

int8 CMPEG1VideoDecoder::ReadMotionVectorCode(CBitInputStream &refBitInput) const
{
	//2-B.4

#define SIGN(x) (refBitInput.Read(1) ? -x : x)

	if(refBitInput.Read(1))
	{
		//Read: 1
		return 0;
	}

	//Read: 0
	if(refBitInput.Read(1))
	{
		//Read: 01
		return SIGN(1);
	}

	//Read: 00
	if(refBitInput.Read(1))
	{
		//Read: 001
		return SIGN(2);
	}

	//Read: 000
	if(refBitInput.Read(1))
	{
		//Read: 0001
		return SIGN(3);
	}

	//Read: 0000
	if(refBitInput.Read(1))
	{
		//Read: 0000 1
		if(refBitInput.Read(1))
		{
			//Read: 0000 11
			return SIGN(4);
		}

		//Read: 0000 10
		if(refBitInput.Read(1))
		{
			//Read: 0000 101
			return SIGN(5);
		}

		//Read: 0000 100
		return SIGN(6);
	}

	//Read: 0000 0
	if(refBitInput.Read(1))
	{
		//Read: 0000 01
		if(refBitInput.Read(1))
		{
			//Read: 0000 011
			return SIGN(7);
		}

		//Read: 0000 010
		if(refBitInput.Read(1))
		{
			//Read: 0000 0101
			if(refBitInput.Read(1))
			{
				//Read: 0000 0101 1
				return SIGN(8);
			}

			//Read: 0000 0101 0
			return SIGN(9);
		}

		//Read: 0000 0100
		if(refBitInput.Read(1))
		{
			//Read: 0000 0100 1
			return SIGN(10);
		}

		//Read: 0000 0100 0
		if(refBitInput.Read(1))
		{
			//Read: 0000 0100 01
			return SIGN(11);
		}

		//Read: 0000 0100 00
		return SIGN(12);
	}

	//Read: 0000 00
	if(refBitInput.Read(1))
	{
		//Read: 0000 001
		if(refBitInput.Read(1))
		{
			//Read: 0000 0011
			if(refBitInput.Read(1))
			{
				//Read: 0000 0011 1
				if(refBitInput.Read(1))
				{
					//Read: 0000 0011 11
					return SIGN(13);
				}

				//Read: 0000 0011 10
				return SIGN(14);
			}

			//Read: 0000 0011 0
			if(refBitInput.Read(1))
			{
				//Read: 0000 0011 01
				return SIGN(15);
			}

			//Read: 0000 0011 00
			return SIGN(16);
		}

		//Read: 0000 0010
		ASSERT(0);
	}

	//Read: 0000 000
	ASSERT(0);

#undef SIGN

	return -1;
}
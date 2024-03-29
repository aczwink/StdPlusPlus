//Class header
#include "CMP3Decoder.h"

const SHuffmanTable &CMP3Decoder::GetHuffmanTable(uint8 index) const
{
	//IMPORTANT NOTE: keep entries sorted according to the length of the code word, for the decoding algorithm to work efficiently

	static const SHuffmanTableEntry entries16[256] =
	{
		{0, 0, 1, 1},
		{1, 0, 3, 3},
		{0, 1, 4, 5},
		{1, 1, 4, 4},
		{0, 2, 6, 14},
		{1, 2, 6, 12},
		{2, 0, 6, 15},
		{2, 1, 6, 13},
		{1, 3, 7, 20},
		{2, 2, 7, 23},
		{3, 1, 7, 21},
		{0, 3, 8, 44},
		{1, 4, 8, 35},
		{1, 15, 8, 9},
		{2, 3, 8, 38},
		{3, 0, 8, 45},
		{3, 2, 8, 39},
		{4, 1, 8, 36},
		{5, 1, 8, 30},
		{15, 1, 8, 10},
		{15, 2, 8, 7},
		{15, 15, 8, 3},
		{0, 4, 9, 74},
		{0, 5, 9, 63},
		{0, 15, 9, 17},
		{1, 5, 9, 62},
		{1, 6, 9, 53},
		{1, 7, 9, 47},
		{2, 4, 9, 67},
		{2, 5, 9, 58},
		{2, 15, 9, 16},
		{3, 3, 9, 69},
		{3, 4, 9, 64},
		{4, 0, 9, 75},
		{4, 2, 9, 68},
		{4, 3, 9, 65},
		{4, 15, 9, 9},
		{5, 0, 9, 66},
		{5, 2, 9, 59},
		{5, 3, 9, 56},
		{6, 1, 9, 54},
		{6, 2, 9, 52},
		{7, 1, 9, 48},
		{15, 0, 9, 12},
		{15, 3, 9, 11},
		{15, 4, 9, 10},
		{0, 6, 10, 110},
		{0, 7, 10, 93},
		{1, 8, 10, 83},
		{1, 9, 10, 75},
		{1, 10, 10, 68},
		{2, 6, 10, 103},
		{2, 7, 10, 90},
		{2, 9, 10, 72},
		{3, 5, 10, 114},
		{3, 6, 10, 99},
		{3, 7, 10, 87},
		{3, 15, 10, 26},
		{4, 4, 10, 115},
		{4, 5, 10, 101},
		{5, 4, 10, 102},
		{5, 15, 10, 16},
		{6, 0, 10, 111},
		{6, 3, 10, 100},
		{6, 15, 10, 10},
		{7, 0, 10, 98},
		{7, 2, 10, 91},
		{7, 3, 10, 88},
		{7, 15, 10, 8},
		{8, 0, 10, 85},
		{8, 1, 10, 84},
		{8, 2, 10, 81},
		{8, 15, 10, 7},
		{9, 1, 10, 76},
		{9, 2, 10, 73},
		{10, 2, 10, 67},
		{10, 15, 10, 4},
		{15, 5, 10, 17},
		{15, 6, 10, 11},
		{15, 7, 10, 9},
		{0, 8, 11, 172},
		{0, 9, 11, 149},
		{0, 10, 11, 138},
		{1, 11, 11, 119},
		{1, 13, 11, 107},
		{2, 8, 11, 161},
		{2, 10, 11, 127},
		{2, 11, 11, 117},
		{2, 12, 11, 110},
		{3, 8, 11, 158},
		{3, 9, 11, 140},
		{4, 6, 11, 179},
		{4, 7, 11, 164},
		{4, 8, 11, 155},
		{5, 5, 11, 185},
		{5, 6, 11, 173},
		{5, 8, 11, 142},
		{6, 4, 11, 184},
		{6, 5, 11, 178},
		{6, 6, 11, 160},
		{6, 7, 11, 133},
		{7, 4, 11, 165},
		{7, 5, 11, 157},
		{7, 6, 11, 148},
		{8, 3, 11, 159},
		{8, 4, 11, 156},
		{8, 5, 11, 143},
		{9, 0, 11, 154},
		{9, 3, 11, 141},
		{9, 4, 11, 131},
		{9, 15, 11, 11},
		{10, 0, 11, 139},
		{10, 1, 11, 129},
		{10, 3, 11, 125},
		{11, 1, 11, 120},
		{11, 2, 11, 118},
		{11, 3, 11, 115},
		{11, 15, 11, 6},
		{12, 15, 11, 4},
		{13, 15, 11, 2},
		{14, 2, 11, 102},
		{14, 15, 11, 0},
		{15, 8, 11, 13},
		{15, 9, 11, 12},
		{15, 10, 11, 10},
		{15, 11, 11, 7},
		{15, 12, 11, 5},
		{15, 13, 11, 3},
		{15, 14, 11, 1},
		{0, 11, 12, 242},
		{0, 12, 12, 225},
		{0, 13, 12, 195},
		{1, 12, 12, 201},
		{1, 14, 12, 207},
		{2, 13, 12, 209},
		{2, 14, 12, 206},
		{3, 10, 12, 252},
		{3, 11, 12, 212},
		{3, 12, 12, 199},
		{4, 9, 12, 264},
		{4, 10, 12, 246},
		{4, 11, 12, 226},
		{5, 7, 12, 265},
		{5, 9, 12, 253},
		{5, 10, 12, 232},
		{6, 8, 12, 257},
		{6, 9, 12, 244},
		{6, 10, 12, 228},
		{6, 11, 12, 217},
		{7, 7, 12, 261},
		{7, 8, 12, 248},
		{8, 6, 12, 260},
		{8, 7, 12, 249},
		{9, 5, 12, 256},
		{9, 6, 12, 245},
		{10, 4, 12, 247},
		{10, 5, 12, 233},
		{10, 6, 12, 229},
		{10, 7, 12, 219},
		{11, 0, 12, 243},
		{11, 4, 12, 227},
		{11, 5, 12, 223},
		{12, 0, 12, 202},
		{12, 1, 12, 224},
		{12, 2, 12, 222},
		{12, 3, 12, 218},
		{12, 4, 12, 216},
		{13, 1, 12, 211},
		{13, 2, 12, 210},
		{13, 3, 12, 208},
		{14, 3, 12, 187},
		{0, 14, 13, 376},
		{3, 13, 13, 387},
		{3, 14, 13, 365},
		{4, 12, 13, 395},
		{4, 13, 13, 382},
		{4, 14, 13, 362},
		{5, 11, 13, 400},
		{5, 12, 13, 388},
		{5, 13, 13, 378},
		{6, 12, 13, 385},
		{6, 13, 13, 366},
		{7, 9, 13, 407},
		{7, 10, 13, 397},
		{7, 11, 13, 372},
		{7, 12, 13, 380},
		{8, 8, 13, 427},
		{8, 9, 13, 401},
		{8, 10, 13, 392},
		{8, 11, 13, 383},
		{9, 7, 13, 426},
		{9, 8, 13, 406},
		{9, 9, 13, 394},
		{9, 10, 13, 384},
		{9, 12, 13, 359},
		{9, 14, 13, 352},
		{10, 8, 13, 393},
		{11, 6, 13, 396},
		{11, 13, 13, 223},
		{12, 5, 13, 389},
		{12, 6, 13, 386},
		{12, 7, 13, 381},
		{12, 8, 13, 364},
		{13, 4, 13, 370},
		{13, 5, 13, 379},
		{14, 0, 13, 377},
		{14, 1, 13, 369},
		{14, 6, 13, 358},
		{5, 14, 14, 445},
		{6, 14, 14, 715},
		{8, 12, 14, 727},
		{8, 13, 14, 713},
		{8, 14, 14, 708},
		{9, 11, 14, 735},
		{9, 13, 14, 710},
		{10, 9, 14, 743},
		{10, 10, 14, 737},
		{10, 11, 14, 720},
		{10, 14, 14, 439},
		{11, 7, 14, 746},
		{11, 8, 14, 742},
		{11, 9, 14, 736},
		{11, 10, 14, 721},
		{11, 11, 14, 712},
		{11, 12, 14, 706},
		{11, 14, 14, 436},
		{12, 10, 14, 443},
		{12, 11, 14, 707},
		{12, 12, 14, 440},
		{12, 13, 14, 437},
		{13, 0, 14, 747},
		{13, 6, 14, 734},
		{13, 7, 14, 723},
		{13, 8, 14, 714},
		{14, 4, 14, 726},
		{14, 5, 14, 722},
		{14, 7, 14, 711},
		{14, 8, 14, 709},
		{14, 14, 14, 434},
		{7, 13, 15, 889},
		{7, 14, 15, 884},
		{10, 12, 15, 885},
		{10, 13, 15, 882},
		{12, 9, 15, 888},
		{13, 10, 15, 883},
		{13, 11, 15, 877},
		{13, 12, 15, 876},
		{13, 14, 15, 865},
		{14, 9, 15, 866},
		{14, 11, 15, 871},
		{14, 13, 15, 870},
		{12, 14, 16, 1728},
		{13, 9, 16, 1735},
		{14, 10, 16, 1734},
		{13, 13, 17, 3459},
		{14, 12, 17, 3458},
	};

	static const SHuffmanTableEntry entries24[256] =
	{
		{0, 0, 4, 15},
		{0, 1, 4, 13},
		{1, 0, 4, 14},
		{1, 1, 4, 12},
		{15, 15, 4, 3},
		{1, 2, 5, 21},
		{2, 1, 5, 22},
		{0, 2, 6, 46},
		{1, 3, 6, 38},
		{2, 0, 6, 47},
		{2, 2, 6, 41},
		{3, 1, 6, 39},
		{0, 3, 7, 80},
		{1, 4, 7, 71},
		{2, 3, 7, 74},
		{2, 4, 7, 68},
		{2, 15, 7, 18},
		{3, 0, 7, 81},
		{3, 2, 7, 75},
		{3, 3, 7, 70},
		{3, 15, 7, 16},
		{4, 1, 7, 72},
		{4, 2, 7, 69},
		{4, 15, 7, 14},
		{5, 1, 7, 66},
		{5, 15, 7, 12},
		{6, 15, 7, 10},
		{15, 1, 7, 20},
		{15, 2, 7, 19},
		{15, 3, 7, 17},
		{15, 4, 7, 15},
		{15, 5, 7, 13},
		{15, 6, 7, 11},
		{15, 7, 7, 9},
		{15, 8, 7, 7},
		{15, 9, 7, 6},
		{15, 10, 7, 4},
		{0, 4, 8, 146},
		{1, 5, 8, 130},
		{1, 6, 8, 122},
		{1, 15, 8, 42},
		{2, 5, 8, 128},
		{2, 6, 8, 120},
		{3, 4, 8, 134},
		{3, 5, 8, 125},
		{3, 6, 8, 116},
		{4, 0, 8, 147},
		{4, 3, 8, 135},
		{4, 4, 8, 127},
		{4, 5, 8, 118},
		{4, 6, 8, 112},
		{5, 2, 8, 129},
		{5, 3, 8, 126},
		{5, 4, 8, 119},
		{5, 5, 8, 114},
		{6, 1, 8, 123},
		{6, 2, 8, 121},
		{6, 3, 8, 117},
		{6, 4, 8, 113},
		{7, 1, 8, 115},
		{7, 2, 8, 111},
		{7, 3, 8, 109},
		{7, 15, 8, 17},
		{8, 15, 8, 16},
		{9, 15, 8, 11},
		{10, 15, 8, 10},
		{11, 15, 8, 6},
		{12, 15, 8, 4},
		{13, 15, 8, 2},
		{14, 15, 8, 0},
		{15, 0, 8, 43},
		{15, 11, 8, 7},
		{15, 12, 8, 5},
		{15, 13, 8, 3},
		{15, 14, 8, 1},
		{0, 5, 9, 262},
		{0, 6, 9, 248},
		{0, 15, 9, 88},
		{1, 7, 9, 216},
		{1, 8, 9, 209},
		{1, 9, 9, 198},
		{2, 7, 9, 221},
		{2, 8, 9, 207},
		{2, 9, 9, 194},
		{2, 10, 9, 182},
		{3, 7, 9, 220},
		{3, 8, 9, 204},
		{3, 9, 9, 190},
		{3, 10, 9, 178},
		{4, 7, 9, 210},
		{4, 8, 9, 200},
		{4, 9, 9, 188},
		{5, 0, 9, 263},
		{5, 6, 9, 214},
		{5, 7, 9, 202},
		{5, 8, 9, 192},
		{5, 9, 9, 180},
		{6, 0, 9, 249},
		{6, 5, 9, 215},
		{6, 6, 9, 206},
		{6, 7, 9, 195},
		{6, 8, 9, 185},
		{7, 4, 9, 211},
		{7, 5, 9, 203},
		{7, 6, 9, 196},
		{7, 7, 9, 187},
		{8, 1, 9, 212},
		{8, 2, 9, 208},
		{8, 3, 9, 205},
		{8, 4, 9, 201},
		{8, 5, 9, 193},
		{8, 6, 9, 186},
		{8, 7, 9, 177},
		{8, 8, 9, 169},
		{9, 1, 9, 199},
		{9, 2, 9, 197},
		{9, 3, 9, 191},
		{9, 4, 9, 189},
		{9, 5, 9, 181},
		{9, 6, 9, 174},
		{10, 1, 9, 184},
		{10, 2, 9, 183},
		{10, 3, 9, 179},
		{10, 4, 9, 175},
		{11, 2, 9, 171},
		{11, 3, 9, 168},
		{11, 4, 9, 164},
		{0, 7, 10, 434},
		{0, 8, 10, 426},
		{1, 10, 10, 327},
		{1, 11, 10, 345},
		{1, 12, 10, 319},
		{1, 13, 10, 297},
		{1, 14, 10, 279},
		{2, 11, 10, 340},
		{2, 12, 10, 315},
		{2, 13, 10, 295},
		{3, 11, 10, 325},
		{3, 12, 10, 311},
		{3, 13, 10, 293},
		{3, 14, 10, 271},
		{4, 10, 10, 352},
		{4, 11, 10, 323},
		{4, 12, 10, 306},
		{4, 13, 10, 285},
		{5, 10, 10, 341},
		{5, 11, 10, 317},
		{5, 12, 10, 301},
		{5, 13, 10, 281},
		{5, 14, 10, 262},
		{6, 9, 10, 347},
		{6, 10, 10, 330},
		{6, 11, 10, 308},
		{6, 12, 10, 291},
		{6, 13, 10, 272},
		{7, 0, 10, 435},
		{7, 8, 10, 353},
		{7, 9, 10, 332},
		{7, 10, 10, 313},
		{7, 11, 10, 298},
		{7, 12, 10, 283},
		{8, 0, 10, 427},
		{8, 9, 10, 320},
		{8, 10, 10, 303},
		{8, 11, 10, 286},
		{8, 12, 10, 268},
		{9, 0, 10, 335},
		{9, 7, 10, 333},
		{9, 8, 10, 321},
		{9, 9, 10, 305},
		{9, 10, 10, 289},
		{9, 11, 10, 275},
		{10, 5, 10, 344},
		{10, 6, 10, 331},
		{10, 7, 10, 314},
		{10, 8, 10, 304},
		{10, 9, 10, 290},
		{10, 10, 10, 277},
		{11, 1, 10, 346},
		{11, 5, 10, 318},
		{11, 6, 10, 309},
		{11, 7, 10, 299},
		{11, 8, 10, 287},
		{11, 9, 10, 276},
		{11, 10, 10, 263},
		{12, 1, 10, 322},
		{12, 2, 10, 316},
		{12, 3, 10, 312},
		{12, 4, 10, 307},
		{12, 5, 10, 302},
		{12, 6, 10, 292},
		{12, 7, 10, 284},
		{12, 8, 10, 269},
		{12, 9, 10, 261},
		{13, 1, 10, 300},
		{13, 2, 10, 296},
		{13, 3, 10, 294},
		{13, 4, 10, 288},
		{13, 5, 10, 282},
		{13, 6, 10, 273},
		{13, 7, 10, 266},
		{14, 1, 10, 280},
		{14, 2, 10, 278},
		{14, 3, 10, 274},
		{14, 4, 10, 267},
		{14, 5, 10, 264},
		{14, 6, 10, 259},
		{0, 9, 11, 669},
		{0, 10, 11, 653},
		{0, 11, 11, 649},
		{0, 12, 11, 621},
		{0, 13, 11, 517},
		{2, 14, 11, 541},
		{4, 14, 11, 540},
		{6, 14, 11, 520},
		{7, 13, 11, 531},
		{7, 14, 11, 381},
		{8, 13, 11, 514},
		{8, 14, 11, 377},
		{9, 12, 11, 521},
		{9, 13, 11, 379},
		{9, 14, 11, 371},
		{10, 0, 11, 668},
		{10, 11, 11, 530},
		{10, 12, 11, 383},
		{10, 13, 11, 373},
		{10, 14, 11, 366},
		{11, 0, 11, 652},
		{11, 11, 11, 513},
		{11, 12, 11, 375},
		{11, 13, 11, 368},
		{11, 14, 11, 362},
		{12, 0, 11, 648},
		{12, 10, 11, 512},
		{12, 11, 11, 376},
		{12, 12, 11, 370},
		{12, 13, 11, 364},
		{12, 14, 11, 359},
		{13, 0, 11, 620},
		{13, 8, 11, 515},
		{13, 9, 11, 380},
		{13, 10, 11, 374},
		{13, 11, 11, 369},
		{13, 12, 11, 365},
		{13, 13, 11, 361},
		{13, 14, 11, 357},
		{14, 7, 11, 382},
		{14, 8, 11, 378},
		{14, 9, 11, 372},
		{14, 10, 11, 367},
		{14, 11, 11, 363},
		{14, 12, 11, 360},
		{14, 13, 11, 358},
		{14, 14, 11, 356},
		{0, 14, 12, 1032},
		{14, 0, 12, 1033},
	};

	switch(index)
	{
	case 0:
		{
			static const SHuffmanTableEntry entries0[1] =
			{
				{0, 0, 0, 0}
			};

			static const SHuffmanTable table0 = {0, sizeof(entries0) / sizeof(entries0[0]), entries0};

			return table0;
		}
		break;
	case 1:
		{
			static const SHuffmanTableEntry entries1[4] =
			{
				{0, 0, 1, 1},
				{1, 0, 2, 1},
				{0, 1, 3, 1},
				{1, 1, 3, 0},
			};

			static const SHuffmanTable table1 = {0, sizeof(entries1) / sizeof(entries1[0]), entries1};

			return table1;
		}
		break;
	case 2:
		{
			static const SHuffmanTableEntry entries2[9] =
			{
				{0, 0, 1, 1},
				{0, 1, 3, 2},
				{1, 0, 3, 3},
				{1, 1, 3, 1},
				{1, 2, 5, 1},
				{2, 0, 5, 3},
				{2, 1, 5, 2},
				{0, 2, 6, 1},
				{2, 2, 6, 0},
			};

			static const SHuffmanTable table2 = {0, sizeof(entries2) / sizeof(entries2[0]), entries2};

			return table2;
		}
		break;
	case 3:
		{
			static const SHuffmanTableEntry entries3[9] =
			{
				//length 2
				{0, 0, 2, 3},
				{0, 1, 2, 2},
				{1, 1, 2, 1},
				//length 3
				{1, 0, 3, 1},
				//length 5
				{1, 2, 5, 1},
				{2, 0, 5, 3},
				{2, 1, 5, 2},
				//length 6
				{0, 2, 6, 1},
				{2, 2, 6, 0},
			};

			static const SHuffmanTable table3 = {0, sizeof(entries3) / sizeof(entries3[0]), entries3};

			return table3;
		}
		break;
	case 5:
		{
			static const SHuffmanTableEntry entries5[16] =
			{
				{0, 0, 1, 1},
				{0, 1, 3, 2},
				{1, 0, 3, 3},
				{1, 1, 3, 1},
				{0, 2, 6, 6},
				{1, 2, 6, 4},
				{2, 0, 6, 7},
				{2, 1, 6, 5},
				{3, 1, 6, 1},
				{0, 3, 7, 5},
				{1, 3, 7, 4},
				{2, 2, 7, 7},
				{3, 0, 7, 6},
				{3, 2, 7, 1},
				{2, 3, 8, 1},
				{3, 3, 8, 0},
			};

			static const SHuffmanTable table5 = {0, sizeof(entries5) / sizeof(entries5[0]), entries5};

			return table5;
		}
		break;
	case 6:
		{
			static const SHuffmanTableEntry entries6[16] =
			{
				//length 2
				{1, 1, 2, 2},
				//length 3
				{0, 0, 3, 7},
				{0, 1, 3, 3},
				{1, 0, 3, 6},
				//length 4
				{1, 2, 4, 3},
				{2, 0, 4, 5},
				{2, 1, 4, 4},
				//length 5
				{0, 2, 5, 5},
				{1, 3, 5, 2},
				{2, 2, 5, 4},
				{3, 1, 5, 3},
				//length 6
				{2, 3, 6, 1},
				{3, 0, 6, 3},
				{3, 2, 6, 2},
				//length 7
				{0, 3, 7, 1},
				{3, 3, 7, 0},
			};

			static const SHuffmanTable table6 = {0, sizeof(entries6) / sizeof(entries6[0]), entries6};

			return table6;
		}
		break;
	case 7:
		{
			static const SHuffmanTableEntry entries7[] =
			{
				{0, 0, 1, 1},
				{0, 1, 3, 2},
				{1, 0, 3, 3},
				{1, 1, 4, 3},
				{2, 1, 5, 4},
				{0, 2, 6, 10},
				{1, 2, 6, 7},
				{2, 0, 6, 11},
				{1, 3, 7, 10},
				{1, 4, 7, 5},
				{2, 2, 7, 13},
				{3, 0, 7, 12},
				{3, 1, 7, 11},
				{4, 0, 7, 7},
				{4, 1, 7, 6},
				{0, 3, 8, 19},
				{0, 4, 8, 16},
				{1, 5, 8, 3},
				{2, 3, 8, 17},
				{2, 4, 8, 8},
				{3, 2, 8, 18},
				{4, 2, 8, 9},
				{5, 0, 8, 6},
				{5, 1, 8, 4},
				{0, 5, 9, 10},
				{2, 5, 9, 4},
				{3, 3, 9, 15},
				{3, 4, 9, 11},
				{3, 5, 9, 2},
				{4, 3, 9, 14},
				{4, 4, 9, 3},
				{5, 2, 9, 5},
				{4, 5, 10, 1},
				{5, 3, 10, 3},
				{5, 4, 10, 2},
				{5, 5, 10, 0},
			};

			static const SHuffmanTable table7 = {0, sizeof(entries7) / sizeof(entries7[0]), entries7};

			return table7;
		}
		break;
	case 8:
		{
			static const SHuffmanTableEntry entries8[36] =
			{
				{0, 0, 2, 3},
				{1, 1, 2, 1},
				{0, 1, 3, 4},
				{1, 0, 3, 5},
				{1, 2, 4, 2},
				{2, 1, 4, 3},
				{0, 2, 6, 6},
				{2, 0, 6, 7},
				{2, 2, 6, 5},
				{4, 1, 7, 5},
				{0, 3, 8, 18},
				{0, 4, 8, 12},
				{1, 3, 8, 16},
				{1, 4, 8, 9},
				{1, 5, 8, 3},
				{2, 3, 8, 14},
				{2, 4, 8, 7},
				{3, 0, 8, 19},
				{3, 1, 8, 17},
				{3, 2, 8, 15},
				{4, 0, 8, 13},
				{4, 2, 8, 8},
				{5, 1, 8, 4},
				{0, 5, 9, 5},
				{2, 5, 9, 3},
				{3, 3, 9, 13},
				{3, 4, 9, 10},
				{4, 3, 9, 11},
				{5, 0, 9, 12},
				{5, 2, 9, 4},
				{5, 3, 9, 1},
				{3, 5, 10, 4},
				{4, 4, 10, 5},
				{4, 5, 10, 1},
				{5, 4, 11, 1},
				{5, 5, 11, 0},
			};

			static const SHuffmanTable table8 = {0, sizeof(entries8) / sizeof(entries8[0]), entries8};

			return table8;
		}
		break;
	case 9:
		{
			static const SHuffmanTableEntry entries9[36] =
			{
				//length 3
				{0, 0, 3, 7},
				{0, 1, 3, 5},
				{1, 0, 3, 6},
				{1, 1, 3, 4},
				//length 4
				{1, 2, 4, 5},
				{2, 0, 4, 7},
				{2, 1, 4, 6},
				//length 5
				{0, 2, 5, 9},
				{1, 3, 5, 5},
				{2, 2, 5, 8},
				{3, 1, 5, 6},
				//length 6
				{0, 3, 6, 0xE},
				{1, 4, 6, 6},
				{2, 3, 6, 8},
				{3, 0, 6, 0xF},
				{3, 2, 6, 9},
				{4, 1, 6, 7},
				//length 7
				{2, 4, 7, 8},
				{3, 3, 7, 0xA},
				{3, 4, 7, 5},
				{4, 0, 7, 0xB},
				{4, 2, 7, 9},
				{4, 3, 7, 6},
				{5, 1, 7, 4},
				//length 8
				{0, 4, 8, 0xF},
				{1, 5, 8, 7},
				{2, 5, 8, 5},
				{3, 5, 8, 1},
				{4, 4, 8, 4},
				{5, 0, 8, 0xE},
				{5, 2, 8, 6},
				{5, 3, 8, 2},
				//length 9
				{0, 5, 9, 7},
				{4, 5, 9, 1},
				{5, 4, 9, 6},
				{5, 5, 9, 0},
			};

			static const SHuffmanTable table9 = {0, sizeof(entries9) / sizeof(entries9[0]), entries9};

			return table9;
		}
		break;
	case 10:
		{
			static const SHuffmanTableEntry entries10[64] =
			{
				{0, 0, 1, 1},
				{0, 1, 3, 2},
				{1, 0, 3, 3},
				{1, 1, 4, 3},
				{0, 2, 6, 10},
				{1, 2, 6, 8},
				{2, 0, 6, 11},
				{2, 1, 6, 9},
				{1, 3, 7, 12},
				{2, 2, 7, 15},
				{3, 0, 7, 14},
				{3, 1, 7, 13},
				{0, 3, 8, 23},
				{1, 4, 8, 18},
				{1, 6, 8, 12},
				{1, 7, 8, 7},
				{2, 3, 8, 21},
				{3, 2, 8, 22},
				{4, 0, 8, 20},
				{4, 1, 8, 19},
				{6, 0, 8, 14},
				{6, 1, 8, 13},
				{7, 1, 8, 8},
				{0, 4, 9, 35},
				{0, 5, 9, 30},
				{0, 6, 9, 12},
				{1, 5, 9, 21},
				{2, 4, 9, 32},
				{2, 6, 9, 19},
				{2, 7, 9, 6},
				{3, 3, 9, 34},
				{3, 6, 9, 18},
				{4, 2, 9, 33},
				{5, 0, 9, 31},
				{5, 1, 9, 22},
				{6, 2, 9, 10},
				{7, 0, 9, 9},
				{7, 2, 9, 7},
				{0, 7, 10, 17},
				{2, 5, 10, 40},
				{3, 4, 10, 46},
				{3, 5, 10, 23},
				{3, 7, 10, 7},
				{4, 3, 10, 47},
				{4, 4, 10, 27},
				{4, 5, 10, 22},
				{4, 6, 10, 9},
				{4, 7, 10, 3},
				{5, 2, 10, 41},
				{5, 3, 10, 26},
				{5, 6, 10, 5},
				{6, 3, 10, 11},
				{6, 4, 10, 16},
				{6, 5, 10, 6},
				{7, 3, 10, 8},
				{7, 4, 10, 4},
				{5, 4, 11, 21},
				{5, 5, 11, 20},
				{5, 7, 11, 3},
				{6, 6, 11, 5},
				{6, 7, 11, 1},
				{7, 5, 11, 4},
				{7, 6, 11, 2},
				{7, 7, 11, 0},
			};

			static const SHuffmanTable table10 = {0, sizeof(entries10) / sizeof(entries10[0]), entries10};

			return table10;
		}
		break;
	case 11:
		{
			static const SHuffmanTableEntry entries11[64] =
			{
				//length 2
				{0, 0, 2, 3},
				//length 3
				{0, 1, 3, 4},
				{1, 0, 3, 5},
				{1, 1, 3, 3},
				//length 4
				{1, 2, 4, 4},
				//length 5
				{0, 2, 5, 10},
				{2, 0, 5, 11},
				{2, 1, 5, 7},
				//length 6
				{1, 3, 6, 10},
				{2, 2, 6, 13},
				{3, 1, 6, 11},
				//length 7
				{0, 3, 7, 24},
				{1, 6, 7, 11},
				{2, 3, 7, 18},
				{3, 0, 7, 25},
				{3, 2, 7, 19},
				{6, 1, 7, 12},
				{6, 2, 7, 9},
				{7, 1, 7, 4},
				//length 8
				{0, 4, 8, 34},
				{0, 6, 8, 21},
				{1, 4, 8, 32},
				{1, 5, 8, 17},
				{1, 7, 8, 10},
				{2, 4, 8, 30},
				{2, 6, 8, 20},
				{2, 7, 8, 5},
				{3, 4, 8, 27},
				{3, 6, 8, 12},
				{4, 0, 8, 35},
				{4, 1, 8, 33},
				{4, 2, 8, 31},
				{5, 0, 8, 28},
				{5, 1, 8, 26},
				{6, 0, 8, 14},
				{6, 3, 8, 13},
				{7, 0, 8, 11},
				{7, 2, 8, 6},
				//length 9
				{0, 5, 9, 33},
				{0, 7, 9, 15},
				{2, 5, 9, 31},
				{3, 3, 9, 59},
				{3, 7, 9, 5},
				{4, 3, 9, 58},
				{4, 4, 9, 30},
				{4, 6, 9, 7},
				{5, 2, 9, 32},
				{6, 4, 9, 14},
				{7, 3, 9, 6},
				//length 10
				{3, 5, 10, 18},
				{4, 5, 10, 16},
				{4, 7, 10, 5},
				{5, 3, 10, 19},
				{5, 4, 10, 17},
				{5, 6, 10, 8},
				{6, 5, 10, 9},
				{6, 6, 10, 4},
				{6, 7, 10, 1},
				{7, 4, 10, 6},
				{7, 5, 10, 3},
				{7, 6, 10, 2},
				{7, 7, 10, 0},
				//length 11
				{5, 5, 11, 15},
				{5, 7, 11, 14},
			};

			static const SHuffmanTable table11 = {0, sizeof(entries11) / sizeof(entries11[0]), entries11};

			return table11;
		}
		break;
	case 12:
		{
			static const SHuffmanTableEntry entries12[64] =
			{
				//length 3
				{0, 1, 3, 6},
				{1, 0, 3, 7},
				{1, 1, 3, 5},
				//length 4
				{0, 0, 4, 9},
				{1, 2, 4, 6},
				{2, 1, 4, 7},
				//length 5
				{0, 2, 5, 0x10},
				{1, 3, 5, 9},
				{2, 0, 5, 0x11},
				{2, 2, 5, 0xB},
				{3, 1, 5, 10},
				//length 6
				{2, 3, 6, 0xE},
				{3, 0, 6, 17},
				{3, 2, 6, 15},
				{3, 3, 6, 12},
				{4, 1, 6, 13},
				//length 7
				{0, 3, 7, 0x21},
				{1, 4, 7, 0x17},
				{1, 5, 7, 0x10},
				{2, 4, 7, 0x15},
				{2, 6, 7, 10},
				{3, 4, 7, 18},
				{4, 0, 7, 32},
				{4, 2, 7, 22},
				{4, 3, 7, 19},
				{5, 1, 7, 17},
				{6, 1, 7, 12},
				{6, 2, 7, 11},
				//length 8
				{0, 4, 8, 0x29},
				{1, 6, 8, 0x1A},
				{1, 7, 8, 0xB},
				{2, 5, 8, 30},
				{2, 7, 8, 7},
				{3, 5, 8, 28},
				{3, 6, 8, 14},
				{3, 7, 8, 5},
				{4, 4, 8, 18},
				{4, 5, 8, 16},
				{4, 6, 8, 9},
				{5, 0, 8, 40},
				{5, 2, 8, 31},
				{5, 3, 8, 29},
				{5, 4, 8, 17},
				{5, 6, 8, 4},
				{6, 0, 8, 27},
				{6, 3, 8, 15},
				{6, 4, 8, 10},
				{7, 1, 8, 12},
				{7, 2, 8, 8},
				//length 9
				{0, 5, 9, 0x27},
				{0, 6, 9, 0x26},
				{0, 7, 9, 0x1A},
				{4, 7, 9, 5},
				{5, 5, 9, 13},
				{5, 7, 9, 2},
				{6, 5, 9, 7},
				{6, 6, 9, 4},
				{7, 0, 9, 27},
				{7, 3, 9, 12},
				{7, 4, 9, 6},
				{7, 5, 9, 3},
				{7, 6, 9, 1},
				//length 10
				{6, 7, 10, 1},
				{7, 7, 10, 0},
			};

			static const SHuffmanTable table12 = {0, sizeof(entries12) / sizeof(entries12[0]), entries12};

			return table12;
		}
		break;
	case 13:
		{
			static const SHuffmanTableEntry entries13[256] =
			{
				{0, 0, 1, 1},
				{1, 0, 3, 3},
				{0, 1, 4, 5},
				{1, 1, 4, 4},
				{0, 2, 6, 14},
				{1, 2, 6, 12},
				{2, 0, 6, 15},
				{2, 1, 6, 13},
				{0, 3, 7, 21},
				{1, 3, 7, 19},
				{2, 2, 7, 23},
				{3, 0, 7, 22},
				{3, 1, 7, 20},
				{4, 1, 7, 16},
				{0, 4, 8, 34},
				{1, 4, 8, 31},
				{1, 5, 8, 26},
				{2, 3, 8, 36},
				{3, 2, 8, 37},
				{4, 0, 8, 35},
				{5, 1, 8, 27},
				{8, 1, 8, 20},
				{0, 5, 9, 51},
				{0, 6, 9, 46},
				{0, 8, 9, 42},
				{1, 6, 9, 44},
				{1, 7, 9, 33},
				{1, 8, 9, 31},
				{1, 9, 9, 24},
				{2, 4, 9, 59},
				{2, 5, 9, 49},
				{2, 8, 9, 29},
				{3, 3, 9, 61},
				{3, 4, 9, 56},
				{4, 2, 9, 60},
				{4, 3, 9, 57},
				{5, 0, 9, 58},
				{5, 2, 9, 50},
				{6, 0, 9, 47},
				{6, 1, 9, 45},
				{7, 1, 9, 34},
				{8, 0, 9, 43},
				{8, 2, 9, 30},
				{9, 1, 9, 25},
				{0, 7, 10, 71},
				{0, 9, 10, 52},
				{1, 10, 10, 32},
				{1, 11, 10, 24},
				{2, 6, 10, 77},
				{2, 7, 10, 65},
				{2, 9, 10, 40},
				{2, 10, 10, 30},
				{3, 5, 10, 79},
				{3, 6, 10, 73},
				{3, 7, 10, 64},
				{3, 8, 10, 43},
				{4, 4, 10, 97},
				{4, 5, 10, 75},
				{4, 8, 10, 54},
				{5, 3, 10, 96},
				{5, 4, 10, 76},
				{5, 5, 10, 70},
				{6, 2, 10, 78},
				{6, 3, 10, 74},
				{7, 0, 10, 72},
				{7, 2, 10, 56},
				{8, 3, 10, 44},
				{8, 4, 10, 55},
				{9, 0, 10, 53},
				{9, 2, 10, 41},
				{9, 3, 10, 37},
				{10, 0, 10, 35},
				{10, 1, 10, 33},
				{10, 2, 10, 31},
				{11, 1, 10, 25},
				{11, 2, 10, 23},
				{0, 10, 11, 68},
				{0, 11, 11, 52},
				{1, 12, 11, 31},
				{2, 11, 11, 40},
				{2, 12, 11, 27},
				{3, 9, 11, 76},
				{3, 10, 11, 56},
				{3, 11, 11, 37},
				{3, 12, 11, 26},
				{4, 6, 11, 114},
				{4, 7, 11, 91},
				{4, 9, 11, 73},
				{4, 10, 11, 55},
				{5, 6, 11, 93},
				{5, 7, 11, 84},
				{5, 8, 11, 77},
				{5, 9, 11, 58},
				{5, 11, 11, 29},
				{6, 4, 11, 115},
				{6, 5, 11, 94},
				{6, 6, 11, 90},
				{6, 7, 11, 79},
				{6, 8, 11, 69},
				{7, 3, 11, 95},
				{7, 4, 11, 92},
				{7, 5, 11, 85},
				{8, 5, 11, 78},
				{8, 6, 11, 72},
				{9, 4, 11, 44},
				{9, 5, 11, 59},
				{9, 6, 11, 54},
				{10, 3, 11, 57},
				{10, 4, 11, 42},
				{11, 0, 11, 53},
				{11, 3, 11, 38},
				{12, 0, 11, 34},
				{12, 1, 11, 32},
				{12, 2, 11, 28},
				{13, 1, 11, 21},
				{0, 12, 12, 67},
				{0, 13, 12, 44},
				{1, 13, 12, 35},
				{1, 14, 12, 22},
				{1, 15, 12, 14},
				{2, 13, 12, 33},
				{3, 13, 12, 31},
				{4, 11, 12, 41},
				{4, 12, 12, 48},
				{5, 10, 12, 79},
				{6, 9, 12, 83},
				{6, 10, 12, 71},
				{6, 11, 12, 50},
				{7, 6, 12, 91},
				{7, 7, 12, 90},
				{7, 8, 12, 86},
				{7, 9, 12, 73},
				{8, 7, 12, 87},
				{8, 8, 12, 78},
				{8, 9, 12, 61},
				{8, 10, 12, 46},
				{9, 8, 12, 66},
				{10, 5, 12, 82},
				{10, 6, 12, 72},
				{10, 8, 12, 47},
				{11, 4, 12, 70},
				{11, 5, 12, 60},
				{11, 6, 12, 51},
				{11, 7, 12, 36},
				{12, 3, 12, 39},
				{12, 4, 12, 49},
				{12, 6, 12, 30},
				{13, 0, 12, 45},
				{13, 2, 12, 34},
				{14, 1, 12, 23},
				{14, 2, 12, 20},
				{15, 0, 12, 16},
				{15, 1, 12, 15},
				{0, 14, 13, 43},
				{0, 15, 13, 19},
				{2, 14, 13, 42},
				{2, 15, 13, 16},
				{3, 14, 13, 25},
				{3, 15, 13, 14},
				{4, 13, 13, 53},
				{4, 14, 13, 23},
				{5, 12, 13, 74},
				{5, 13, 13, 49},
				{6, 12, 13, 59},
				{6, 13, 13, 38},
				{7, 10, 13, 77},
				{7, 11, 13, 65},
				{7, 12, 13, 51},
				{8, 11, 13, 54},
				{8, 12, 13, 37},
				{9, 7, 13, 81},
				{9, 9, 13, 76},
				{9, 10, 13, 57},
				{10, 7, 13, 80},
				{10, 9, 13, 58},
				{10, 11, 13, 21},
				{11, 8, 13, 55},
				{11, 9, 13, 26},
				{11, 10, 13, 34},
				{12, 5, 13, 75},
				{12, 7, 13, 52},
				{13, 3, 13, 64},
				{13, 4, 13, 56},
				{13, 5, 13, 50},
				{14, 0, 13, 48},
				{14, 3, 13, 39},
				{14, 4, 13, 36},
				{14, 5, 13, 35},
				{15, 2, 13, 17},
				{4, 15, 14, 24},
				{5, 14, 14, 41},
				{5, 15, 14, 17},
				{6, 14, 14, 36},
				{6, 15, 14, 15},
				{7, 13, 14, 44},
				{8, 13, 14, 30},
				{9, 11, 14, 54},
				{9, 12, 14, 37},
				{9, 13, 14, 18},
				{10, 10, 14, 55},
				{10, 12, 14, 22},
				{11, 11, 14, 23},
				{12, 8, 14, 48},
				{12, 9, 14, 40},
				{13, 6, 14, 49},
				{13, 7, 14, 45},
				{13, 8, 14, 31},
				{13, 9, 14, 19},
				{13, 10, 14, 12},
				{14, 7, 14, 21},
				{14, 8, 14, 16},
				{15, 3, 14, 27},
				{15, 4, 14, 25},
				{15, 5, 14, 20},
				{15, 7, 14, 11},
				{8, 14, 15, 20},
				{8, 15, 15, 16},
				{9, 15, 15, 11},
				{10, 13, 15, 26},
				{11, 12, 15, 27},
				{11, 13, 15, 14},
				{11, 14, 15, 9},
				{12, 10, 15, 52},
				{12, 11, 15, 28},
				{12, 12, 15, 18},
				{13, 11, 15, 15},
				{13, 13, 15, 7},
				{14, 6, 15, 53},
				{14, 10, 15, 13},
				{14, 11, 15, 10},
				{14, 12, 15, 6},
				{15, 6, 15, 29},
				{15, 8, 15, 17},
				{15, 9, 15, 12},
				{7, 14, 16, 43},
				{7, 15, 16, 42},
				{9, 14, 16, 39},
				{10, 14, 16, 38},
				{11, 15, 16, 7},
				{12, 13, 16, 17},
				{12, 14, 16, 9},
				{12, 15, 16, 5},
				{13, 12, 16, 10},
				{13, 14, 16, 6},
				{13, 15, 16, 3},
				{14, 14, 16, 4},
				{14, 15, 16, 2},
				{15, 10, 16, 16},
				{15, 11, 16, 8},
				{15, 15, 16, 1},
				{10, 15, 17, 22},
				{14, 9, 17, 23},
				{14, 13, 17, 1},
				{15, 13, 18, 1},
				{15, 12, 19, 1},
				{15, 14, 19, 0},
			};

			static const SHuffmanTable table13 = {0, sizeof(entries13) / sizeof(entries13[0]), entries13};

			return table13;
		}
		break;
	case 15:
		{
			static const SHuffmanTableEntry entries15[256] =
			{
				{0, 0, 3, 7},
				{1, 1, 3, 5},
				{0, 1, 4, 12},
				{1, 0, 4, 13},
				{0, 2, 5, 18},
				{1, 2, 5, 16},
				{2, 0, 5, 19},
				{2, 1, 5, 17},
				{2, 2, 5, 15},
				{1, 3, 6, 27},
				{2, 3, 6, 24},
				{3, 0, 6, 29},
				{3, 1, 6, 28},
				{3, 2, 6, 25},
				{4, 1, 6, 22},
				{0, 3, 7, 53},
				{0, 4, 7, 47},
				{1, 4, 7, 46},
				{1, 5, 7, 36},
				{2, 4, 7, 41},
				{2, 5, 7, 34},
				{3, 3, 7, 43},
				{3, 4, 7, 39},
				{4, 0, 7, 52},
				{4, 2, 7, 42},
				{4, 3, 7, 40},
				{5, 1, 7, 37},
				{5, 2, 7, 35},
				{6, 1, 7, 32},
				{0, 5, 8, 76},
				{1, 6, 8, 61},
				{1, 7, 8, 51},
				{1, 8, 8, 42},
				{2, 6, 8, 59},
				{2, 7, 8, 48},
				{2, 8, 8, 40},
				{3, 5, 8, 63},
				{3, 6, 8, 55},
				{4, 4, 8, 67},
				{4, 5, 8, 57},
				{5, 0, 8, 77},
				{5, 3, 8, 66},
				{5, 4, 8, 58},
				{5, 5, 8, 52},
				{6, 2, 8, 60},
				{6, 3, 8, 56},
				{6, 4, 8, 50},
				{7, 1, 8, 53},
				{7, 2, 8, 49},
				{8, 1, 8, 43},
				{8, 2, 8, 41},
				{9, 1, 8, 34},
				{0, 6, 9, 124},
				{0, 7, 9, 108},
				{0, 8, 9, 89},
				{1, 9, 9, 70},
				{1, 10, 9, 52},
				{2, 9, 9, 64},
				{2, 10, 9, 50},
				{3, 7, 9, 93},
				{3, 8, 9, 76},
				{3, 9, 9, 59},
				{4, 6, 9, 95},
				{4, 7, 9, 79},
				{4, 8, 9, 72},
				{4, 9, 9, 57},
				{5, 6, 9, 91},
				{5, 7, 9, 74},
				{5, 8, 9, 62},
				{5, 9, 9, 48},
				{6, 0, 9, 125},
				{6, 5, 9, 92},
				{6, 6, 9, 78},
				{6, 7, 9, 65},
				{6, 8, 9, 55},
				{7, 0, 9, 109},
				{7, 3, 9, 94},
				{7, 4, 9, 88},
				{7, 5, 9, 75},
				{7, 6, 9, 66},
				{8, 0, 9, 90},
				{8, 3, 9, 77},
				{8, 4, 9, 73},
				{8, 5, 9, 63},
				{8, 6, 9, 56},
				{9, 0, 9, 71},
				{9, 2, 9, 67},
				{9, 3, 9, 60},
				{9, 4, 9, 58},
				{9, 5, 9, 49},
				{10, 1, 9, 53},
				{10, 2, 9, 51},
				{10, 3, 9, 47},
				{11, 1, 9, 42},
				{11, 2, 9, 40},
				{11, 3, 9, 37},
				{12, 2, 9, 30},
				{0, 9, 10, 123},
				{0, 10, 10, 108},
				{1, 11, 10, 83},
				{1, 12, 10, 65},
				{1, 13, 10, 41},
				{2, 11, 10, 78},
				{2, 12, 10, 62},
				{3, 10, 10, 93},
				{3, 11, 10, 72},
				{3, 12, 10, 54},
				{4, 10, 10, 89},
				{4, 11, 10, 69},
				{4, 12, 10, 49},
				{5, 10, 10, 79},
				{5, 11, 10, 63},
				{6, 9, 10, 87},
				{6, 10, 10, 71},
				{6, 11, 10, 51},
				{7, 7, 10, 122},
				{7, 8, 10, 91},
				{7, 9, 10, 73},
				{7, 10, 10, 56},
				{7, 11, 10, 42},
				{8, 7, 10, 92},
				{8, 8, 10, 77},
				{8, 9, 10, 66},
				{8, 10, 10, 47},
				{9, 6, 10, 88},
				{9, 7, 10, 76},
				{9, 8, 10, 67},
				{10, 0, 10, 109},
				{10, 4, 10, 90},
				{10, 5, 10, 82},
				{10, 6, 10, 58},
				{10, 7, 10, 57},
				{10, 8, 10, 48},
				{11, 0, 10, 86},
				{11, 4, 10, 70},
				{11, 5, 10, 64},
				{11, 6, 10, 52},
				{11, 7, 10, 43},
				{12, 1, 10, 68},
				{12, 3, 10, 55},
				{12, 4, 10, 50},
				{12, 5, 10, 46},
				{13, 1, 10, 44},
				{13, 2, 10, 39},
				{13, 3, 10, 38},
				{13, 4, 10, 34},
				{0, 11, 11, 119},
				{0, 12, 11, 107},
				{0, 13, 11, 81},
				{1, 14, 11, 59},
				{1, 15, 11, 36},
				{2, 13, 11, 80},
				{2, 14, 11, 56},
				{2, 15, 11, 33},
				{3, 13, 11, 75},
				{3, 14, 11, 50},
				{3, 15, 11, 29},
				{4, 13, 11, 66},
				{4, 14, 11, 46},
				{4, 15, 11, 27},
				{5, 12, 11, 90},
				{5, 13, 11, 62},
				{5, 14, 11, 40},
				{6, 12, 11, 73},
				{6, 13, 11, 51},
				{7, 12, 11, 64},
				{7, 13, 11, 44},
				{7, 14, 11, 21},
				{8, 11, 11, 67},
				{8, 12, 11, 48},
				{9, 9, 11, 106},
				{9, 10, 11, 71},
				{9, 11, 11, 54},
				{9, 12, 11, 38},
				{10, 9, 11, 72},
				{10, 10, 11, 57},
				{10, 11, 11, 41},
				{10, 12, 11, 23},
				{11, 8, 11, 70},
				{11, 9, 11, 55},
				{11, 10, 11, 42},
				{11, 11, 11, 25},
				{12, 0, 11, 118},
				{12, 6, 11, 74},
				{12, 7, 11, 65},
				{12, 8, 11, 49},
				{12, 9, 11, 39},
				{12, 10, 11, 24},
				{12, 11, 11, 16},
				{13, 0, 11, 91},
				{13, 5, 11, 63},
				{13, 6, 11, 52},
				{13, 7, 11, 45},
				{13, 8, 11, 31},
				{14, 1, 11, 60},
				{14, 2, 11, 58},
				{14, 3, 11, 53},
				{14, 4, 11, 47},
				{14, 5, 11, 43},
				{14, 6, 11, 32},
				{14, 7, 11, 22},
				{15, 1, 11, 37},
				{15, 2, 11, 34},
				{15, 3, 11, 30},
				{15, 4, 11, 28},
				{15, 5, 11, 20},
				{15, 6, 11, 17},
				{0, 14, 12, 122},
				{5, 15, 12, 38},
				{6, 14, 12, 70},
				{6, 15, 12, 30},
				{7, 15, 12, 25},
				{8, 13, 12, 53},
				{8, 14, 12, 36},
				{8, 15, 12, 20},
				{9, 13, 12, 39},
				{9, 14, 12, 23},
				{9, 15, 12, 15},
				{10, 13, 12, 27},
				{10, 15, 12, 9},
				{11, 12, 12, 29},
				{11, 13, 12, 18},
				{11, 14, 12, 11},
				{12, 12, 12, 22},
				{12, 13, 12, 13},
				{13, 9, 12, 52},
				{13, 10, 12, 28},
				{13, 11, 12, 19},
				{13, 12, 12, 14},
				{13, 13, 12, 8},
				{14, 0, 12, 123},
				{14, 8, 12, 37},
				{14, 9, 12, 24},
				{14, 10, 12, 17},
				{14, 11, 12, 12},
				{14, 14, 12, 2},
				{15, 0, 12, 71},
				{15, 7, 12, 26},
				{15, 8, 12, 21},
				{15, 9, 12, 16},
				{15, 10, 12, 10},
				{15, 11, 12, 6},
				{0, 15, 13, 63},
				{10, 14, 13, 62},
				{11, 15, 13, 11},
				{12, 14, 13, 14},
				{12, 15, 13, 7},
				{13, 14, 13, 9},
				{13, 15, 13, 3},
				{14, 12, 13, 15},
				{14, 13, 13, 10},
				{14, 15, 13, 1},
				{15, 12, 13, 8},
				{15, 13, 13, 6},
				{15, 14, 13, 2},
				{15, 15, 13, 0},
			};

			static const SHuffmanTable table15 = {0, sizeof(entries15) / sizeof(entries15[0]), entries15};

			return table15;
		}
		break;
	case 16:
		{
			static const SHuffmanTable table16 = {1, sizeof(entries16) / sizeof(entries16[0]), entries16};

			return table16;
		}
		break;
	case 17:
		{
			static const SHuffmanTable table17 = {2, sizeof(entries16) / sizeof(entries16[0]), entries16};

			return table17;
		}
		break;
	case 18:
		{
			static const SHuffmanTable table18 = {3, sizeof(entries16) / sizeof(entries16[0]), entries16};

			return table18;
		}
		break;
	case 19:
		{
			static const SHuffmanTable table19 = {4, sizeof(entries16) / sizeof(entries16[0]), entries16};

			return table19;
		}
		break;
	case 20:
		{
			static const SHuffmanTable table20 = {6, sizeof(entries16) / sizeof(entries16[0]), entries16};

			return table20;
		}
		break;
	case 21:
		{
			static const SHuffmanTable table21 = {8, sizeof(entries16) / sizeof(entries16[0]), entries16};

			return table21;
		}
		break;
	case 22:
		{
			static const SHuffmanTable table22 = {10, sizeof(entries16) / sizeof(entries16[0]), entries16};

			return table22;
		}
		break;
	case 23:
		{
			static const SHuffmanTable table23 = {13, sizeof(entries16) / sizeof(entries16[0]), entries16};

			return table23;
		}
		break;
	case 24:
		{
			static const SHuffmanTable table24 = {4, sizeof(entries24) / sizeof(entries24[0]), entries24};

			return table24;
		}
		break;
	case 25:
		{
			static const SHuffmanTable table25 = {5, sizeof(entries24) / sizeof(entries24[0]), entries24};

			return table25;
		}
		break;
	case 26:
		{
			static const SHuffmanTable table26 = {6, sizeof(entries24) / sizeof(entries24[0]), entries24};

			return table26;
		}
		break;
	case 27:
		{
			static const SHuffmanTable table27 = {7, sizeof(entries24) / sizeof(entries24[0]), entries24};

			return table27;
		}
		break;
	case 28:
		{
			static const SHuffmanTable table28 = {8, sizeof(entries24) / sizeof(entries24[0]), entries24};

			return table28;
		}
		break;
	case 29:
		{
			static const SHuffmanTable table29 = {9, sizeof(entries24) / sizeof(entries24[0]), entries24};

			return table29;
		}
		break;
	case 30:
		{
			static const SHuffmanTable table30 = {11, sizeof(entries24) / sizeof(entries24[0]), entries24};

			return table30;
		}
		break;
	case 31:
		{
			static const SHuffmanTable table31 = {13, sizeof(entries24) / sizeof(entries24[0]), entries24};

			return table31;
		}
		break;
	default:
		ASSERT(0);
	}
}

void CMP3Decoder::HuffmanDecodeQuadruple(int8 &refV, int8 &refW, int16 &refX, int16 &refY, uint8 tableIdx, CBitInputStream &refBitStream)
{
	//3-B.7

	refV = 0;
	refW = 0;
	refX = 0;
	refY = 0;

	if(tableIdx == 0) //Table A
	{
		if(refBitStream.Read(1))
		{
			//Read: 1
			goto accept;
		}

		//Read: 0
		if(refBitStream.Read(1))
		{
			//Read: 01
			if(refBitStream.Read(1))
			{
				//Read: 011
				if(refBitStream.Read(1))
				{
					//Read: 0111
					refV = 1;
					goto accept;
				}

				//Read: 0110
				refW = 1;
				goto accept;
			}

			//Read: 010
			if(refBitStream.Read(1))
			{
				//Read: 0101
				refY = 1;
				goto accept;
			}

			//Read: 0100
			refX = 1;
			goto accept;
		}

		//Read: 00
		if(refBitStream.Read(1))
		{
			//Read: 001
			if(refBitStream.Read(1))
			{
				//Read: 0011
				if(refBitStream.Read(1))
				{
					//Read: 00111
					refV = 1;
					refW = 1;
					goto accept;
				}

				//Read: 00110
				refV = 1;
				refX = 1;
				goto accept;
			}

			//Read: 0010
			if(refBitStream.Read(1))
			{
				//Read: 00101
				refX = 1;
				refY = 1;
				goto accept;
			}

			//Read: 00100
			refW = 1;
			refX = 1;
			goto accept;
		}

		//Read: 000
		if(refBitStream.Read(1))
		{
			//Read: 0001
			if(refBitStream.Read(1))
			{
				//Read: 00011
				refV = 1;
				refY = 1;
				goto accept;
			}

			//Read: 00010
			if(refBitStream.Read(1))
			{
				//Read: 000101
				refW = 1;
				refY = 1;
				goto accept;
			}

			//Read: 000100
			refW = 1;
			refX = 1;
			refY = 1;
			goto accept;
		}

		//Read: 0000
		if(refBitStream.Read(1))
		{
			//Read: 00001
			if(refBitStream.Read(1))
			{
				//Read: 000011
				refV = 1;
				refW = 1;
				refX = 1;
				goto accept;
			}

			//Read: 000010
			refV = 1;
			refW = 1;
			refY = 1;
			goto accept;
		}

		//Read: 00000
		if(refBitStream.Read(1))
		{
			//Read: 000001
			refV = 1;
			refW = 1;
			refX = 1;
			refY = 1;
			goto accept;
		}

		//Read: 000000
		refV = 1;
		refX = 1;
		refY = 1;
		goto accept;
	}
	else if(tableIdx == 1)
	{
		//hlen is = 4 for all values
		switch(refBitStream.Read(4))
		{
		case 0:
			refV = 1;
			refW = 1;
			refX = 1;
			refY = 1;
			goto accept;
		case 1:
			refV = 1;
			refW = 1;
			refX = 1;
			goto accept;
		case 2:
			refV = 1;
			refW = 1;
			refY = 1;
			goto accept;
		case 3:
			refV = 1;
			refW = 1;
			goto accept;
		case 4:
			refV = 1;
			refX = 1;
			refY = 1;
			goto accept;
		case 5:
			refV = 1;
			refX = 1;
			goto accept;
		case 6:
			refV = 1;
			refY = 1;
			goto accept;
		case 7:
			refV = 1;
			goto accept;
		case 8:
			refW = 1;
			refX = 1;
			refY = 1;
			goto accept;
		case 9:
			refW = 1;
			refX = 1;
			goto accept;
		case 10:
			refW = 1;
			refY = 1;
			goto accept;
		case 11:
			refW = 1;
			goto accept;
		case 12:
			refX = 1;
			refY = 1;
			goto accept;
		case 13:
			refX = 1;
			goto accept;
		case 14:
			refY = 1;
			goto accept;
		case 15:
			goto accept;
		}
	}

	ASSERT(0);

accept:;
	if(refV)
		if(refBitStream.Read(1))
			refV = -refV;
	if(refW)
		if(refBitStream.Read(1))
			refW = -refW;
	if(refX)
		if(refBitStream.Read(1))
			refX = -refX;
	if(refY)
		if(refBitStream.Read(1))
			refY = -refY;
}
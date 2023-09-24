#include <ACStdLib.h>
using namespace ACStdLib;

//Definitions
#define MPEG1_STARTCODE_PREFIX 0x1 //3 bytes wide
#define MPEG1_STARTCODE_PACKHEADER 0xBA

//MPEG-1 Video
#define MPEG1VIDEO_STARTCODE_PICTURE 0
#define MPEG1VIDEO_STARTCODE_SEQUENCE_HEADER 0xB3
#define MPEG1VIDEO_STARTCODE_EXTENSION 0xB5
#define MPEG1VIDEO_STARTCODE_GROUPOFPICTURES 0xB8

#define MPEG1VIDEO_STARTCODE_SLICE_FIRST 1
#define MPEG1VIDEO_STARTCODE_SLICE_LAST 0xAF

//Functions
bool FindNextStartCode(uint32 &refState, ASeekableInputStream &refInput);
#include <ACStdLib.h>
using namespace ACStdLib;
//Namespaces
using namespace ACStdLib::Multimedia;
//Definitions
#define BMP_HEADER_TYPE "BM"
#define BMP_HEADER_TYPE_SIZE 2
#define BMP_INFOHEADER_SIZE 40

//Prototypes
void AddMS_FourCC_VideoCodecs(CFiniteSet<ECodecId> &refCodecSet);
void ReadBMPHeader(bool &refIsBottomUp, AInputStream &refInput, CVideoStream &refStream);
#include "GifProcessor.h"

// public:

void CGifProcessor::Print(SGif *Gif)
{
	printf_s("Header Block: ");
	NMess::PrintHex(Gif->Header, GIF_SIZE_HEADER_BLOCK);
	printf_s(" => ");
	NMess::PrintAscii(Gif->Header, GIF_SIZE_HEADER_BLOCK);
	printf_s("\n");

	printf_s("Logical Screen Descriptor: ");
	NMess::PrintHex(Gif->LocalScreenDescriptor, GIF_SIZE_LOCAL_SCREEN_DESCRIPTOR);
	printf_s("\n");
	uint16_t CanvasWidth = (Gif->LocalScreenDescriptor[1] << 8) | Gif->LocalScreenDescriptor[0];
	uint16_t CanvasHeight = (Gif->LocalScreenDescriptor[3] << 8) | Gif->LocalScreenDescriptor[2];
	uint8_t PackedField = Gif->LocalScreenDescriptor[4];
	uint8_t BackgroundColorIndex = Gif->LocalScreenDescriptor[5];
	uint8_t PixelAspectRatio = Gif->LocalScreenDescriptor[6];

	uint8_t GlobalColorTableFlag = (PackedField >> 7) & 0b1;
	uint8_t ColorResolution = ((PackedField >> 4) & 0b111) + 1;
	uint8_t SortFlag = (PackedField >> 3) & 0b1;
	uint16_t GlobalColorTableSize = (uint16_t) pow(2, (PackedField & 0b111) + 1);
	
	printf_s("\tCanvas Width: %i\n", CanvasWidth);
	printf_s("\tCanvas Height: %i\n", CanvasHeight);
	printf_s("\tPacked Field: %02x\n", PackedField);
	printf_s("\t\tGlobal Color Table Flag: %i\n", GlobalColorTableFlag);
	printf_s("\t\tColor Resolution: %i\n", ColorResolution);
	printf_s("\t\tSort Flag: %i\n", SortFlag);
	printf_s("\t\tSize of Global Color Table: %i\n", GlobalColorTableSize);
	printf_s("\tBackground Color Index: %i\n", BackgroundColorIndex);
	printf_s("\tPixel Aspect Ratio: %i\n", PixelAspectRatio);

	if (ColorResolution != 8) {
		printf_s("GifLoader >> Print >> Color Depth not equal to 8!\n");
	}

	if (GlobalColorTableFlag) {
		printf_s("Global Color Table:\n");
		uint32_t GlobalColorTableByteLength = sizeof(uint8_t) * GlobalColorTableSize * GIF_MODE_RGB;
		uint32_t Counter = 0;
		uint32_t Limit = 8;
		printf_s("\t");
		for (uint32_t I = 0; I < GlobalColorTableByteLength; I += GIF_MODE_RGB) {
			if (Counter == Limit) {
				printf_s(
					"\n\t%02x%02x%02x, ",
					Gif->GlobalColorTable[I],
					Gif->GlobalColorTable[I + 1],
					Gif->GlobalColorTable[I + 2]
				);
				Counter = 0;
			}
			else if (Counter == (Limit - 1)) {
				printf_s(
					"%02x%02x%02x",
					Gif->GlobalColorTable[I],
					Gif->GlobalColorTable[I + 1],
					Gif->GlobalColorTable[I + 2]
				);
			}
			else {
				printf_s(
					"%02x%02x%02x, ",
					Gif->GlobalColorTable[I],
					Gif->GlobalColorTable[I + 1],
					Gif->GlobalColorTable[I + 2]
				);
			}
			Counter++;
		}
	}
}

void CGifProcessor::Load(const char *Filepath, SGif *Gif)
{
	CFile *File = CFile::Open(Filepath, "rb");

	File->Read(&Gif->Header, sizeof(uint8_t), ASIZE(Gif->Header));
	
	uint64_t HeaderValue = ((uint64_t) *((uint32_t *) Gif->Header)) | ((uint64_t) *((uint16_t *) (Gif->Header + sizeof(uint32_t))) << 32);
	if ((NUtilConverter::SwapEndian(HeaderValue) >> 16) != GIF_MAGIC_HEADER_BLOCK) {
		printf_s("GifLoader >> Load >> Incorrect Header Block!\n");
		return;
	}

	File->Read(Gif->LocalScreenDescriptor, sizeof(uint8_t), ASIZE(Gif->LocalScreenDescriptor));

	uint16_t CanvasWidth = (Gif->LocalScreenDescriptor[1] << 8) | Gif->LocalScreenDescriptor[0];
	uint16_t CanvasHeight = (Gif->LocalScreenDescriptor[3] << 8) | Gif->LocalScreenDescriptor[2];
	uint8_t PackedField = Gif->LocalScreenDescriptor[4];
	uint8_t BackgroundColorIndex = Gif->LocalScreenDescriptor[5];
	uint8_t PixelAspectRatio = Gif->LocalScreenDescriptor[6];

	uint8_t GlobalColorTableFlag = (PackedField >> 7) & 0b1;
	uint8_t ColorResolution = ((PackedField >> 4) & 0b111) + 1;
	uint8_t SortFlag = (PackedField >> 3) & 0b1;
	uint16_t GlobalColorTableSize = (uint16_t) pow(2, (PackedField & 0b111) + 1);
	
	if (ColorResolution != 8) {
		printf_s("GifLoader >> Load >> Color Depth not equal to 8!\n");
	}

	if (GlobalColorTableFlag) {
		uint32_t GlobalColorTableByteLength = sizeof(uint8_t) * GlobalColorTableSize * GIF_MODE_RGB;
		Gif->GlobalColorTable = (uint8_t *) malloc(GlobalColorTableByteLength);
		File->Read(Gif->GlobalColorTable, sizeof(uint8_t), GlobalColorTableByteLength);
	}

	uint8_t ExtensionIntroducer;

	while ((ExtensionIntroducer = File->ReadUInt8()) != GIF_MAGIC_IMAGE_DESCRIPTOR) {
		printf("Extension Introducer: %02x\n", ExtensionIntroducer);
	}
	
}
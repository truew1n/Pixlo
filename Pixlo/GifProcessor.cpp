#include "GifProcessor.h"

#include <vector>

// public:

void CGifProcessor::Print(SGif *Gif)
{
	/*printf_s("Header Block: ");
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
	}*/
}

SAnimation CGifProcessor::Load(const char *Filepath, SGif *Gif)
{
	CFile *File = CFile::Open(Filepath, "rb");

	File->Read(&Gif->Header, sizeof(uint8_t), GIF_SIZE_HEADER_BLOCK);
	
	uint64_t HeaderValue = ((uint64_t) *((uint32_t *) Gif->Header)) | ((uint64_t) *((uint16_t *) (Gif->Header + sizeof(uint32_t))) << 32);
	if ((NUtilConverter::SwapEndian(HeaderValue) >> 16) != GIF_MAGIC_HEADER_BLOCK) {
		printf_s("GifLoader >> Load >> Incorrect Header Block!\n");
		return {};
	}

	File->Read(Gif->LocalScreenDescriptor, sizeof(uint8_t), GIF_SIZE_LOCAL_SCREEN_DESCRIPTOR);

	uint16_t CanvasWidth = (Gif->LocalScreenDescriptor[1] << 8) | Gif->LocalScreenDescriptor[0];
	uint16_t CanvasHeight = (Gif->LocalScreenDescriptor[3] << 8) | Gif->LocalScreenDescriptor[2];
	uint8_t GlobalPackedField = Gif->LocalScreenDescriptor[4];
	uint8_t BackgroundColorIndex = Gif->LocalScreenDescriptor[5];
	uint8_t PixelAspectRatio = Gif->LocalScreenDescriptor[6];

	uint8_t GlobalColorTableFlag = (GlobalPackedField >> 7) & 0b1;
	uint8_t GlobalColorResolution = ((GlobalPackedField >> 4) & 0b111) + 1;
	uint8_t GlobalSortFlag = (GlobalPackedField >> 3) & 0b1;
	uint16_t GlobalColorTableSize = (uint16_t) pow(2, (GlobalPackedField & 0b111) + 1);

	if (GlobalColorTableFlag) {
		uint32_t GlobalColorTableByteLength = sizeof(uint8_t) * GlobalColorTableSize * GIF_MODE_RGB;
		Gif->GlobalColorTable = (uint8_t *) malloc(GlobalColorTableByteLength);
		File->Read(Gif->GlobalColorTable, sizeof(uint8_t), GlobalColorTableByteLength);
	}

	SAnimation GifAnimation;
	uint8_t *PreviousFrameData = nullptr;
	GifAnimation.Size = 0;

	uint8_t TrailerMarker = 0;
	while ((TrailerMarker = File->ReadUInt8()) != GIF_MAGIC_TRAILER_MARKER) {
		File->Seek(File->Tell() - 1, ESeekOrigin::Set);
		SGifFrame GifFrame = { 0 };

		uint8_t ExtensionIntroducer;
		while ((ExtensionIntroducer = File->ReadUInt8()) != GIF_MAGIC_IMAGE_DESCRIPTOR) {
			uint32_t ExtensionLabel = File->ReadUInt8();

			switch (ExtensionLabel) {
				case GIF_MAGIC_APPLICATION_EXTENSION_LABEL: {
					uint8_t BytesSkip = File->ReadUInt8();
					File->Seek(BytesSkip, ESeekOrigin::Current);
					BytesSkip = File->ReadUInt8();
					File->Seek(BytesSkip + 1, ESeekOrigin::Current);
					break;
				}
				case GIF_MAGIC_GRAPHICS_CONTROL_EXTENSION_LABEL: {
					GifFrame.GraphicsControlExtension[0] = ExtensionIntroducer;
					GifFrame.GraphicsControlExtension[1] = ExtensionLabel;

					File->Read(GifFrame.GraphicsControlExtension + 2, sizeof(uint8_t), GIF_SIZE_GRAPHICS_CONTROL_EXTENSION - 2);
					break;
				}
				case GIF_MAGIC_COMMENT_EXTENSION_LABEL:
				case GIF_MAGIC_PLAIN_TEXT_EXTENSION_LABEL: {
					uint8_t BytesSkip = 0;
					while ((BytesSkip = File->ReadUInt8()) != 0) {
						File->Seek(BytesSkip, ESeekOrigin::Current);
					}
					break;
				}
				default: {
					printf_s("GifLoader >> Load >> Uknown Extension Label: %02x!\n", ExtensionLabel);
					return {};
					break;
				}
			}
		}


		GifFrame.ImageDescriptor[0] = ExtensionIntroducer;
		File->Read(GifFrame.ImageDescriptor + 1, sizeof(uint8_t), GIF_SIZE_IMAGE_DESCRIPTOR - 1);

		uint16_t ImageLeft = (GifFrame.ImageDescriptor[2] << 8) | GifFrame.ImageDescriptor[1];
		uint16_t ImageTop = (GifFrame.ImageDescriptor[4] << 8) | GifFrame.ImageDescriptor[3];
		uint16_t ImageWidth = (GifFrame.ImageDescriptor[6] << 8) | GifFrame.ImageDescriptor[5];
		uint16_t ImageHeight = (GifFrame.ImageDescriptor[8] << 8) | GifFrame.ImageDescriptor[7];
		uint8_t LocalPackedField = GifFrame.ImageDescriptor[9];

		uint8_t LocalColorTableFlag = (LocalPackedField >> 7) & 0b1;
		uint8_t InterlaceFlag = (LocalPackedField >> 6) & 0b1;
		uint8_t LocalSortFlag = (LocalPackedField >> 5) & 0b1;
		uint16_t LocalColorTableSize = (uint16_t) pow(2, (LocalPackedField & 0b111) + 1);

		if (LocalColorTableFlag) {
			uint32_t LocalColorTableByteLength = sizeof(uint8_t) * LocalColorTableSize * GIF_MODE_RGB;
			GifFrame.LocalColorTable = (uint8_t *) malloc(LocalColorTableByteLength);
			File->Read(GifFrame.LocalColorTable, sizeof(uint8_t), LocalColorTableByteLength);
		}

		uint8_t MinimumCodeSize = File->ReadUInt8();
		uint16_t ClearCode = 1 << MinimumCodeSize;
		uint16_t EndOfInformationCode = ClearCode + 1;

		uint16_t CodeSize = MinimumCodeSize + 1;
		uint16_t MaxCode = (1 << CodeSize) - 1;
		uint16_t NextCode = ClearCode + 2;

		std::vector<std::vector<uint8_t>> CodeTable(1 << 12);
		for (uint16_t i = 0; i < ClearCode; ++i) {
			CodeTable[i] = { (uint8_t) i };
		}

		std::vector<uint8_t> OutputBuffer;
		uint16_t PreviousCode = 0xFFFF;
		uint32_t DataAccumulator = 0;
		uint8_t BitsInAccumulator = 0;

		uint8_t BlockSize = 0;
		while ((BlockSize = File->ReadUInt8()) != 0) {
			std::vector<uint8_t> CompressedData(BlockSize);
			File->Read(CompressedData.data(), sizeof(uint8_t), BlockSize);

			for (uint8_t Byte : CompressedData) {
				DataAccumulator |= (Byte << BitsInAccumulator);
				BitsInAccumulator += 8;

				while (BitsInAccumulator >= CodeSize) {
					uint16_t CurrentCode = DataAccumulator & ((1 << CodeSize) - 1);
					DataAccumulator >>= CodeSize;
					BitsInAccumulator -= CodeSize;

					if (CurrentCode == ClearCode) {
						CodeSize = MinimumCodeSize + 1;
						MaxCode = (1 << CodeSize) - 1;
						NextCode = ClearCode + 2;
						PreviousCode = 0xFFFF;
						continue;
					}
					if (CurrentCode == EndOfInformationCode) {
						goto LZWDecompressionDone;
					}

					if (PreviousCode == 0xFFFF) {
						OutputBuffer.insert(OutputBuffer.end(), CodeTable[CurrentCode].begin(), CodeTable[CurrentCode].end());
						PreviousCode = CurrentCode;
						continue;
					}

					std::vector<uint8_t> Entry;
					if (CurrentCode < NextCode) {
						Entry = CodeTable[CurrentCode];
					}
					else if (CurrentCode == NextCode) {
						Entry = CodeTable[PreviousCode];
						Entry.push_back(CodeTable[PreviousCode][0]);
					}
					else {
						printf_s("GifLoader >> LZWDecompression >> Invalid code detected!\n");
						return {};
					}

					OutputBuffer.insert(OutputBuffer.end(), Entry.begin(), Entry.end());

					if (NextCode < (1 << 12)) {
						std::vector<uint8_t> NewEntry = CodeTable[PreviousCode];
						NewEntry.push_back(Entry[0]);
						CodeTable[NextCode++] = NewEntry;

						if (NextCode > MaxCode && CodeSize < 12) {
							++CodeSize;
							MaxCode = (1 << CodeSize) - 1;
						}
					}

					PreviousCode = CurrentCode;
				}
			}
		}

	LZWDecompressionDone:

		GifFrame.ImageData = (uint8_t *) malloc(OutputBuffer.size());
		memcpy(GifFrame.ImageData, OutputBuffer.data(), OutputBuffer.size());
		GifFrame.ImageDataSize = OutputBuffer.size();

		uint8_t *ColorTable = nullptr;
		uint16_t ColorTableSize = 0;

		if (LocalColorTableFlag) {
			ColorTable = GifFrame.LocalColorTable;
			ColorTableSize = LocalColorTableSize;
		}
		else if (GlobalColorTableFlag) {
			ColorTable = Gif->GlobalColorTable;
			ColorTableSize = GlobalColorTableSize;
		}
		else {
			printf_s("GifLoader >> Load >> No color table available!\n");
			return {};
		}


		uint16_t TransparentIndex = GifFrame.GraphicsControlExtension[6]; // Get transparent index

		uint32_t PixelCount = CanvasWidth * CanvasHeight; // Ensure all frames use this size
		uint8_t *ARGBData = (uint8_t *) malloc(PixelCount * GIF_MODE_ARGB);

		// Process the frame image
		
		for (uint32_t i = 0; i < ImageHeight; ++i) {
			for (uint32_t j = 0; j < ImageWidth; ++j) {
				uint16_t Index = GifFrame.ImageData[i * ImageWidth + j];
				uint32_t PixelIndex = (ImageTop + i) * CanvasWidth + (ImageLeft + j);

				// Handle transparency
				if (Index == TransparentIndex && PreviousFrameData) {
					ARGBData[PixelIndex * GIF_MODE_ARGB + 0] = PreviousFrameData[PixelIndex * GIF_MODE_ARGB + 0];
					ARGBData[PixelIndex * GIF_MODE_ARGB + 1] = PreviousFrameData[PixelIndex * GIF_MODE_ARGB + 1];
					ARGBData[PixelIndex * GIF_MODE_ARGB + 2] = PreviousFrameData[PixelIndex * GIF_MODE_ARGB + 2];
					ARGBData[PixelIndex * GIF_MODE_ARGB + 3] = PreviousFrameData[PixelIndex * GIF_MODE_ARGB + 3];
					continue;
				}

				if (Index >= ColorTableSize) {
					printf_s("GifLoader >> Load >> Invalid color index detected!\n");
					free(ARGBData);
					return {};
				}

				// Map index to ARGB
				 // Ensure position matches canvas
				ARGBData[PixelIndex * GIF_MODE_ARGB + 0] = ColorTable[Index * GIF_MODE_RGB + 2]; // Red
				ARGBData[PixelIndex * GIF_MODE_ARGB + 1] = ColorTable[Index * GIF_MODE_RGB + 1]; // Green
				ARGBData[PixelIndex * GIF_MODE_ARGB + 2] = ColorTable[Index * GIF_MODE_RGB + 0]; // Blue
				ARGBData[PixelIndex * GIF_MODE_ARGB + 3] = 0xFF; // Alpha: fully opaque
			}
		}

		// Use the ARGB data for the current frame
		SImage Image;
		Image.Data = ARGBData;
		Image.Width = CanvasWidth;
		Image.Height = CanvasHeight;
		Image.Bits = GlobalColorResolution;
		Image.Channel = GIF_MODE_ARGB;

		PreviousFrameData = Image.Data;

		GifAnimation.Frames.push_back(Image);
		GifAnimation.Delay = GifFrame.GraphicsControlExtension[5] << 8 | GifFrame.GraphicsControlExtension[4];
		GifAnimation.Size++;

		File->Seek(1, ESeekOrigin::Current);
	}
	
	return GifAnimation;
}
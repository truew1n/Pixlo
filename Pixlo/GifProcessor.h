#ifndef PIXLO_GIF_LOADER_H
#define PIXLO_GIF_LOADER_H

#include <iostream>

#include "Util.h"
#include "File.h"
#include "Type.h"

// ######## CHUNK SIZE ########

#define GIF_SIZE_HEADER_BLOCK 6
#define GIF_SIZE_LOCAL_SCREEN_DESCRIPTOR 7
#define GIF_SIZE_GRAPHICS_CONTROL_EXTENSION 8
#define GIF_SIZE_IMAGE_DESCRIPTOR 10

// ######## MAGIC NUMBERS ########

#define GIF_MAGIC_HEADER_BLOCK 0x474946383961
#define GIF_MAGIC_EXTENSION_INTRODUCER 0x21
#define GIF_MAGIC_IMAGE_DESCRIPTOR 0x2C
#define GIF_MAGIC_GRAPHICS_CONTROL_EXTENSION_LABEL 0xF9
#define GIF_MAGIC_PLAIN_TEXT_EXTENSION_LABEL 0x01
#define GIF_MAGIC_APPLICATION_EXTENSION_LABEL 0xFF
#define GIF_MAGIC_COMMENT_EXTENSION_LABEL 0xFE
#define GIF_MAGIC_TRAILER_MARKER 0x3B

// ######## COLOR MODES ########

#define GIF_MODE_RGB 3
#define GIF_MODE_ARGB 4

typedef struct SGifFrame {
	uint8_t GraphicsControlExtension[GIF_SIZE_GRAPHICS_CONTROL_EXTENSION];
	uint8_t ImageDescriptor[GIF_SIZE_IMAGE_DESCRIPTOR];
	uint8_t *LocalColorTable;
	uint8_t *ImageData;
	uint64_t ImageDataSize;
} SGifFrame;

typedef struct SGif {
	uint8_t Header[GIF_SIZE_HEADER_BLOCK];
	uint8_t LocalScreenDescriptor[GIF_SIZE_LOCAL_SCREEN_DESCRIPTOR];
	uint8_t *GlobalColorTable;
	SGifFrame *Frames;
	uint32_t FrameCount;
} SGif;


class CGifProcessor {
public:
	static SAnimation Load(const char *Filepath, SGif *Gif);
};

#endif
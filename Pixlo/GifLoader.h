#ifndef PIXLO_GIF_LOADER_H
#define PIXLO_GIF_LOADER_H

#include <iostream>

#include "Util.h"
#include "Array.h"


// **** Gif Globals: Per File ****

#define GIF_HEADER_BLOCK_SIZE 6
#define GIF_LOCAL_SCREEN_DESCRIPTOR_SIZE 7

// **** Gif Locals: Per Frame ****

#define GIF_GRAPHICS_CONTROL_EXTENSION 8
#define GIF_IMAGE_DESCRIPTOR 10

typedef struct SGifFrame {
	uint8_t GraphicsControlExtension[GIF_GRAPHICS_CONTROL_EXTENSION];
	uint8_t ImageDescriptor[GIF_IMAGE_DESCRIPTOR];
	uint8_t *ImageData;
};

typedef struct SGif {
	uint8_t Header[GIF_HEADER_BLOCK_SIZE];
	uint8_t LocalScreenDescriptor[GIF_LOCAL_SCREEN_DESCRIPTOR_SIZE];
	uint8_t *GlobalColorTable;
	SGifFrame *Frames;
} SGif;


class CGifLoader {
public:
	void Print(SGif *Gif);

	void Load(const char *Filepath, SGif *Gif);
};

#endif
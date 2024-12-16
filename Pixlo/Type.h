#ifndef PIXLO_TYPE_H
#define PIXLO_TYPE_H

#include <iostream>
#include <vector>

typedef struct SImage {
	uint8_t *Data;
	uint32_t Width;
	uint32_t Height;
	uint8_t Bits;
	uint8_t Channel;
} SImage;

typedef struct SAnimation {
	std::vector<SImage> Frames;
	uint32_t Delay;
	uint32_t Size;
} SAnimation;

#endif
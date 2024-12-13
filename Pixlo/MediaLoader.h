#ifndef PIXLO_MEDIA_LOADER_H
#define PIXLO_MEDIA_LOADER_H

#include <iostream>

#include "Array.h"

/*
	Data - Contains whole data of the frame
	Width - Width of the frame
	Height - Height of the frame
	Bits - How many bits per pixel
	Channel - How many channels is per pixel
	Note: Some modes can have 1 channel and 32 bits. So it would be R32.
*/
typedef struct SImage {
	uint8_t *Data;
	uint32_t Width;
	uint32_t Height;
	uint8_t Bits;
	uint8_t Channel;
} SImage;

typedef struct SAnimation {
	TArray<SImage> Frames;
	uint32_t Size;
} SAnimation;

enum class EImageFileFormat : uint8_t {
	BMP,
	PNG,
	JPG,
	SVG
};

enum class EAnimationFileFormat : uint8_t {
	GIF,
	APNG
};

class CMediaLoader {
private:
	static const char* MatchFileFormat(EImageFileFormat FileFormat);
	static const char* MatchFileFormat(EAnimationFileFormat FileFormat);

	SAnimation LoadGif(const char *Filepath);
public:
	static SImage LoadImage(const char *Filepath, EImageFileFormat FileFormat);
	static SAnimation LoadAnimation(const char *Filepath, EAnimationFileFormat FileFormat);
};

#endif
#ifndef PIXLO_MEDIA_LOADER_H
#define PIXLO_MEDIA_LOADER_H

#include <iostream>

#include "Array.h"
#include "Type.h"

/*
	Data - Contains whole data of the frame
	Width - Width of the frame
	Height - Height of the frame
	Bits - How many bits per pixel
	Channel - How many channels is per pixel
	Note: Some modes can have 1 channel and 32 bits. So it would be R32.
*/


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

class CMediaProcessor {
private:
	static const char* MatchFileFormat(EImageFileFormat FileFormat);
	static const char* MatchFileFormat(EAnimationFileFormat FileFormat);

	SAnimation LoadGif(const char *Filepath);
public:
	static SImage LoadImage(const char *Filepath, EImageFileFormat FileFormat);
	static SAnimation LoadAnimation(const char *Filepath, EAnimationFileFormat FileFormat);
};

#endif
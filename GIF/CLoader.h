#ifndef GIFLIB_LOADER_H
#define GIFLIB_LOADER_H

#include <iostream>


/*
	Data - Contains whole data of the frame
	Width - Width of the frame
	Height - Height of the frame
	Bits - How many bits per pixel
	Channel - How many channels is per pixel
	Note: Some modes can have 1 channel and 32 bits. So it would be R32.
*/
typedef struct SFrame {
	uint8_t *Data;
	uint32_t Width;
	uint32_t Height;
	uint8_t Bits;
	uint8_t Channel;
} SFrame;

typedef struct EFileFormat {

};

class CLoader {

	static SFrame LoadFrame(const char *Filepath, EFileFormat FileFormat);

};

#endif
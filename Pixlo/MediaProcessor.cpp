#include "MediaProcessor.h"

// private:

const char* CMediaProcessor::MatchFileFormat(EImageFileFormat FileFormat)
{
	switch (FileFormat) {
		case EImageFileFormat::BMP: return "BMP";
		case EImageFileFormat::PNG: return "PNG";
		case EImageFileFormat::JPG: return "JPG";
		case EImageFileFormat::SVG: return "SVG";
		default: return "UNKNOWN IMAGE FORMAT";
	}
}

const char* CMediaProcessor::MatchFileFormat(EAnimationFileFormat FileFormat)
{
	switch (FileFormat) {
		case EAnimationFileFormat::GIF: return "GIF";
		case EAnimationFileFormat::APNG: return "APNG";
		default: return "UNKNOWN ANIMATION FORMAT";
	}
}

SAnimation CMediaProcessor::LoadGif(const char *Filepath)
{
	return {};
}

// public:

SImage CMediaProcessor::LoadImage(const char *Filepath, EImageFileFormat FileFormat)
{
	SImage Image = { 0 };
	switch (FileFormat) {
		case EImageFileFormat::BMP: {

			break;
		}
		default: {
			std::cout << "MediaLoader >> LoadImage >> Unknown file format!\n";
			break;
		}
	}
	return  {};
}

SAnimation CMediaProcessor::LoadAnimation(const char *Filepath, EAnimationFileFormat FileFormat)
{
	switch (FileFormat) {
		case EAnimationFileFormat::GIF: {
			
			break;
		}
		default: {
			std::cout << "MediaLoader >> LoadAnimation >> Unknown file format!\n";
			break;
		}
	}
	return {};
}
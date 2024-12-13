#include "MediaLoader.h"

// private:

const char* CMediaLoader::MatchFileFormat(EImageFileFormat FileFormat)
{
	switch (FileFormat) {
		case EImageFileFormat::BMP: return "BMP";
		case EImageFileFormat::PNG: return "PNG";
		case EImageFileFormat::JPG: return "JPG";
		case EImageFileFormat::SVG: return "SVG";
		default: return "UNKNOWN IMAGE FORMAT";
	}
}

const char* CMediaLoader::MatchFileFormat(EAnimationFileFormat FileFormat)
{
	switch (FileFormat) {
		case EAnimationFileFormat::GIF: return "GIF";
		case EAnimationFileFormat::APNG: return "APNG";
		default: return "UNKNOWN ANIMATION FORMAT";
	}
}

SAnimation CMediaLoader::LoadGif(const char *Filepath)
{

}

// public:

SImage CMediaLoader::LoadImage(const char *Filepath, EImageFileFormat FileFormat)
{
	SImage Image = { 0 };
	switch (FileFormat) {
		case EImageFileFormat::BMP: {

			break;
		}
		default: {
			std::cout << "LoadImage >> Unknown file format!\n";
			break;
		}
	}

}

SAnimation CMediaLoader::LoadAnimation(const char *Filepath, EAnimationFileFormat FileFormat)
{
	switch (FileFormat) {
		case EAnimationFileFormat::GIF: {
			
			break;
		}
		default: {
			std::cout << "LoadAnimation >> Unknown file format!\n";
			break;
		}
	}

}
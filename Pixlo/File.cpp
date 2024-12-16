#include "File.h"

CFile::CFile()
{
	File = nullptr;
	BInitialized = false;
}

CFile *CFile::Open(const char *Filepath, const char *Mode)
{
	CFile *NewFile = new CFile();

	fopen_s(&NewFile->File, Filepath, Mode);

	if (!NewFile->File) {
		std::cerr << "CFile: Failed to open a file: " << Filepath << std::endl;
	}

	NewFile->BInitialized = true;

	return NewFile;
}

uint8_t CFile::ReadUInt8()
{
	UNINITIALIZED_OPERATION_CHECK(0);

	return (uint8_t) fgetc(File);
}

int8_t CFile::ReadInt8()
{
	UNINITIALIZED_OPERATION_CHECK(0);

	return (int8_t) fgetc(File);
}

bool CFile::Read(void *Buffer, size_t ElementSize, size_t ElementCount)
{
	UNINITIALIZED_OPERATION_CHECK(false);
	
	fread(Buffer, ElementSize, ElementCount, File);

	return true;
}

bool CFile::Write(void *Buffer, size_t ElementSize, size_t ElementCount)
{
	UNINITIALIZED_OPERATION_CHECK(false);

	fwrite(Buffer, ElementSize, ElementCount, File);

	return true;
}

bool CFile::Seek(int64_t Offset, ESeekOrigin Origin)
{
	UNINITIALIZED_OPERATION_CHECK(false);

	fseek(File, (int32_t) Offset, (int32_t) Origin);

	return true;
}

uint64_t CFile::Tell()
{
	UNINITIALIZED_OPERATION_CHECK(false);

	return ftell(File);
}

CFile::~CFile()
{
	if (BInitialized) {
		fclose(File);
	}
}
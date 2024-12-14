#ifndef PT_FILE_H
#define PT_FILE_H

#include <iostream>

#define UNINITIALIZED_OPERATION_CHECK(ReturnValue)	\
if(!BInitialized) return ReturnValue;				\

enum class ESeekOrigin : int8_t {
	Current = SEEK_CUR,
	End = SEEK_END,
	Set = SEEK_SET
};

class CFile {
private:
	FILE *File;
	bool BInitialized;
public:
	CFile();

	static CFile *Open(const char *Filepath, const char *Mode);
	uint8_t ReadUInt8();
	int8_t ReadInt8();
	bool Read(void *Buffer, uint64_t ElementSize, uint64_t ElementCount);
	bool Write(void *Buffer, uint64_t ElementSize, uint64_t ElementCount);
	bool Seek(int32_t Offset, ESeekOrigin Origin);
	uint64_t Tell();

	~CFile();
};

#endif


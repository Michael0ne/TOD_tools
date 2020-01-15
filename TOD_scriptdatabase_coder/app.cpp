#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>

#define READ_BLOCK_SIZE 4

//	Dunno what these are, really.
struct Database_Header {
	int		m_nUnk1;
	int		m_nUnk2;
};

void processFile(FILE* filePtr)
{
	fseek(filePtr, 0, 0);

	char*	buffer = nullptr;
	long	currOffset = 0;
	long	maxOffset = 0;

	fseek(filePtr, 0, SEEK_END);
	maxOffset = ftell(filePtr);
	rewind(filePtr);

	//	TODO: doesn't work!
	while (currOffset < maxOffset) {
		if (!buffer) buffer = (char*)malloc(READ_BLOCK_SIZE); else realloc(buffer, READ_BLOCK_SIZE);

		fgets(buffer, READ_BLOCK_SIZE, filePtr);

		currOffset = ftell(filePtr) + 1;

		if (*buffer == (char)0 || *(buffer + (char)1) == (char)0 || *(buffer + (char)2) == (char)0 || *(buffer + (char)3) == (char)0) continue;

		printf("[0x%x]\t%s\t|\t%x %x %x %x\n", currOffset, buffer, buffer[0], buffer[1], buffer[2], buffer[3]);
	}

	if (buffer)
		free(buffer);
}

int main()
{
	FILE* hFile = fopen("database.bin", "rb");

	if (!hFile)
		return 0;
	
	processFile(hFile);

	fclose(hFile);

	return 1;
}
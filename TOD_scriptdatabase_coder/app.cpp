#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>

//	Header.
struct Database_Header {
	unsigned int	m_nUnk1;	//	Probably unsigned int m_nEntriesTotal;
};

//	For each 'entry'.
struct Database_Entry {
	unsigned char	m_nLength;	//	How many characters to read?
	char*			m_szEntry;	//	Actual string.
};

void processFile(FILE* filePtr)
{
	FILE* fileOutPtr = fopen("database.bin.txt", "w");

	if (!fileOutPtr) {
		printf("[ERROR] Could not create and open outer file!\n");

		return;
	}

	rewind(filePtr);
	rewind(fileOutPtr);

	//	Read header.
	Database_Header	header;

	fread(&header, sizeof(header), 1, filePtr);

	//	Output header.
	printf("[INFO]\tHeader is: %d\n", header.m_nUnk1);

	unsigned int entriesFound = 0;

	while (!feof(filePtr)) {
		BYTE len = 0;
		fread(&len, 1, 1, filePtr);
		if (len == 0) continue;

		char* buffer = (char*)malloc(len);
		if (buffer == nullptr) continue;

		fseek(filePtr, 3, SEEK_CUR);
		fread(buffer, len, 1, filePtr);

		printf("[0x%x]\t%s\n", ftell(filePtr), buffer);

		fputs(buffer, fileOutPtr);
		fputs("\n", fileOutPtr);

		entriesFound++;

		free(buffer);
	}

	printf("[INFO]\tFound: %d entries.\n", entriesFound);

	fclose(fileOutPtr);
}

int main()
{
	FILE* hFile = fopen("database.bin", "rb");

	if (!hFile) {
		printf("[ERROR] Could not open input database file!\n");

		return 0;
	}
	
	processFile(hFile);

	fclose(hFile);

	return 1;
}
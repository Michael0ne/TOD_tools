#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>

//	Header.
struct Database_Header {
	unsigned int	m_nEntriesTotal;
};

//	For each 'entry'.
struct Database_Entry {
	unsigned char	m_nLength;	//	How many characters to read?
	char*			m_szEntry;	//	Actual string.
};

void processFile(FILE* filePtr)
{
	FILE* fileOutPtr = fopen("entities.txt", "w");

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
	printf("[INFO]\tTotal global entities: %u\n", header.m_nEntriesTotal);

	unsigned int entriesFound = 0;
	char buffer[MAX_PATH];
	memset(&buffer, 0, sizeof(buffer));

	while (!feof(filePtr)) {
		if (entriesFound == header.m_nEntriesTotal) {
			printf("[INFO]\tFinished reading total %u out of %u global entities.\n", entriesFound, header.m_nEntriesTotal);

			unsigned int cmds = 0;
			fread(&cmds, 4, 1, filePtr);

			printf("[INFO]\tTotal commands: %u\n", cmds);

			fclose(fileOutPtr);

			fileOutPtr = fopen("commands.txt", "w");
			rewind(fileOutPtr);
		}

		unsigned char len = 0;
		fread(&len, 1, 1, filePtr);
		if (len == 0) continue;

		fseek(filePtr, 3, SEEK_CUR);
		fread(buffer, len, 1, filePtr);

		fputs(buffer, fileOutPtr);
		fputs("\n", fileOutPtr);

		entriesFound++;
	}

	fclose(fileOutPtr);
}

int main()
{
	FILE* hFile = fopen("database.bin", "rb");

	if (!hFile) {
		printf("[ERROR]\tCould not open input database file!\n");

		return 0;
	}
	
	processFile(hFile);

	fclose(hFile);

	getchar();

	return 1;
}
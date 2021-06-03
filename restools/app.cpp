#include "AssetBlockReader.h"
#include "TextureResourceReader.h"
#include "SaveResourceReader.h"
#include "ScriptDatabaseReader.h"
#include <iostream>

int main(_In_ int argc, _In_reads_(argc) _Pre_z_ char** argv, _In_z_ char** envp)
{
	//	NOTE: not enough arguments specified.
	if (argc < 3)
	{
		PrintUsage();

		return NULL;
	}

	if (!argv[1] || !argv[2])
	{
		char errmessage[32] = {};
		LoadString(GetModuleHandle(NULL), 106, errmessage, sizeof(errmessage));
		std::cout << errmessage << std::endl;

		return NULL;
	}

	//	NOTE: convert resource path right away (assured it's non-empty string). No heap allocations here :)
	ConvertBackslashes(argv[2]);
	const char* const lastslashpos = strrchr(argv[2], '/');
	const char* const dotpos = strrchr(argv[2], '.');
	char workingdir[1024] = {};
	strncpy_s(workingdir, sizeof(workingdir), argv[2], lastslashpos - argv[2]);

	if (lastslashpos == nullptr || (dotpos != nullptr && dotpos > lastslashpos))
	{
		char errmessage[32] = {};
		LoadString(GetModuleHandle(NULL), 105, errmessage, sizeof(errmessage));
		std::cout << errmessage << std::endl;

		return NULL;
	}

	//	NOTE: first argument - resource type (right now let's trust the user with their choice).
	GenericResourceReader* resreader = nullptr;

	if (*argv[1] == 't' &&
		strcmp(argv[1], "texture") == NULL)
		resreader = new TextureResourceReader(lastslashpos + 1);

	if ((*argv[1] == 's' && argv[1][1] == 'a') &&
		strcmp(argv[1], "save") == NULL)
		resreader = new SaveResourceReader(lastslashpos + 1);

	if ((*argv[1] == 's' && argv[1][1] == 'c') &&
		strcmp(argv[1], "scriptdb") == NULL)
		resreader = new ScriptDatabaseReader(lastslashpos + 1);

	if (*argv[1] == 'a' && argv[1][1] == 's' && argv[1][2] == 's' && argv[1][3] == 'e' && argv[1][4] == 't')
		resreader = new AssetBlockReader(lastslashpos + 1);

	if (resreader == nullptr)
	{
		char errmessage[32] = {};
		LoadString(GetModuleHandle(NULL), 104, errmessage, sizeof(errmessage));
		printf(errmessage, argv[1]);

		return NULL;
	}

	//	NOTE: set current working dir and try to open file.
	resreader->SetCurrentWorkingDir(workingdir);

	if (!resreader->OpenResourceFile())
	{
		resreader->PrintError();
		delete resreader;

		return NULL;
	}

	//	NOTE: file is o.k. so read and process data, then output (and dump, if possible).
	resreader->ReadInfo();
	resreader->PrintFileInfo();
	resreader->PrintInfo();

	if (argv[3] != nullptr &&
		(*argv[3] == 'd' && strcmp(argv[3], "dumpdata") == NULL))
		resreader->DumpData();

	delete resreader;

	return NULL;
}
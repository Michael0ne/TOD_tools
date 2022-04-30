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
        LoadString(GetModuleHandle(nullptr), 106, errmessage, sizeof(errmessage));
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
        LoadString(GetModuleHandle(nullptr), 105, errmessage, sizeof(errmessage));
        std::cout << errmessage << std::endl;

        return NULL;
    }

    //	NOTE: first argument - resource type (right now let's trust the user with their choice).
    GenericResourceReader* resreader = nullptr;

    if (strcmp(argv[1], "texture") == NULL)
        resreader = new TextureResourceReader(lastslashpos + 1, GenericResourceReader::PlatformDefinition::PC);

    if (strcmp(argv[1], "save") == NULL)
        resreader = new SaveResourceReader(lastslashpos + 1);

    if (strcmp(argv[1], "scriptdb") == NULL)
        resreader = new ScriptDatabaseReader(lastslashpos + 1);

    if (strcmp(argv[1], "asset") == NULL)
        resreader = new AssetBlockReader(lastslashpos + 1);

    //	NOTE: Future-proof. Just in case it will be better to handle different assets in a different way.
    if (strcmp(argv[1], "mission") == NULL)
        resreader = new AssetBlockReader(lastslashpos + 1, ".mission");

    if (strcmp(argv[1], "submap") == NULL)
        resreader = new AssetBlockReader(lastslashpos + 1, ".submap");

    if (strcmp(argv[1], "playerdata") == NULL)
        resreader = new AssetBlockReader(lastslashpos + 1, ".playerdata");

    if (resreader == nullptr)
    {
        char errmessage[32] = {};
        LoadString(GetModuleHandle(nullptr), 104, errmessage, sizeof(errmessage));
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
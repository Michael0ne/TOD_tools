#pragma once
#include "Globals.h"
#include "LogDump.h"
#include <vector>
#include <map>

#ifdef PLATFORM_PS2
    #define SCRIPT_PROPERTIES_TOTAL 5940
    #define SCRIPT_PROPERTIES_BUILTIN_TOTAL 711

    #define SCRIPT_PROPERTIES_BUILTIN_CRC 0xE99606BA
    #define SCRIPT_PROPERTIES_DATABASE_CRC 0x24A37B21
#endif
#ifdef PLATFORM_XBOX
#endif
#ifdef PLATFORM_PC
    #define SCRIPT_PROPERTIES_TOTAL 5994
    #define SCRIPT_PROPERTIES_BUILTIN_TOTAL 711

    #define SCRIPT_PROPERTIES_BUILTIN_CRC 0xE99606BA
    #define SCRIPT_PROPERTIES_LOADED_CRC 0x65C37710
#endif

struct GlobalProperty
{
    int                         m_PropertyId = 0;
    char*                       m_PropertyName = nullptr;
    class BaseType*             m_PropertyType = nullptr;

    GlobalProperty() {};
    GlobalProperty(const char* const propertyname, unsigned int ind);
    ~GlobalProperty();

    void                        GetNameAndType(String& outStr) const;	//	@8731C0

    static void                 ClearGlobalProperties();    //  @873650

    static GlobalProperty&      GetById(unsigned int id);	//	@872FC0
};

extern std::vector<GlobalProperty>		GlobalPropertiesList;	//	@A3CF20
extern std::map<String, unsigned int>	GlobalPropertiesMap;	//	@A3CF30

class GlobalCommand
{
protected:
    struct CommandArgument
    {
        String					m_PropertyName;
        class BaseType*			m_ScriptType;
        unsigned int			m_TotalSizeBytes;

        CommandArgument(String& argname, const class BaseType* argtype, unsigned int argsize);
    };

    struct CommandArguments
    {
        std::vector<CommandArgument>	m_ArgumentsList;
        int						field_C;
        unsigned int			m_TotalSizeBytes;
        unsigned int			m_TotalSize;
    };
    
    CommandArguments			m_Arguments;	//	NOTE: element at index 0 is always return scripttype.
    int							m_GlobalIndex;
    char*						m_ArgumentsString;	//	NOTE: full name with arguments.
    char*						m_CommandName;	//	NOTE: the name without arguments (only command name).

private:
    void						AddArgumentType(BaseType* argtype);	//	@862650

public:
    GlobalCommand(const char* const commandname, const unsigned int commandind);	//	@871FE0
    ~GlobalCommand();

    void						GetReturnTypeString(String& outStr);	//	@871A90

    static void                 ClearGlobalCommands();  //  @8724E0
};

extern std::vector<GlobalCommand>		GlobalCommandsList;	//	@A11470
extern std::map<String, unsigned int>	GlobalCommandsMap;	//	@A3CF08

class EntityType;
class Entity;

class GlobalScript
{
    struct Property
    {
        GlobalProperty         *m_Info;
        unsigned int            m_Offset;
        char*                   m_DefaultValue;
        unsigned int            field_C;
    };

    struct Method
    {
        unsigned short          m_Id;
        unsigned short          m_PropertyBlockId;
        void                    (* m_ThreadHandler)(class ScriptThread*);
        void                    (* m_MethodPtr)(int* args);
    };

    struct Parameter
    {
        void                    (* m_ProcPtr)(void*);
        BaseType*               m_ParamType;
    };

    friend class Node;
protected:
    std::vector<Property>       m_PropertiesList;
    std::map<unsigned int, unsigned int> m_PropertiesValues;
    unsigned int                field_1C;
    unsigned int                m_ScriptSize;
    std::vector<Method>         m_MethodsList;
    std::vector<Parameter>      m_ParametersList;
    unsigned int                m_PropertiesBlocksTotal;
    EntityType                 *m_BaseEntity;
    String                      m_Name;
    char                        field_5C;
    void                        (* field_60)(int*);

public:
    GlobalScript(const char* const scriptName, const char* const parentName, bool a3, bool a4);	//	@48A530

    void                        AddStructElement(unsigned int fieldId, const char* const defaultValue, unsigned int);	//	@48AF10
    void                        AddMethod(unsigned short methodid, void (*scriptthreadhandler)(class ScriptThread*), void (*methodptr)(int*));	//	@48A690
    void                        CalculateSize();	//	@48AA60
    bool						_48A7E0(Node* node, int scriptId, void* args);	//	@48A7E0
    void                        ClearEntityProperties(Entity* ent); //  @489C90
    class EntityType*           GetScriptEntity() const;  //  @489AE0

    class EntityType*           AssignScriptToEntity(const EntityType* parent);	//	@48A3F0

    static GlobalScript*        GetGlobalScriptByName(const char* name);	//	@48C590
    static GlobalScript*        GetGlobalScriptById(const unsigned int id);	//	@48C580
    static int                  GetScriptIdByName(const char* const name);	//	@48C910
    static void                 InstantiateGlobalScripts();  //  @48C960
    static void                 AssignCommonNodes();    //  @48C7D0

    static unsigned int			GetScriptIdByFullName(const char* const name);  //  NOTE: a special version of 'GetTypeByName' to be used when loading game-specific scripts. Not in original code.

    static std::vector<GlobalScript*>	ScriptsList;	//	@A0B424
    static std::vector<Node*>           SceneScriptEntitiesList;    //  @A3B5A4
};

extern BaseType*                GlobalScriptsArray[410];	//	@A3B7A4	//	TODO: this could be just a global 'scripts space' where mixed objects are contained.
extern Node*                    CacheScriptNode;    //  @A3B58C
extern Node*                    CommonScriptNode;   //  @A3B590
extern Node*                    CommonAnimSlotScriptNode;   //  @A3B594
extern Node*                    CommonTriggerScriptNode;    //  @A3B598
extern Node*                    IntroCommonScriptNode;  //  @A3B59C

ASSERT_CLASS_SIZE(GlobalScript, 100);

extern unsigned int	GlobalPropertyListChecksum;	//	@A3CF40
extern bool			GlobalPropertyListChecksumObtained;	//	@A3CF1C
extern unsigned int	GlobalCommandListChecksum;	//	@A3CF18
extern bool			GlobalCommandListChecksumObtained;	//	@A3CEF4

extern unsigned int GetGlobalPropertyListChecksum();	//	@873440
extern unsigned int GetGlobalCommandListChecksum();	//	@871DD0
extern unsigned int GetGlobalPropertyListSize();    //  @872FB0
extern unsigned int GetGlobalCommandListSize(); //  @871A20
extern int	GetPropertyIdByName(const char* const propertyname);	//	@8732C0
extern int	GetCommandByName_Impl(const char* const commandname);	//	@872360
extern int	GetCommandByName(const char* const commandname);	//	@872410
extern int	RegisterGlobalProperty(const char* const propertyname, bool existingProperty);	//	@8736B0
extern int	RegisterGlobalCommand(const char* const commandname, bool existingCommand);	//	@872590

extern void	ReadDatabaseFile(const char* path);	//	@48C400
extern void	LoadScripts();	//	@7A1F60
extern bool FindScript(const char* const scriptname, String& zipname);   //  @48C680

namespace Script
{
    static bool FileCheck = false;	//	@A35DE0
    static bool ForceFeedback = false;	//	@A35E70
    static bool SavePlatformPS2 = true;	//	@A090C8
    static unsigned int Ps2MaxTextureSize = 1024;	//	@A10FF0
    static const char* const CountryCodes[] = {
        "uk", "fr", "it", "de", "es", "dk"
    };	//	@A089BC
    static bool _A1B98D = true;	//	@A1B98D
    static bool CheckOriginalAsset = true;	//	@A11540
    static bool WarningShow = true;	//	@A082FE
    static bool FrameConsoleMarker = true;	//	@A1B98C
    static float MinFadeDist = 15.f;	//	@A11C8C
    static bool LodAndFade = true;	//	@A11C88
    static bool FixDanglingRefs = true;	//	@A1207D
    static float LodFactor = 1.0f;	//	@A12090
    static unsigned int LanguageStringsOffset = 0;	//	@A35E28
    static bool RelaxBuildVersionCheck;	//	@A5D5B0
    static bool CutsceneDisableAware;	//	@A3D892
    static bool CutsceneForceCompleteLodUpdates;	//	@A5D5A8
    static bool ShowHud;	//	@A3E0C8
    static bool CheckDataSanity;	//	@A5D5A9
    static bool CheckDivisionByZero;	//	@A5D5AA
    static bool Ps2PlayCtrl;	//	@A5D5B8
    static bool CheckDanglingRefs;	//	@A3DC4C
    static bool SimulateReleaseBuild;	//	@A3B584
    static float _A11C90 = 1.f;	//	@A11C90

    static bool LoadBlocks;
    static bool Fullscreen;
    static unsigned int IconResourceId;

    extern String Filesystem;
    extern String ControlType;
    extern String Region;
    extern String VersionName;	//	@A1B9C8
    extern String StreamedSoundExt;	//	@A35EE4
    extern String LanguageMode;	//	@A086A8
    extern String ScriptsPath;	//	@A0B434
    extern std::vector<StringTuple> DirectoryMappings;	//	@A35DE4

    static const char* GetCurrentCountryCode() { return CountryCodes[LanguageStringsOffset]; };	//	@42E500
    static void SetCountryCode(const char* const lang)	//	@42E530
    {
        unsigned int languageIndex = NULL;

        for (unsigned int i = 0; i < (sizeof(CountryCodes) / sizeof(char*)); i++)
            if (*CountryCodes[i] == *lang && CountryCodes[i][1] == lang[1])
                languageIndex = i;

        Script::LanguageStringsOffset = languageIndex;
    }
}
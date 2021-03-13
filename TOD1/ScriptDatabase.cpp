#include "ScriptDatabase.h"
#include "LogDump.h"
#include "Performance.h"
#include "Globals.h"
#include "ScriptTypes.h"
#include "Blocks.h"

std::vector<GlobalProperty>		GlobalPropertiesList;	//	@A3CF20
std::map<String, unsigned int>	GlobalPropertiesMap;	//	@A3CF30
std::vector<GlobalCommand>		GlobalCommandsList;	//	@A11470
std::map<String, unsigned int>	GlobalCommandsMap;	//	@A3CF08
std::vector<GlobalScript> GlobalScript::ScriptsList;
unsigned int	GlobalPropertyListChecksum;	//	@A3CF40
bool			GlobalPropertyListChecksumObtained;	//	@A3CF1C
unsigned int	GlobalCommandListChecksum;	//	@A3CF18
bool			GlobalCommandListChecksumObtained;	//	@A3CEF4

unsigned int GetGlobalPropertyListChecksum()
{
	if (GlobalPropertyListChecksumObtained)
		return GlobalPropertyListChecksum;

	String checksum_str;
	if (GlobalPropertiesList.size() > 0)
	{
		for (std::vector<GlobalProperty>::const_iterator it = GlobalPropertiesList.cbegin(); it != GlobalPropertiesList.cend(); ++it)
		{
			String tempstr;
			it->GetNameAndType(tempstr);

			if (checksum_str.m_nLength >= 10000)
				break;
			checksum_str.Append(tempstr.m_szString);
		}
	}

	GlobalPropertyListChecksum = Utils::CalcCRC32(checksum_str.m_szString, checksum_str.m_nLength);
	GlobalPropertyListChecksumObtained = true;
	return GlobalPropertyListChecksum;
}

unsigned int GetGlobalCommandListChecksum()
{
	if (GlobalCommandListChecksumObtained)
		return GlobalCommandListChecksum;

	String checksum_str;
	if (GlobalCommandsList.size() > 0)
	{
		for (std::vector<GlobalCommand>::iterator it = GlobalCommandsList.begin(); it != GlobalCommandsList.end(); ++it)
		{
			String tempstr;
			it->GetReturnTypeString(tempstr);

			checksum_str.Append(tempstr.m_szString);
		}
	}

	GlobalCommandListChecksum = Utils::CalcCRC32(checksum_str.m_szString, checksum_str.m_nLength);
	GlobalCommandListChecksumObtained = true;
	return GlobalCommandListChecksum;
}

int GetPropertyIdByName(const char* const propertyname)
{
	char propname[64] = {};
	strncpy_s(propname, sizeof(propname), propertyname, strchr(propertyname, ':') - propertyname);

	const std::map<String, unsigned int>::const_iterator& it = GlobalPropertiesMap.find(propname);
	if (it == GlobalPropertiesMap.cend())
		return -1;
	else
		return it->second;
}

int GetCommandByName_Impl(const char* const commandname)
{
	String cmdname = commandname;
	cmdname.ToLowerCase();

	const std::map<String, unsigned int>::const_iterator& it = GlobalCommandsMap.find(cmdname);
	if (it == GlobalCommandsMap.cend())
		return -1;
	else
		return it->second;
}

int GetCommandByName(const char* const commandname)
{
	const char* const ddotpos = strchr(commandname, ':');
	if (ddotpos)
	{
		char cmdname[256] = {};
		strncpy(cmdname, commandname, ddotpos - commandname);

		return GetCommandByName_Impl(cmdname);
	}
	else
		return GetCommandByName_Impl(commandname);
}

int RegisterGlobalProperty(const char* const propertyname, bool existingProperty)
{
	if (!GlobalPropertiesList.size())
		GlobalPropertiesList.reserve(6100);

	const char* ddotpos = strchr(propertyname, ':');
	if (!ddotpos)
	{
		LogDump::LogA("unable to register property '%s' - it does not specify a type", propertyname);
		return -1;
	}

	int propid = GetPropertyIdByName(propertyname);
	const char* proptype = ddotpos + 1;
	if (existingProperty && propid >= 0)
	{
		if (strncmp(GlobalPropertiesList[propid].m_PropertyType->m_TypeName.m_szString, proptype, strlen(proptype)) == NULL)
		{
#if defined INCLUDE_FIXES && defined VERBOSELOG
			LogDump::LogA("Property \"%s\" was already registered with id=%d!\n", propertyname, propid);
#endif
			return propid;
		}
		else
			return -1;
	}
	else
	{
		ScriptType::LoadScript(proptype);

		unsigned int propind = GlobalPropertiesList.size();
		GlobalPropertiesList.emplace_back(propertyname, propind);

		char propname[256] = {};
		strncpy(propname, propertyname, ddotpos - propertyname);
		String::ToLowerCase(propname);

		GlobalPropertiesMap[propname] = propind;
		GlobalPropertyListChecksumObtained = false;

#if defined INCLUDE_FIXES && defined VERBOSELOG
		LogDump::LogA("Property \"%s\" has been registered with id=%d\n", propertyname, propind);
#endif

		return propind;
	}
}

int RegisterGlobalCommand(const char* const commandname, bool existingCommand)
{
	if (!GlobalCommandsList.size())
		GlobalCommandsList.reserve(3000);

	int cmdid = GetCommandByName(commandname);
	const char* const ddotpos = strchr(commandname, ':');
	if (existingCommand && cmdid >= 0)
	{
		if (ddotpos)
			ScriptType::GetTypeByName(ddotpos + 1);

#if defined INCLUDE_FIXES && defined VERBOSELOG
		LogDump::LogA("Command \"%s\" already exists with id=%d!\n", commandname, cmdid);
#endif

		return cmdid;
	}
	else
	{
		int commandind = GlobalCommandsList.size();
		GlobalCommandsList.emplace_back(commandname, commandind);

		if (ddotpos)
		{
			char cmdname[256] = {};
			strncpy(cmdname, commandname, ddotpos - commandname);
			GlobalCommandsMap[cmdname] = commandind;
		}
		else
			GlobalCommandsMap[commandname] = commandind;

#if defined INCLUDE_FIXES && defined VERBOSELOG
		LogDump::LogA("Command \"%s\" has been registered with id=%d!\n", commandname, commandind);
#endif

		return commandind;
	}
}

void ReadDatabaseFile(const char* path)
{
	LogDump::LogA("Loading script database\n");
	File dbfile(path, 0x21, true);

	if (!dbfile.IsFileOpen())
		return;

	DWORD timeStart = Performance::GetMilliseconds();
	unsigned int	totalProperties = NULL;
	unsigned int	totalCommands = NULL;

	dbfile.Read(&totalProperties, sizeof(totalProperties));
	GlobalPropertiesList.reserve(totalProperties);

	for (unsigned int i = 0; i < totalProperties; i++)
	{
		char propertyName[128] = {};
		unsigned int propertyNameLen = NULL;

		dbfile.Read(&propertyNameLen, sizeof(propertyNameLen));
		dbfile.Read(propertyName, propertyNameLen);

		RegisterGlobalProperty(propertyName, false);
	}

	dbfile.Read(&totalCommands, sizeof(totalCommands));
	GlobalCommandsList.reserve(totalCommands);

	for (unsigned int i = 0; i < totalCommands; i++)
	{
		char commandName[128] = {};
		unsigned int commandNameLen = NULL;

		dbfile.Read(&commandNameLen, sizeof(commandNameLen));
		dbfile.Read(commandName, commandNameLen);

		RegisterGlobalCommand(commandName, false);
	}

	LogDump::LogA("Done loading script database (%dms)\n", Performance::GetMilliseconds() - timeStart);
#ifdef INCLUDE_FIXES
	LogDump::LogA("Read %d properties and %d commands\n", totalProperties, totalCommands);
#endif
}

#pragma message(TODO_IMPLEMENTATION)
void LoadScripts()
{
	//	TODO: initialize built-in structures here.

	if (GlobalPropertiesList.size() == SCRIPT_PROPERTIES_BUILTIN_TOTAL)
#ifdef INCLUDE_FIXES
		LogDump::LogA("%s built-in properties initialized.\n", GlobalCommandsList.size());
#else
		GlobalCommandsList.size();	//	TODO: why?
#endif

	if (GetGlobalPropertyListChecksum() == SCRIPT_PROPERTIES_BUILTIN_CRC)
		GetGlobalCommandListChecksum();

	String tmpstr;
	g_Blocks->GetPlatformSpecificPath(tmpstr, "/data/scripts/stable/Database.bin", "bin", ResType::PLATFORM_PC);
	ReadDatabaseFile(tmpstr.m_szString);

	if (GlobalPropertiesList.size() == SCRIPT_PROPERTIES_TOTAL)
#ifdef INCLUDE_FIXES
		LogDump::LogA("%s more properties loaded.\n", GlobalCommandsList.size());
#else
		GlobalCommandsList.size();	//	TODO: why?
#endif

	if (GetGlobalPropertyListChecksum() == SCRIPT_PROPERTIES_LOADED_CRC)
		GetGlobalCommandListChecksum();

	//	TODO: initialize built-in entities here.
}

void GlobalProperty::GetNameAndType(String& outStr) const
{
	if (m_PropertyType != tNOTHING)
	{
		char buf[256] = {};
		strncpy(buf, m_PropertyName, strlen(m_PropertyName));
		buf[strlen(m_PropertyName)] = ':';
		strcat(buf, m_PropertyType->m_TypeName.m_szString);

		outStr = buf;
		return;
	}
	else
		outStr = m_PropertyName;
}

GlobalProperty::GlobalProperty(const char* const propertyname, unsigned int ind)
{
	m_PropertyId = ind;

	size_t propnamestrlen = strchr(propertyname, ':') - propertyname;
	m_PropertyName = new char[propnamestrlen + 1];
	strncpy(m_PropertyName, propertyname, propnamestrlen);
	m_PropertyName[propnamestrlen] = NULL;

	m_PropertyType = ScriptType::GetTypeByName(propertyname + propnamestrlen + 1);
}

GlobalProperty::~GlobalProperty()
{
	if (m_PropertyName)
		delete[] m_PropertyName;
}

void GlobalCommand::AddArgumentType(const ScriptType* argtype)
{
	String emptystr;
	m_Arguments.m_ArgumentsList.emplace_back(emptystr, argtype, m_Arguments.m_TotalSizeBytes);
	m_Arguments.m_TotalSizeBytes += argtype->m_Size;
	m_Arguments.m_TotalSize += argtype->GetTypeSize();
}

void GlobalCommand::GetReturnTypeString(String& outStr)
{
	if (m_Arguments.m_ArgumentsList[0].m_ScriptType->m_Size)
	{
		char buf[128] = {};
		sprintf(buf, "%s:%s", m_ArgumentsString, m_Arguments.m_ArgumentsList[0].m_ScriptType->m_TypeName.m_szString);

		outStr = buf;
		return;
	}
	else
		outStr = m_ArgumentsString;
}

GlobalCommand::GlobalCommand(const char* const commandname, const unsigned int commandind)
{
	MESSAGE_CLASS_CREATED(GlobalCommand);

	char cmdname[256] = {};
	strcpy(cmdname, commandname);

	m_Arguments.m_TotalSize = NULL;
	m_Arguments.m_TotalSizeBytes = NULL;
	m_GlobalIndex = commandind;

	if (!commandname || !*commandname || !strchr(commandname, ':'))
	{
		AddArgumentType(tNOTHING);
	}
	else
	{
		AddArgumentType(ScriptType::GetTypeByName(strchr(commandname, ':') + 1));
		*strchr(cmdname, ':') = NULL;
	}

	char* const brcktopenpos = strchr(cmdname, '(');
	const char* const brcktclospos = strrchr(cmdname, ')');

	if (!brcktopenpos || !brcktclospos)
	{
		m_CommandName = _strdup(cmdname);
	}
	else
	{
		if (brcktopenpos + 1 <= brcktclospos)
		{
			char args[256] = {};
			strcpy(args, brcktopenpos + 1);
			*strrchr(args, ')') = NULL;

			char* tok = strtok(args, ",");
			while (tok)
			{
				ScriptType* argscripttype = ScriptType::LoadScript(tok);
#ifdef INCLUDE_FIXES
				//	NOTE: this should NOT happen!
				if (!argscripttype)
				{
					LogDump::LogA("*** Failed to load script \"%s\" ( command: \"%s\" ) ***\n", tok, commandname);
					AddArgumentType(tNOTHING);
				}
				else
#endif					
				AddArgumentType(argscripttype);
				tok = strtok(NULL, ",");
			}
		}

		*brcktopenpos = NULL;
		m_CommandName = _strdup(cmdname);
	}

	m_ArgumentsString = _strdup(cmdname);
}

GlobalCommand::~GlobalCommand()
{
	MESSAGE_CLASS_DESTROYED(GlobalCommand);

	delete m_ArgumentsString;
	delete m_CommandName;
}

#pragma message(TODO_IMPLEMENTATION)
class ScriptType_Entity* GlobalScript::AssignScriptToEntity(const ScriptType_Entity* parent)
{
	return nullptr;
}

GlobalScript* GlobalScript::GetGlobalScriptByName(const char* name)
{
	if (!name || !*name)
		return nullptr;

	if (!ScriptsList.size())
		return nullptr;

	const size_t namelen = strlen(name);
	for (unsigned int i = 0; i < ScriptsList.size(); ++i)
		if (strncmp(ScriptsList[i].m_Name.m_szString, name, namelen) == NULL)
			return &ScriptsList[i];

	return nullptr;
}

GlobalCommand::CommandArgument::CommandArgument(String& argname, const ScriptType* argtype, unsigned int argsize)
{
	m_PropertyName = argname;
	m_ScriptType = (ScriptType*)argtype;
	m_TotalSizeBytes = argsize;
}
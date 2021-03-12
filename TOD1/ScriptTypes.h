#pragma once
#include "StringsPool.h"
#include "Types.h"
#include <vector>
#include <map>

class ScriptType
{
public:
	enum ScriptTypeId
	{
		TYPE_NOTHING = 0,		//	NOTHING! Really!
		TYPE_NUMBER = 1,		//	FLOAT
		TYPE_INTEGER = 2,		//	INTEGER! Really!
		TYPE_STRING = 3,		//	LIBC STRING
		TYPE_TRUTH = 4,			//	BOOLEAN
		TYPE_VECTOR = 5,		//	VECTOR4
		TYPE_QUATERNION = 6,	//	QUATERNION
		TYPE_COLOR = 7,			//	COLOR RGBA
		TYPE_LIST = 8,			//	SIMPLE LIST
		TYPE_DICT = 9,			//	DICTIONARY. (HASHMAP)
		TYPE_ENTITY = 10,		//	SCRIPT ENTITY
		TYPE_STRUCT = 11,		//	STRUCTURE
		TYPE_LAST_DONTUSE
	};

	enum ScriptTypeSize
	{
		TYPE_NOTHING_SIZE = 0,
		TYPE_NUMBER_SIZE = 1,
		TYPE_INTEGER_SIZE = 1,
		TYPE_STRING_SIZE = 1,
		TYPE_TRUTH_SIZE = 1,
		TYPE_VECTOR_SIZE = 3,
		TYPE_QUATERNION_SIZE = 4,
		TYPE_COLOR_SIZE = 1,
		TYPE_LIST_SIZE = 1,
		TYPE_DICT_SIZE = 1,
		TYPE_ENTITY_SIZE = 1,
		TYPE_STRUCT_SIZE = 1
	};

public:
	String			m_TypeName;
	ScriptTypeId	m_TypeId;
	unsigned int	m_Size;
	unsigned int	m_GlobalId;

	virtual			~ScriptType();	//	@867A70
	virtual int		_489370(int* unk1, int* unk2);	//	@489370
	virtual void	GetDefaultValue(void* unk1);	//	NOTE: this pointer type is actually class-dependent. Returns zero (default value?).
	virtual void	nullsub_2(String* unk2);
	virtual void	_4893A0(int* unk1, int* unk2);	//	@4893A0
	virtual String* PrintFormatted(String* outStr, void* value, int precision);	//	@862F50	//	NOTE: lowest word is number of digits after dot, highest - total number of digits.
	virtual int		DoOperation(char* unk1, void* unk2);	//	@862AA0
	virtual int		_489410(int* unk1);	//	@489410
	virtual int		_863760_1(int* unk1, int* unk2);	//	@863760
	virtual int		_863760_2(int* unk1, int* unk2);	//	@863760
	virtual int		_8637F0(int* unk1, String* unk2, int unk3);	//	@8637F0
	virtual int		_862A50(int unk1, int* unk2, int* unk3);	//	@862A50
	virtual void	nullsub_3(int unk1, void* unk2, int unk3, int unk4, int unk5, void* outop);
	virtual void	nullsub_4(int* unk1, int unk2, int unk3, int unk4, int unk5, int unk6);
	virtual bool	AreParamsEqual(int unk1, int unk2);	//	@7A1F00
	virtual bool	AreParamsNotEqual(int unk1, int unk2);	//	@862AB0
	virtual void	GetOperationType(const char* operationStr, int* outOperationId, ScriptType** outResultType, char* unk4);	//	@8637D0
	virtual void	PerformOperation(int operationId, void* params);	//	@8C4D60	//	NOTE: get operation id by calling 'GetOperationType'. Params is of type of the class, zero element stores result of operation.
	virtual char	_8637E0(int unk1, int unk2);	//	@8637E0
	virtual bool	IsInfinite(void* unk1);	//	@489440

private:
	void			RemoveTypeFromList(const char* name);	//	@862B50
	
	static unsigned int	GetTypeSize_Impl(const ScriptType* type);	//	@862AC0

public:
	ScriptType(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);	//	@862E90
	ScriptType(const ScriptType&);

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete (void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
		ptr = nullptr;
	}

	unsigned int	GetTypeSize() const;	//	@862B20

	static ScriptType*	GetTypeByName(const char* name);	//	@862C00
	static ScriptType*	LoadScript(const char* script);	//	@863070
	static bool			ParseVariableString(const char* variable, String& variableName, String& variableType);	//	@862F70
};

ASSERT_CLASS_SIZE(ScriptType, 32);

class ScriptType_Nothing : public ScriptType
{
public:
	ScriptType_Nothing(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete (void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
		ptr = nullptr;
	}
};

class ScriptType_Number : public ScriptType
{
public:
	ScriptType_Number(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete (void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
		ptr = nullptr;
	}
};

class ScriptType_Integer : public ScriptType
{
public:
	ScriptType_Integer(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete (void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
		ptr = nullptr;
	}
};

class ScriptType_Boolean : public ScriptType
{
public:
	ScriptType_Boolean(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete (void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
		ptr = nullptr;
	}
};

class ScriptType_Vector : public ScriptType
{
public:
	ScriptType_Vector(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete (void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
		ptr = nullptr;
	}
};

class ScriptType_Quaternion : public ScriptType
{
public:
	ScriptType_Quaternion(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete (void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
		ptr = nullptr;
	}
};

class ScriptType_Color : public ScriptType
{
public:
	ScriptType_Color(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete (void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
		ptr = nullptr;
	}
};

class ScriptType_String : public ScriptType
{
public:
	ScriptType_String(ScriptTypeId typeId, const char* const typeName, ScriptTypeSize typeSize);

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete (void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
		ptr = nullptr;
	}
};

//	TODO: rename to 'EntityType' as in original code.
class ScriptType_Entity : public ScriptType
{
	struct ScriptFuncDesc
	{
		void			(*m_ScriptProcPtr)(class Entity*, void* params);
		int				field_4;
		int				field_8;
		int				field_C;

		ScriptFuncDesc() {};
		ScriptFuncDesc(const void* const scriptprocptr, int a2, int a3, int a4);
	};

	struct ScriptMethodDesc
	{
		ScriptType*		m_ReturnType;
		int*			field_4;
		void*			(*m_Getter)();
		unsigned int	field_C;
		unsigned int	field_10;
		unsigned int	field_14;
		void			(*m_Setter)(void*);
		unsigned int	field_1C;
		unsigned int	field_20;
		int				field_24;
		unsigned int	field_28;	//	NOTE: some flags.
		unsigned int	field_2C;

		ScriptMethodDesc() {};

		ScriptMethodDesc(const ScriptType* rettype,
			int* a2,
			const void* const getterprocptr,
			unsigned int a4,
			unsigned int a5,
			unsigned int a6,
			const void* const setterprocptr,
			unsigned int a8,
			unsigned int a9,
			int a10,
			unsigned int a11,
			unsigned int a12);
	};

public:
	typedef void* (*CREATOR)(AllocatorIndex);

	CREATOR										m_Creator;
	ScriptType_Entity*							m_Parent;
	int*										m_Script;
protected:
	std::map<unsigned short, ScriptFuncDesc>	m_ScriptsList;	//	NOTE: each 'derived' script derives it's parent scripts.
	int											field_38;
	std::map<unsigned short, unsigned short>	field_3C;	//	NOTE: could be list with properties id's and their values.
	int											field_48;
	std::vector<ScriptMethodDesc>				m_PropertiesList;	//	NOTE: could be list with methods for THIS exact class only.
	std::vector<ScriptMethodDesc>				m_PropertiesList_1;	//	NOTE: another list same as above, but there are more methods in this list.
	int											field_6C;
	int											field_70;
	bool										m_HasParent;
public:
	ScriptType_Entity(const char* szEntityName);	//	@86CC00
	~ScriptType_Entity();

	void*	CreateNode();	//	@86C770
	void	InheritFrom(ScriptType_Entity* from);	//	@86CB40
	void	SetCreator(CREATOR creator)
	{
		m_Creator = creator;
	}
	void	RegisterScript(const char* const scriptname,
		const void* const scriptprocptr,
		const int a3,
		const int a4,
		const int a5,
		const char* const editorcontrolstr,
		const char* const a7);	//	@86EC70
	void	RegisterProperty(const ScriptType* returntype,
		const char* const propertyname,
		const void* getterprocptr,
		const int a4,
		const int a5,
		const int a6,
		const void* setterprocptr,
		const int a8,
		const int a9,
		const int a10,
		const char* const a11,
		const int a12,
		const int a13,
		const int argumentstotal);	//	@86D370
	void	_86E9B0();	//	@86E9B0	//	NOTE: probably, this propagates all methods and properties from parent entity.

	static ScriptType_Entity* GetScriptEntityByName(const char* name);	//	@862C70
};

ASSERT_CLASS_SIZE(ScriptType_Entity, 120);

struct ScriptField
{
	friend struct ScriptFieldsList;
	friend class ScriptType_Script;
protected:
	String			m_Name;
	ScriptType* m_Type;
	unsigned int	m_FieldOffset;

public:
	ScriptField()
		: m_Name({}), m_Type(nullptr), m_FieldOffset(NULL)
	{};

	ScriptField(const char* name, ScriptType* stype, unsigned int);
	ScriptField(const ScriptField& rhs);	//	@7A1D40
};

struct ScriptFieldsList
{
	friend class ScriptType_Script;
protected:
	ScriptField* m_Elements;
	unsigned int	m_CurrentIndex;
	unsigned int	m_Capacity;
	unsigned int	m_Flags;

	unsigned int	m_TotalSizeBytes;
	unsigned int	m_TotalSize;

public:
	ScriptFieldsList()
		: m_Elements(nullptr), m_CurrentIndex(NULL), m_Capacity(NULL), m_Flags(NULL), m_TotalSizeBytes(NULL), m_TotalSize(NULL)
	{};

	ScriptFieldsList(unsigned int flags);	//	NOTE: inlined in code.
	ScriptFieldsList(const ScriptFieldsList& rhs);	//	@7A1D90

	void			Add(const ScriptField&);	//	@862550

	void			Clear();	//	@7A1CE0
};

//	NOTE: this is essentially a struct.
class ScriptType_Script : public ScriptType
{
protected:
	String			m_Name;
	ScriptFieldsList m_Fields;

public:
	ScriptType_Script(const char* name, const ScriptFieldsList& fields);	//	@7A1E90
};

class ScriptType_List : public ScriptType
{
protected:
	ScriptType* m_ListElementsType;
	int m_ListElementSize;
	char m_IsTypeId3_8_9_11;

public:
	ScriptType_List(const ScriptType& elementsType);	//	@870630
	~ScriptType_List()
	{
		MESSAGE_CLASS_DESTROYED(ScriptType_List);
	}
};

class ScriptType_Dict : public ScriptType
{
protected:
	ScriptType* m_ElementsType;

public:
	ScriptType_Dict(const ScriptType& elementsType);	//	@867B50
	~ScriptType_Dict()
	{
		MESSAGE_CLASS_DESTROYED(ScriptType_Dict);
	}
};

//	NOTE: DumpTable class.
class DumpTable_Column
{
public:
	virtual ~DumpTable_Column();
	DumpTable_Column();
};

class DumpTable_IntegerColumn : public DumpTable_Column
{
protected:
	unsigned int	field_4;
	char			field_8;
	String			m_ColumnName;
};

struct DumpTable_Element
{
	friend class ScriptType_Builtin;
protected:
	std::vector<DumpTable_Column>	m_Columns;
	int				field_C;
	unsigned int	m_NumRows;

public:
	DumpTable_Element();	//	@4014E0
	DumpTable_Element(const char* const s);	//	@401E40
};

//	NOTE: Builtin-related classes.
struct BuiltinMember
{
	ScriptType*	m_RetType;
	void*		(*m_GetProcPtr)(void);
	void*		(*m_SetProcPtr)(void);
	String		m_Proto;
	String		m_Str_2;
};

struct BuiltinHandler
{
protected:
	String		m_Prototype;
	void*		(*m_Handler)(void*);
	String		m_Name;

public:
	inline BuiltinHandler(const char* _prot, void* (*_hndlr)(void*), const char* _name);
	BuiltinHandler() {};
};

class ScriptType_Builtin : public ScriptType_Entity
{
private:
	ScriptType_Builtin();	//	@486D00
	
	std::vector<BuiltinHandler>	m_HandlersList;
	int							field_C;
	std::map<int, int>			m_MembersMap;
	int							field_1C;

	void	RegisterMemberFunction(ScriptType* _rettype, const char* _membname, void* (*_getproc)(), void (*_setproc)(int), const char* _membproto, const char* _unk);	//	@486D90
	void	RegisterHandler(const char* _hsignature, void*, const char* _hmsg);	//	@486430

public:
	void	Sin(float*);	//	@487C10
	void	Cos(float*);	//	@487C30
	void	Tan(float*);	//	@487C50
	void	Asin(float*);	//	@487C70
	void	Acos(float*);	//	@487C90
	void	Atan(float*);	//	@487CB0
	void	Abs(int*);	//	@487CE0
	void	Fabs(float*);	//	@487CD0
	void	Sqrt(float*);	//	@487D20
	void	Floor(float*);	//	@487D00
	void	Ceil(float*);	//	@487D30
	void	Clamp(float*);	//	@487D50
	void	Testbits(int*);	//	@487D90
	void	Setbit(int*);	//	@487DB0
	void	Getbit(int*);	//	@487DD0
	void	Rand_seed(int*);	//	@484F70
	void	Rand_integer(int*);	//	@484F80
	void	Rand_number(float*);	//	@484FA0
	void	Get_facecoll_MaterialID(int*);	//	@485D30
	void	GetTime(float*);	//	@487DF0
	void	Print(int*);	//	@484FB0
	void	IsKeyDown(int*);	//	@485210
	void	IsKeyPressed(int*);	//	@485250
	void	IsKeyReleased(int*);	//	@485280
	void	DrawPoint(int*);	//	@4855F0
	void	DrawLine(int*);	//	@4855B0
	void	DrawLine2D(int*);	//	@4855C0
	void	DrawSphere(int*);	//	@485600
	void	ProfileBegin(int*);	//	@484F50
	void	ProfileEnd(int*);	//	@484F60
	void	NumberToInteger(void*);	//	@487E30
	void	IntegerToNumber(void*);	//	@487E20
	void	PrintStack(int*);	//	@4852B0
	void	GenericCall(int*);	//	@485300
	void	QuadTreeQuery(int*);	//	@486950
	void	AuxQuadTreeQuery(int*);	//	@4865A0
	void	SetSelectedSoundrenderer(int*);	//	@484FE0
	void	SfxMuteAll(int*);	//	@485070
	void	SfxIsMuteAll(int*);	//	@485090
	void	AllocateGlobalStreamedSound(int*);	//	@4850B0
	void	DeallocateGlobalStreamedSound(int*);	//	@4850E0
	void	PlayGlobalStreamedSound(int*);	//	@4850F0
	void	StopGlobalStreamedSound(int*);	//	@485110
	void	SetVolumePitchGlobalStreamedSound(int*);	//	@485130
	void	GetDefaultFxVolumeVar(float*);	//	@485150
	void	GetDefaultAmbienceVolumeVar(float*);	//	@485170
	void	GetDefaultMusicVolumeVar(float*);	//	@485190
	void	GetDefaultSpeaksVolumeVar(float*);	//	@4851B0
	void	SetVolModifierOnGroup(int*);	//	@485870
	void	CutsceneDisableAware(int*);	//	@4851D0
	void	IsDebugConsoleActive(int*);	//	@4851E0
	void	DebugConsoleTextBox(int*);	//	@4851F0
	void	DebugConsolePrint(int*);	//	@485200
	void	GlobalKillAllEmmiters(int*);	//	@484FD0
	void	GetVersionNumber(char**);	//	@4856F0
	void	GetConfigString(int*);	//	@485770
	void	GetConfigTruth(int*);	//	@485310
	void	GetSessionVariableString(int*);	//	@4857F0
	void	SetSessionVariableString(int*);	//	@485390
	void	GetSessionVariableTruth(char*);	//	@485350
	void	SetSessionVariableTruth(int*);	//	@4853B0
	void	SetCurrentCountryCode(char*);	//	@487E10
	void	GetCurrentCountryCode(char*);	//	@4882F0
	void	SetDiscErrorMessage(int*);	//	@485610
	void	SetLodFactor(float*);	//	@4852E0
	void	LoadScene(char**);	//	@485CD0
	void	GetSystemLanguage(int*);	//	@4855D0
	void	StartCleanupDashboard(int*);	//	@4853E0
	void	SetScreenResolution(int*);	//	@4853F0
	void	GetScreenResolution(float*);	//	@485420
	void	IsScreenResolutionAvailable(int*);	//	@485460
	void	IsWideScreen(int*);	//	@485640
	void	SetVirtualHudScreenSize(float*);	//	@485480
	void	GetVirtualHudScreenSize(float*);	//	@4854C0
	void	GetScreenTopInVirtualUnits(float*);	//	@485500
	void	GetScreenBottomInVirtualUnits(float*);	//	@485520
	void	GetScreenLeftInVirtualUnits(float*);	//	@485540
	void	GetScreenRightInVirtualUnits(float*);	//	@485560
	void	DisableCurrentLoadScreen(int*);	//	@485580
	void	GetEditorActive(bool*);	//	@4853D0
	void	DumptableCreate(int*);	//	@486200
	void	DumptableCreateFromFile(int*);	//	@4862C0
	void	DumptableAddIntegerColumn(int*);	//	@4858C0
	void	DumptableAddNumberColumn(int*);	//	@4858F0
	void	DumptableAddStringColumn(int*);	//	@485920
	void	DumptableSetNumRows(int*);	//	@485950
	void	DumptableSetIntegerValue(int*);	//	@485970
	void	DumptableSetNumberValue(float*);	//	@4859A0
	void	DumptableSetStringValue(int*);	//	@4859D0
	void	DumptableWriteToFile(int*);	//	@485A00
	void	DumptableClose(int*);	//	@485A90
	void	EditorReloadAllAssets(int*);	//	@485590
	void	EditorSelectNode(int*);	//	@4855A0
	void	GetRegion(int*);	//	@485620
	void	GetMessageId(int*);	//	@488040
	void	QuitGame(int*);	//	@485660
	void	GetCoverdemoPlayMode(int*);	//	@485670
	void	GetCoverdemoInactiveTimeoutSec(int*);	//	@485690
	void	GetCoverdemoGameplayTimeoutSec(int*);	//	@4856B0
	void	CoverdemoExit(int*);	//	@4856D0

	static void	Register();	//	@486F30

	static const Vector4f		ZeroVector;	//	@9B7084
	static const Vector4f		RightVector;	//	@9B70A4
	static const Vector4f		UpVector;	//	@9B70B4
	static const Vector4f		InVector;	//	@9B70C4
	static const Vector4f		LeftVector;	//	@9B70D4
	static const Vector4f		DownVector;	//	@9B70F4
	static const Vector4f		OutVector;	//	@9B7104
	static const ColorRGB		ColorBlack;	//	@9B7114
	static const ColorRGB		ColorRed;	//	@9B7124
	static const ColorRGB		ColorGreen;	//	@9B7134
	static const ColorRGB		ColorYellow;	//	@9B7144
	static const ColorRGB		ColorDarkBlue;	//	@9B7154
	static const ColorRGB		ColorPink;	//	@9B7164
	static const ColorRGB		ColorBlue;	//	@9B7174
	static const ColorRGB		ColorWhite;	//	@9B7184
};

extern std::vector<DumpTable_Element>	DumpTable;	//	@A0B3A4
extern ScriptType_Builtin* tBuiltin;	//	@A3B578

extern ScriptType_Nothing* tNOTHING;	//	@A3CE94
extern ScriptType_Number* tNUMBER;	//	@A3CEC0
extern ScriptType_Integer* tINTEGER;	//	@A3CEB8
extern ScriptType_Boolean* tBOOLEAN;	//	@A3CEC4
extern ScriptType_Vector* tVECTOR;	//	@A3CEB4
extern ScriptType_Quaternion* tQUATERNION;	//	@A3CE98
extern ScriptType_Color* tCOLOR;	//	@A3CEA4
extern ScriptType_String* tSTRING;	//	@A3CEB0

void	InitScriptTypes();	//	@8634E0
unsigned int GetTypesChecksum();	//	@862CF0

static bool TypesListCRCCalculated;		//	@A3CEC8
static std::vector<ScriptType*> TypesList;		//	@A3CECC
static int TypesListCRC;				//	@A3CEDC
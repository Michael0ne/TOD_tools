#pragma once

#include "Types.h"
#include "List.h"
#include "Dictionary.h"

namespace ScriptTypes
{
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
		TYPE_SCRIPT = 11,		//	SCRIPT WITH PARAMETERS (Struct essentially)
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
		TYPE_SCRIPT_SIZE = 1
	};

	static const char* szScriptTypeName[TYPE_LAST_DONTUSE] = {
		"nothing",
		"number",
		"integer",
		"string",
		"truth",
		"vector",
		"quaternion",
		"color",
		"list",
		"dict",
		NULL,	//	NOTE: no name because each structure being created provides it's own name.
		NULL
	};

	#define SCRIPT_TYPE_CLASS_SIZE 32

	//	NOTE: THIS is base class for all script entities.
	class ScriptType
	{
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
		virtual String*	PrintFormatted(String* outStr, void* value, int precision);	//	@862F50	//	NOTE: lowest word is number of digits after dot, highest - total number of digits.
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

	public:
		ScriptType();
		ScriptType(ScriptTypeId typeId, const char* typeName, ScriptTypeSize typeSize);	//	@862E90
		ScriptType(const ScriptType&);

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete (void* ptr)
		{
			if (ptr)
				Allocators::ReleaseMemory(ptr, 0);
		}
	};

	static int GetTypeSize(ScriptType* type);	//	@862AC0
	static class ScriptType_Entity* GetScriptEntityByName(const char* name);	//	@862C70

	class ScriptType_Nothing : public ScriptType
	{
	private:

	public:
		ScriptType_Nothing(ScriptTypeId typeId, const char* typeName, ScriptTypeSize typeSize) : ScriptType(typeId, typeName, typeSize)
		{
			MESSAGE_CLASS_CREATED(ScriptType_Nothing);
		}

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete (void* ptr)
		{
			if (ptr)
				Allocators::ReleaseMemory(ptr, 0);
		}
	};

	static ScriptType_Nothing*			tyNothing = (ScriptType_Nothing*)0xA3CE88;	//	@A3CE88

	class ScriptType_Number : public ScriptType
	{
	private:

	public:
		ScriptType_Number(ScriptTypeId typeId, const char* typeName, ScriptTypeSize typeSize) : ScriptType(typeId, typeName, typeSize)
		{
			MESSAGE_CLASS_CREATED(ScriptType_Number);
		}

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete (void* ptr)
		{
			if (ptr)
				Allocators::ReleaseMemory(ptr, 0);
		}
	};

	static ScriptType_Number*			tyNumber = (ScriptType_Number*)0xA3CEA8;	//	@A3CEA8

	class ScriptType_Integer : public ScriptType
	{
	private:

	public:
		ScriptType_Integer(ScriptTypeId typeId, const char* typeName, ScriptTypeSize typeSize) : ScriptType(typeId, typeName, typeSize)
		{
			MESSAGE_CLASS_CREATED(ScriptType_Integer);
		}

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete (void* ptr)
		{
			if (ptr)
				Allocators::ReleaseMemory(ptr, 0);
		}
	};

	static ScriptType_Integer*			tyInteger = (ScriptType_Integer*)0xA3CE8C;	//	@A3CE8C

	class ScriptType_Boolean : public ScriptType
	{
	private:

	public:
		ScriptType_Boolean(ScriptTypeId typeId, const char* typeName, ScriptTypeSize typeSize) : ScriptType(typeId, typeName, typeSize)
		{
			MESSAGE_CLASS_CREATED(ScriptType_Boolean);
		}

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete (void* ptr)
		{
			if (ptr)
				Allocators::ReleaseMemory(ptr, 0);
		}
	};

	static ScriptType_Boolean*			tyBoolean = (ScriptType_Boolean*)0xA3CEBC;	//	@A3CEBC

	class ScriptType_Vector : public ScriptType
	{
	private:

	public:
		ScriptType_Vector(ScriptTypeId typeId, const char* typeName, ScriptTypeSize typeSize) : ScriptType(typeId, typeName, typeSize)
		{
			MESSAGE_CLASS_CREATED(ScriptType_Vector);
		}

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete (void* ptr)
		{
			if (ptr)
				Allocators::ReleaseMemory(ptr, 0);
		}
	};

	static ScriptType_Vector*			tyVector = (ScriptType_Vector*)0xA3CEAC;	//	@A3CEAC

	class ScriptType_Quaternion : public ScriptType
	{
	private:

	public:
		ScriptType_Quaternion(ScriptTypeId typeId, const char* typeName, ScriptTypeSize typeSize) : ScriptType(typeId, typeName, typeSize)
		{
			MESSAGE_CLASS_CREATED(ScriptType_Quaternion);
		}

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete (void* ptr)
		{
			if (ptr)
				Allocators::ReleaseMemory(ptr, 0);
		}
	};

	static ScriptType_Quaternion*		tyQuaternion = (ScriptType_Quaternion*)0xA3CE90;	//	@A3CE90

	class ScriptType_Color : public ScriptType
	{
	private:

	public:
		ScriptType_Color(ScriptTypeId typeId, const char* typeName, ScriptTypeSize typeSize) : ScriptType(typeId, typeName, typeSize)
		{
			MESSAGE_CLASS_CREATED(ScriptType_Color);
		}

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete (void* ptr)
		{
			if (ptr)
				Allocators::ReleaseMemory(ptr, 0);
		}
	};

	static ScriptType_Color*			tyColor = (ScriptType_Color*)0xA3CE9C;		//	@A3CE9C

	class ScriptType_String : public ScriptType
	{
	private:

	public:
		ScriptType_String(ScriptTypeId typeId, const char* typeName, ScriptTypeSize typeSize) : ScriptType(typeId, typeName, typeSize)
		{
			MESSAGE_CLASS_CREATED(ScriptType_String);
		}

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete (void* ptr)
		{
			if (ptr)
				Allocators::ReleaseMemory(ptr, 0);
		}
	};

	static ScriptType_String*			tyString = (ScriptType_String*)0xA3CEA0;	//	@A3CEA0

	struct EntityProperties
	{
	private:
		ScriptTypes::ScriptType* m_ReturnType;
		int field_4;
		void* m_Getter;
		int field_C;
		int field_10;
		int field_14;
		void* m_Setter;
		int field_1C;
		int field_20;
		int field_24;
		int field_28;
		int field_2C;
	};

	#define SCRIPT_TYPE_ENTITY_CLASS_SIZE 120

	class ScriptType_Entity : public ScriptType
	{
	protected:
		void* (*m_Creator)(AllocatorIndex allocatorIndex);
	public:
		ScriptType_Entity* m_Parent;
		int* m_Script;
	protected:
		//	NOTE: it looks like 2 members below are Map's with key being the name of the script/property, value being index to lists with actual information for script/property.
		int m_Properties[4];
		int m_Properties_1[4];
		List<EntityProperties> m_ScriptsList;
		List<EntityProperties> m_PropertiesList;
		int field_6C;
		int field_70;
		bool	m_HasParent;
	public:
		ScriptType_Entity(const char* szEntityName);	//	@86CC00
		~ScriptType_Entity()
		{
			MESSAGE_CLASS_DESTROYED(ScriptType_Entity);
		}

		void*	CreateNode();	//	@86C770
		void	InheritFrom(ScriptType_Entity* from);	//	@86CB40
	};

	struct ScriptField
	{
		friend struct ScriptFieldsList;
		friend class ScriptType_Script;
	protected:
		String			m_Name;
		ScriptType*		m_Type;
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
		ScriptField*	m_Elements;
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

	static ScriptType_Entity*			tENTITY = (ScriptType_Entity*)0xA3CEE0;		//	@A3CEE0

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

	struct BuiltinMember
	{
	protected:
		ScriptType* m_ReturnType;
		void*		(*m_GetProcPtr)();
		void		(*m_SetProcPtr)();
		String		m_MemberProto;
		String		m_String_1;
	};

	struct BuiltinHandler
	{
	protected:
		String		m_Prototype;
		void*		(*m_Handler)(void*);
		String		m_Name;

	public:
		inline BuiltinHandler(const char* _prot, void* (*_hndlr)(void*), const char* _name);
	};

	class ScriptType_Builtin : public ScriptType
	{
	protected:
		List<BuiltinHandler>	m_HandlersList;
		Dictionary<String, BuiltinMember>	m_MembersMap;

		void	RegisterMember(ScriptType* _rettype, const char* _membname, void* (*_getproc)(), void (*_setproc)(int), const char* _membproto, const char* _unk);	//	@486D90
		void	RegisterHandler(const char* _hsignature, void* (*_hndlr)(void*), const char* _hmsg);	//	@486430
	};

	static ScriptType_Nothing*			tNOTHING = (ScriptType_Nothing*)0xA3CE94;	//	@A3CE94
	static ScriptType_Number*			tNUMBER = (ScriptType_Number*)0xA3CEC0;		//	@A3CEC0
	static ScriptType_Integer*			tINTEGER = (ScriptType_Integer*)0xA3CEB8;	//	@A3CEB8
	static ScriptType_Boolean*			tBOOLEAN = (ScriptType_Boolean*)0xA3CEC4;	//	@A3CEC4
	static ScriptType_Vector*			tVECTOR = (ScriptType_Vector*)0xA3CEB4;		//	@A3CEB4
	static ScriptType_Quaternion*		tQUATERNION = (ScriptType_Quaternion*)0xA3CE98;	//	@A3CE98
	static ScriptType_Color*			tCOLOR = (ScriptType_Color*)0xA3CEA4;		//	@A3CEA4
	static ScriptType_String*			tSTRING = (ScriptType_String*)0xA3CEB0;		//	@A3CEB0

	void						Init();	//	@8634E0

	static bool&				TypesListCRCCalculated = *(bool*)0xA3CEC8;	//	@A3CEC8
	static List<ScriptType>&	TypesList = *(List<ScriptType>*)0xA3CECC;	//	@A3CECC
	static int&					TypesListCRC = *(int*)0xA3CEDC;				//	@A3CEDC
}

static_assert(sizeof(ScriptTypes::ScriptType) == SCRIPT_TYPE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(ScriptTypes::ScriptType));
static_assert(sizeof(ScriptTypes::ScriptType_Entity) == SCRIPT_TYPE_ENTITY_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(ScriptTypes::ScriptType_Entity));
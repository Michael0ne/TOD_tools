#pragma once
#include "BaseType.h"
#include "ScriptDatabase.h"

class EntityType : public BaseType
{
	struct ScriptInfo
	{
		void            (__thiscall* m_ScriptPtr)(void*, void*) = nullptr;
		int             field_4 = NULL;
		int             field_8 = NULL;
		int             field_C = NULL;
	};

	struct PropertyInfo
	{
		BaseType       *m_ReturnType = nullptr;
		int            *field_4 = nullptr;
		void*           (__thiscall *m_GetterPtr)(void*) = nullptr;
		unsigned int    field_C = NULL;
		unsigned int    field_10 = NULL;
		unsigned int    field_14 = NULL;
		void            (__thiscall *m_SetterPtr)(void*, void*) = nullptr;
		unsigned int    field_1C = NULL;
		unsigned int    field_20 = NULL;
		int             field_24 = NULL;
		union
		{
			struct
			{
				unsigned GlobalIndex : 15;
				unsigned _16 : 1;
				unsigned _17 : 1;
				unsigned _18 : 1;
				unsigned _19 : 1;
				unsigned _20 : 1;
				unsigned _21 : 1;
				unsigned _22 : 1;
				unsigned _23 : 1;
				unsigned _24 : 1;
				unsigned _25 : 1;
				unsigned _26 : 1;
				unsigned _27 : 1;
				unsigned _28 : 1;
				unsigned _29 : 1;
				unsigned _30 : 1;
			}           m_FlagsBits;
			unsigned int m_Flags;
		}               m_Flags;
		unsigned int    field_2C = NULL;
	};

public:
	typedef void* (*CREATOR)(AllocatorIndex);

	CREATOR             m_Creator;
	EntityType         *m_Parent;
	GlobalScript       *m_Script;
protected:
	std::map<unsigned short, ScriptInfo> m_ScriptsList;	//	NOTE: each 'derived' script derives it's parent scripts.
	int                 field_38;
	std::map<unsigned short, unsigned short> field_3C;	//	NOTE: could be list with properties id's and their values.
	int                 field_48;
	std::vector<PropertyInfo> m_PropertiesList;	//	NOTE: could be list with methods for THIS exact class only.
	std::vector<PropertyInfo> m_PropertiesList_1;	//	NOTE: another list same as above, but there are more methods in this list.
	int                 field_6C;
	int                 field_70;
	bool                m_HasParent;

public:
	EntityType(const char* const entityname);	//	@86CC00
	virtual ~EntityType();	//	@4886C0

	void*               CreateNode() const;	//	@86C770
	void                InheritFrom(const EntityType* from);	//	@86CB40
	void                SetCreator(CREATOR creator);
						template <typename T>
	void				RegisterScript(const char* const scriptname, T scriptptr, const int a3, const int a4, const int a5, const char* const editorcontrolstr, const char* const a7);	//	@86EC70
						template <typename T, typename T1>
	void				RegisterProperty(const BaseType* returntype, const char* const propertyname,T getterptr, const int a4, const int a5, const int a6, T1 setterptr, const int a8, const int a9, const int a10, const char* const a11, const int a12, const int a13, const int propertyind);	//	@86D370
	void				_86E9B0();	//	@86E9B0	//	NOTE: probably, this propagates all methods and properties from parent entity.
};

ASSERT_CLASS_SIZE(EntityType, 120);
#pragma once
#include "BaseType.h"

class EntityType : public BaseType
{
	struct ScriptFuncDescription
	{
		void			(*m_ScriptProcPtr)(class Entity*, void* params) = nullptr;
		int				field_4 = NULL;
		int				field_8 = NULL;
		int				field_C = NULL;
	};

	struct ScriptMethodDescription
	{
		BaseType* m_ReturnType = nullptr;
		int* field_4 = nullptr;
		void*			(*m_Getter)(void) = nullptr;
		unsigned int	field_C = NULL;
		unsigned int	field_10 = NULL;
		unsigned int	field_14 = NULL;
		void			(*m_Setter)(void*) = nullptr;
		unsigned int	field_1C = NULL;
		unsigned int	field_20 = NULL;
		int				field_24 = NULL;
		unsigned int	field_28 = NULL;	//	NOTE: some flags.
		unsigned int	field_2C = NULL;
	};

public:
	typedef void* (*CREATOR)(AllocatorIndex);

	CREATOR				m_Creator;
	EntityType*			m_Parent;
	int*				m_Script;
protected:
	std::map<unsigned short, ScriptFuncDescription>	m_ScriptsList;	//	NOTE: each 'derived' script derives it's parent scripts.
	int					field_38;
	std::map<unsigned short, unsigned short>	field_3C;	//	NOTE: could be list with properties id's and their values.
	int					field_48;
	std::vector<ScriptMethodDescription>	m_PropertiesList;	//	NOTE: could be list with methods for THIS exact class only.
	std::vector<ScriptMethodDescription>	m_PropertiesList_1;	//	NOTE: another list same as above, but there are more methods in this list.
	int					field_6C;
	int					field_70;
	bool				m_HasParent;

public:
	EntityType(const char* const entityname);	//	@86CC00
	virtual ~EntityType();	//	@4886C0

	void*				CreateNode() const;	//	@86C770
	void				InheritFrom(EntityType* from);	//	@86CB40
	inline void			SetCreator(CREATOR creator) { m_Creator = creator; }
	template <class C>
	void				RegisterScript(const char* const scriptname, void (C::* scriptproc)(void*), const int a3, const int a4, const int a5, const char* const editorcontrolstr, const char* const a7);	//	@86EC70
	template <class C>
	void				RegisterProperty(const BaseType* returntype, const char* const propertyname, void* (C::* getter)(int) const, const int a4, const int a5, const int a6, void (C::* setter)(int), const int a8, const int a9, const int a10, const char* const a11, const int a12, const int a13, const int argumentstotal);	//	@86D370
	void				_86E9B0();	//	@86E9B0	//	NOTE: probably, this propagates all methods and properties from parent entity.
};

ASSERT_CLASS_SIZE(EntityType, 120);
#pragma once
#include "ScriptDatabase.h"

class IScriptThread
{
public:
	virtual						~IScriptThread();
	virtual int					stub1(void*);
};

class ScriptThread : public IScriptThread
{
	struct ThreadList_1
	{
		unsigned int			field_0;
		int*					field_4;	//	NOTE: pointer to another list. Contents vary.
		unsigned int			field_8;
	};

	struct CallStackElement
	{
		class Node* m_NodePtr;
		void					(*m_FuncPtr)(ScriptThread*);
		unsigned int			field_8;
		unsigned int			field_C;
		unsigned int			field_10;
	};

	struct ThreadCallStack
	{
		unsigned int			field_0;
		int* m_Addresses;
		unsigned int			field_8;
	};

	struct MethodStruct
	{
		void					(*field_0)(ScriptThread*);
		int						field_4;
		void					(*field_8)(ScriptThread*);
		Node*					field_C;
		GlobalScript*			field_10;
		unsigned int			field_14;
	};
protected:
	Defragmentator*				m_Defragmentator;
	std::vector<ThreadList_1>	m_List_1;	//	NOTE: associated with defragmentator?
	Defragmentator*				m_Defragmentator_1;
	std::vector<ThreadCallStack>m_CallStack;
	int							m_SleepUntil;
	int							m_WaitForFrame;
	int							m_ThreadFlags;
	class Node*					m_ScriptNode;
	MethodStruct*				field_3C;
	CallStackElement*			field_40;
public:
	virtual						~ScriptThread();	//	@48ED10
	virtual int					stub1(void*);

	ScriptThread(class Node*);	//	@48EC70

	void						DumpState(String&);	//	@48D690
};
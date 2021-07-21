#pragma once
#include "Node.h"

class Control : public Node
{
	enum ControlType
	{
		NONE = 0,
		KEYBOARD = 1,
		MOUSE = 2,
		MOUSE_BUTTON = 3,
		GAMEPAD = 4,
		DPAD = 5	//	NOTE: "A_LEFT" "A_RIGHT" etc...
	};
protected:
	int				m_Key;
	char			field_54;
	ControlType		m_ControlType;

public:
	Control() : Node(NODE_MASK_EMPTY)	//	NOTE: no constructor.
	{
		MESSAGE_CLASS_CREATED(Control);

		m_Key = -1;
		field_54 = 0;
		m_ControlType = KEYBOARD;
	}

	static void		Register();	//	@924A30

protected:
	const int		MousePositionEngineToString() const;	//	@924600
	const int		MousePositionRawToString() const;	//	@9245A0

private:
	const int		GetKey() const;	//	@8F92C0
	void			SetKey(const int keyind);	//	@924590

	const char*		GetKeyStr() const;	//	@9249C0
	void			SetKeyStr(const char* args);	//	@9247B0

	const char*		DPadKeyToStr(const unsigned int keyid) const;	//	@924730

	static Control* Create(AllocatorIndex);	//	@924E60
};

extern EntityType* tControl;	//	@A3E158

ASSERT_CLASS_SIZE(Control, 92);
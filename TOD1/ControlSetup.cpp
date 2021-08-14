#include "ControlSetup.h"
#include "InputKeyboard.h"
#include "InputMouse.h"

EntityType* tControlSetup;

void ControlSetup::GetControlId(int* args) const
{
	*args = GetControlId_Impl((const char* const)args[1]);
}

const int ControlSetup::GetControlId_Impl(const char* const controlname) const
{
	if (!m_ControlsList.size())
		return -1;

	for (unsigned int i = 0; i < m_ControlsList.size(); ++i)
		if (strcmp(m_ControlsList[i].m_Name.m_Str, controlname) == NULL)
			return i;

	return NULL;
}

void ControlSetup::IsControlDown(int* args) const
{
	*args = IsControlDown_Impl(args[1]);
}

#pragma message(TODO_IMPLEMENTATION)
const bool ControlSetup::IsControlDown_Impl(const int controlid) const
{
	return false;
}

void ControlSetup::GetKeyPressedString(char** args) const
{
	*args = (char*)GetKeyPressedString_Impl();
}

const char* const ControlSetup::GetKeyPressedString_Impl()
{
	for (unsigned int i = 0; i < 255; ++i)
		if (g_InputKeyboard->m_Acquired &&
			g_InputKeyboard->m_ButtonStates[i] & 1)
			return Input::Keyboard::KeyboardButtonToStr(i);

	for (unsigned int i = 0; i < 10; ++i)
		if (g_InputMouse->m_ShouldBeProcessed &&
			g_InputMouse->m_MouseButtons[i] & 2)
			return Input::Mouse::MouseButtonToStr(i);

	return nullptr;
}

ControlSetup* ControlSetup::Create(AllocatorIndex)
{
	return new ControlSetup;
}

#pragma message(TODO_IMPLEMENTATION)
void ControlSetup::Register()
{
	tControlSetup = new EntityType("ControlSetup");
	tControlSetup->InheritFrom(tNode);
	tControlSetup->SetCreator((EntityType::CREATOR)Create);

	tControlSetup->RegisterScript("getcontrolid(string):integer", &GetControlId, NULL, NULL, NULL, nullptr, nullptr);
	tControlSetup->RegisterScript("iscontroldown(integer):truth", &IsControlDown, NULL, NULL, NULL, nullptr, nullptr);
}
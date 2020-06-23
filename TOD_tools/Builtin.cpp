#include "Builtin.h"
#include "Globals.h"

Builtin* g_Builtin = NULL;

int Builtin::GetMessageId_Impl(const char* szMessage)
{
	//	Figure out where colon character is and cut out from beginning of string up to it's position.
	unsigned int pos = 0;
	while (pos < strlen(szMessage))
		if (szMessage[pos] == ':')
			break;

	return _GetMessageId(&szMessage[pos]);
}

int Builtin::_GetMessageId(const char* szMessage)
{
	char* msg = (char*)szMessage;
	Utils::ToLowercase(msg);

	//	NOTE: this 'method' below used in many places and looks like just comparing string from 2nd and 3rd arguments.
	int* var_0 = (*(int*(__thiscall*)(int*, int*, const String*))0x8729F0)((int*)0xA3CF08, (int*)0xA3CF10, &String(msg));

	if (var_0 && var_0 + 4 != 0)
		return *(var_0 + 4);
	else
		return -1;
}

void Builtin::GetEditorActive(int* outActive)
{
	*outActive = 0;
}

const Vector4f& Builtin::m_ZeroVector = *(Vector4f*)0x9B7084;
const Vector4f& Builtin::m_RightVector = *(Vector4f*)0x9B70A4;
const Vector4f& Builtin::m_UpVector = *(Vector4f*)0x9B70B4;
const Vector4f& Builtin::m_InVector = *(Vector4f*)0x9B70C4;
const Quaternion<float>& Builtin::m_Orientation = *(Quaternion<float>*)0x9B70D4;
const Vector4f& Builtin::m_LeftVector = *(Vector4f*)0x9B70E4;
const Vector4f& Builtin::m_DownVector = *(Vector4f*)0x9B70F4;
const Vector4f& Builtin::m_OutVector = *(Vector4f*)0x9B7104;
const Vector4f& Builtin::m_vUnkColor = *(Vector4f*)0x9B7114;
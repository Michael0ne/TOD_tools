#include "MoviePlayer.h"

String& MoviePlayer::MovieName = *(String*)0xA3D864;	//	@A3D864

MoviePlayer_Struct72::MoviePlayer_Struct72()
{
	m_MovieName = String();

	field_24 = 1;
	m_f0 = m_f4 = 1.0f;
	field_8 = field_C = 0;
	m_f10 = 1.0f;
	field_25 = 0;
	m_Direct3DDevice9 = nullptr;
	field_44 = 0;
}
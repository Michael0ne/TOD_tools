#include "File.h"

File* File::g_FilesArray[8] = {
	(File*)0xA35DB8,
	(File*)0xA35DBC,
	(File*)0xA35DC0,
	(File*)0xA35DC4,
	(File*)0xA35DC8,
	(File*)0xA35DCC,
	(File*)0xA35DD0,
	(File*)0xA35DD4
};

HANDLE* File::g_FileSemaphores[8] = {
	(HANDLE*)0xA35D98,
	(HANDLE*)0xA35D9C,
	(HANDLE*)0xA35DA0,
	(HANDLE*)0xA35DA4,
	(HANDLE*)0xA35DA8,
	(HANDLE*)0xA35DAC,
	(HANDLE*)0xA35DB0,
	(HANDLE*)0xA35DB8
};

inline void PATCH_FILEPROC()
{

}
#include "RealtimeFolder.h"

RealtimeFolder* RealtimeFolder::RealtimeFolderInstance;	//	@A3D8D4
RealtimeFolder* tRealtimeFolder = nullptr;

RealtimeFolder::RealtimeFolder() : Folder_()
{
	MESSAGE_CLASS_CREATED(RealtimeFolder);

	RealtimeFolderInstance = this;
}
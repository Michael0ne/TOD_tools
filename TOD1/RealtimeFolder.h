#pragma once

#include "Folder.h"

#define REALTIME_FOLDER_CLASS_SIZE 88

class RealtimeFolder : public Folder_
{
	friend class Scene;
protected:

	static RealtimeFolder*	RealtimeFolderInstance;	//	@A3D8D4
public:
	RealtimeFolder();
};

extern RealtimeFolder* tRealtimeFolder;
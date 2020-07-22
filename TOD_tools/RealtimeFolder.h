#pragma once

#include "Folder.h"

#define REALTIME_FOLDER_CLASS_SIZE 88

class RealtimeFolder : public Folder_
{
protected:

public:
	RealtimeFolder() : Folder_()	//	NOTE: no constructor.
	{
		MESSAGE_CLASS_CREATED(RealtimeFolder);

		*Instance = this;
	}

	static RealtimeFolder** Instance;
};
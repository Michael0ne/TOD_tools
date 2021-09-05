#pragma once
#include "Folder.h"

class RealtimeFolder : public Folder_
{
 friend class Scene;
protected:

 static RealtimeFolder* RealtimeFolderInstance; // @A3D8D4
public:
 RealtimeFolder();
};

extern RealtimeFolder* tRealtimeFolder;

ASSERT_CLASS_SIZE(RealtimeFolder, 88);
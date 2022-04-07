#pragma once
#include "Folder.h"

class RealtimeFolder : public Folder_
{
    friend class Scene;
protected:

    static RealtimeFolder* Instance; // @A3D8D4
public:
    inline RealtimeFolder() : Folder_()
    {
        MESSAGE_CLASS_CREATED(RealtimeFolder);

        Instance = this;
    }

    static void             Register(); //  @892B80

private:
    static RealtimeFolder*  Create(AllocatorIndex); //  @892C20
};

extern EntityType* tRealtimeFolder; //  @A3D8D0

ASSERT_CLASS_SIZE(RealtimeFolder, 88);
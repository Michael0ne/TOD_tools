#pragma once
#include "Node.h"
#include <vector>

class SignPost : public Node
{
protected:
    std::vector<int>   *m_ConnectionsList;
    std::vector<int>    field_54;
    std::vector<int>    field_64;
    int                 field_74;
    int                 field_78;
    int                 field_7C;
    int                 field_80;
    int                 field_84;

private:
    static SignPost*    Create(AllocatorIndex); //  @921040

public:
    SignPost(); // @920EA0
    virtual ~SignPost();    //  @920F60

    std::vector<int>*   GetConnections() const; //  @750FA0
    void                SetConnections(void* args); //  @921360

    static void     Register(); //  @921420

    static std::vector<SignPost*> SignPostList; // @A133F0
};

extern EntityType* tSignPost;   //  @A3E144

ASSERT_CLASS_SIZE(SignPost, 136);
#pragma once
#include "Globals.h"
#include "FragmentAsset.h"
#include "AssetManager.h"

class Fragment
{
public:
    Node           *m_Owner;
    FragmentAsset  *m_FragmentRes;
    int            *field_8;
    char           *m_Name;
    Utils::UniqueId m_UniqueId;

public:
    Fragment(class Node* owner); // @87F1E0
    ~Fragment()
    {
        MESSAGE_CLASS_DESTROYED(Fragment);

        delete[] m_Name;
        if (m_FragmentRes)
            g_AssetManager->DecreaseResourceReferenceCount(m_FragmentRes);
        delete field_8;
    };

    void   SetFragmentName(const char* fragmentname); // @87F090
    void   LoadResourceFile(const char* fname); // @87F130
    void   ApplyFragment() const; // @87F190
};

ASSERT_CLASS_SIZE(Fragment, 24);
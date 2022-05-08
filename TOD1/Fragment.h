#pragma once
#include "Globals.h"
#include "FragmentAsset.h"
#include "AssetManager.h"

class Fragment
{
public:
    Node           *m_Owner;
    AssetLoader     m_FragmentRes;
    char           *m_Name;
    Utils::UniqueId m_UniqueId;

public:
    Fragment(class Node* owner); // @87F1E0
    ~Fragment()
    {
        MESSAGE_CLASS_DESTROYED(Fragment);

        delete[] m_Name;
    };

    void   SetFragmentName(const char* fragmentname); // @87F090
    void   LoadResourceFile(const char* fname); // @87F130
    void   ApplyFragment() const; // @87F190
};

ASSERT_CLASS_SIZE(Fragment, 24);
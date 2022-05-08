#include "Fragment.h"
#include "AssetManager.h"
#include "Node.h"

Fragment::Fragment(Node* owner)
{
    MESSAGE_CLASS_CREATED(Fragment);

    m_UniqueId = {};
    m_Owner = owner;
    m_Name = nullptr;
}

void Fragment::SetFragmentName(const char* fragmentname)
{
    if (m_Name)
        delete[] m_Name;

    m_Name = new char[52];
    strcpy(m_Name, fragmentname);
}


void Fragment::LoadResourceFile(const char* fname)
{
    m_FragmentRes = AssetLoader(fname);
}

void Fragment::ApplyFragment() const
{
    if (m_FragmentRes && m_FragmentRes.GetAsset<FragmentAsset>()->m_ResourceTimestamp)
    {
        g_AssetManager->AddLoadedAssetName(m_FragmentRes.GetAsset<FragmentAsset>()->m_ResourcePath);
        m_FragmentRes.GetAsset<FragmentAsset>()->ApplyFragmentResource(m_Owner->m_Id.Id, true);
        g_AssetManager->m_LoadedAssetsNames.pop_back();
    }
}
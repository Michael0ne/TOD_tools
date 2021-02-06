#include "Fragment.h"
#include "Globals.h"
#include "Blocks.h"
#include "Node.h"

Fragment::Fragment(Node* owner)
{
	MESSAGE_CLASS_CREATED(Fragment);

	m_FragmentRes = nullptr;
	field_8 = 1;
	Utils::CreateUniqueId(m_UniqueId);
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
	ResType::ResourceHolder reshld;
	reshld.LoadResourceFromBlock(fname);
	m_FragmentRes->ApplyLoadedResource(reshld);

	if (reshld.m_Resource)
		g_Blocks->DecreaseResourceReferenceCount(reshld.m_Resource);
}

void Fragment::ApplyFragment() const
{
	if (m_FragmentRes && m_FragmentRes->m_ResourceTimestamp)
	{
		g_Blocks->SetSceneName(m_FragmentRes->m_ResourcePath);
		m_FragmentRes->ApplyFragmentResource(m_Owner->m_Id >> 8, true);
		g_Blocks->RemoveLastSceneName();
	}
}
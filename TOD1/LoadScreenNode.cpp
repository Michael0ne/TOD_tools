#include "LoadScreenNode.h"
#include "AssetManager.h"
#include "LoadScreenInfo.h"
#include "Progress.h"
#include "StringType.h"

EntityType* tLoadScreenNode;

void LoadScreenNode::Activate_Impl()
{
    g_AssetManager->GetResourcePath(m_ImageFile, m_ImageFile.m_Str);
    g_LoadScreenInfo->SetTexturePath(m_ImageFile.m_Str);
    g_Progress->SetLoadScreenSprite(m_LoadBarSprite);
    g_LoadScreenInfo->Enable(m_RenderOnTopNode);
}

LoadScreenNode::LoadScreenNode() : Node(NODE_MASK_EMPTY)
{
    MESSAGE_CLASS_CREATED(LoadScreenNode);

    m_ImageFile;
    m_RenderOnTopNode = nullptr;
    m_LoadBarSprite = nullptr;
}

LoadScreenNode::~LoadScreenNode()
{
    MESSAGE_CLASS_DESTROYED(LoadScreenNode);
}

void LoadScreenNode::Instantiate()
{
    Node::Instantiate();

    m_LoadBarSprite = nullptr;
    if (g_Progress)
        g_Progress->SetLoadScreenSprite(nullptr);
}

const char* LoadScreenNode::GetImageFile() const
{
    return m_ImageFile.m_Str;
}

void LoadScreenNode::SetImageFile(const char* _fname)
{
    if (_fname && *_fname && g_AssetManager)
        m_ImageFile = g_AssetManager->GetResourcePathSceneRelative(_fname);
}

void LoadScreenNode::Activate()
{
    Activate_Impl();
}

void LoadScreenNode::Deactivate(int)
{
    g_LoadScreenInfo->Deactivate();
}

void LoadScreenNode::SetRenderOnTopNode(Node* _node)
{
    m_RenderOnTopNode = _node;
}

void LoadScreenNode::SetLoadbarSprite(class Sprite* _sprite)
{
    m_LoadBarSprite = _sprite;
    g_Progress->SetLoadScreenSprite(_sprite);
}

void LoadScreenNode::Register()
{
    tLoadScreenNode = new EntityType("LoadScreenNode");
    tLoadScreenNode->InheritFrom(tNode);
    tLoadScreenNode->SetCreator((CREATOR)Create);

    tLoadScreenNode->RegisterProperty(tSTRING, "imagefile", (EntityGetterFunction)&GetImageFile, 0, 0, 0, (EntitySetterFunction)&SetImageFile, 0, 0, 0, "control=resource|type=*.bmp", 0, 0, -1);
    tLoadScreenNode->RegisterScript("activate", (EntityFunctionMember)&Activate, 0, 0, 0, "control=button|text=Activate", nullptr);
    tLoadScreenNode->RegisterScript("deactivate", (EntityFunctionMember)&Deactivate, 0, 0, 0, "control=button|Deactivate", nullptr);
    tLoadScreenNode->RegisterScript("setrenderontopnode(entity)", (EntityFunctionMember)&SetRenderOnTopNode, 0, 0, 0, nullptr, nullptr);
    tLoadScreenNode->RegisterScript("setloadbarsprite(entity)", (EntityFunctionMember)&SetLoadbarSprite, 0, 0, 0, nullptr, nullptr);

    tLoadScreenNode->PropagateProperties();
}

LoadScreenNode* LoadScreenNode::Create(AllocatorIndex)
{
    return new LoadScreenNode;
}
#include "LoadScreenInfo.h"
#include "Timer.h"
#include "LogDump.h"
#include "AssetManager.h"
#include "GfxInternal.h"
#include "ScriptDatabase.h"
#include "Camera.h"
#include "Scene.h"

LoadScreenInfo* g_LoadScreenInfo = nullptr;
unsigned int LoadScreenInfo::AllocatorIdForTextureResourceLoading;
bool LoadScreenInfo::IsLoadingLoadScreenTexture;

void LoadScreenInfo::LoadTexture()
{
    if (m_TexturePath.Empty())
        m_TexturePath = LOADSCREEN_DEFAULT_TEXTURE;

    AllocatorIdForTextureResourceLoading = RENDERLIST;
    IsLoadingLoadScreenTexture = true;

    String respath;
    AssetManager::CorrectTextureResourcePath(respath, m_TexturePath.m_Str, GfxInternal::GetRegion(), AssetManager::PlatformId::PC);

    LogDump::LogA("LoadScreen:%s\n", respath.m_Str);
    m_TextureResource = (TextureAsset*)g_AssetManager->LoadNativeResource(respath.m_Str);

    if (g_GfxInternal->m_CheckerboardTextures[0] == m_TextureResource->m_Texture_1)
        LogDump::LogA("TEXTURE NOT FOUND!!!!!\n");

    IsLoadingLoadScreenTexture = false;
    AllocatorIdForTextureResourceLoading = DEFAULT;
}

LoadScreenInfo::LoadScreenInfo(const char* texturePath)
{
    MESSAGE_CLASS_CREATED(LoadScreenInfo);

    m_TexturePath = texturePath;
    m_Enabled = false;
}

void LoadScreenInfo::SetTexturePath(const char* _path)
{
    m_TexturePath = _path;
}

void LoadScreenInfo::Enable(Node* topNode)
{
    if (m_Enabled)
        return;

    m_StartTime = Timer::GetMilliseconds();
    LogDump::LogA("Enabling load screen\n");

    Show(topNode);

    m_Enabled = true;
}

#pragma message(TODO_IMPLEMENTATION)
void LoadScreenInfo::Show(Node* topNode)
{
    DWORD timestart = Timer::GetMilliseconds();
    float f9758 = g_GfxInternal_Dx9->field_9758;

    g_AssetManager->DestroySceneNodesFrameBuffers(true);
    LoadTexture();

    FrameBuffer* fb = new FrameBuffer(50, 4, 2);
    for (unsigned int i = 3; i; i--)
    {
        fb->Reset();
        fb->SubmitRenderFullscreenTextureCommand(m_TextureResource->m_Texture_1);
        fb->_436BF0();
        fb->_436040(23, 0);

        if (topNode)
        {
            CameraMatrix* cammat = new CameraMatrix;

            DirectX::XMMATRIX actcammat;
            Scene::SceneInstance->m_ActiveCamera->GetMatrix(actcammat);

            cammat->m_Vec[0] = *(Vector4f*)&(actcammat.r[3]);
            cammat->m_Vec[1] = Scene::SceneInstance->m_ActiveCamera->m_CameraMatrix.m_Vec[0];
            cammat->m_Vec[2] = Scene::SceneInstance->m_ActiveCamera->m_CameraMatrix.m_Vec[1];
            cammat->m_Vec[3] = Scene::SceneInstance->m_ActiveCamera->m_CameraMatrix.m_Vec[2];
            cammat->m_Vec[4] = Scene::SceneInstance->m_ActiveCamera->m_CameraMatrix.m_Vec[3];
            cammat->m_Vec[5] = Scene::SceneInstance->m_ActiveCamera->m_CameraMatrix.m_Vec[4];

            topNode->_88C310(cammat);

            delete cammat;
        }

        g_GfxInternal->Render(nullptr, true, 23, 28);
    }

    g_GfxInternal_Dx9->field_9758 += f9758;
    delete fb;

    g_AssetManager->DestroyTextureAsset(*m_TextureResource);
    Asset::Destroy(m_TextureResource);

    LogDump::LogA("LoadScreen::Show (%s) took %ims\n", m_TexturePath.m_Str, Timer::GetMilliseconds() - timestart);
}

void LoadScreenInfo::Deactivate()
{
    if (!m_Enabled)
        return;

    LogDump::LogA("Disabling load screen (%0.1f sec)\n", (Timer::GetMilliseconds() - m_StartTime) * 0.001f);
    m_Enabled = false;
}
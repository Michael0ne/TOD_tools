#pragma once
#include "Folder.h"
#include "MeshColorAsset.h"
#include "Camera.h"
#include <vector>

#define EDITOR_SESSION_FILE_VERSION 3

class Scene : public Folder_
{
public:
    enum PlayMode
    {
        MODE_INGAME = 0,
        MODE_STOP = 1,
        MODE_PAUSED = 2,
        MODE_REWIND = 3,
        MODE_4 = 4
    };

    enum SaveOperationStatus
    {
        STATUS_OK = 0,
        STATUS_CANT_OPEN_FILE = 1,
        STATUS_CHECKSUM_MISMATCH = 2,
        STATUS_SAVEDIR_NOT_READY = 3,
        STATUS_CANT_READ_SAVE_DATA = 4
    };

    enum SaveLoadState
    {
        STATE_DONE = 0,
        STATE_SAVE = 1,
        STATE_LOAD = 2,
        STATE_LOAD_SUMMARY = 3
    };

public:
    MeshColorAsset* m_StaticLighting;
    PlayMode        m_PlayMode;
    Camera*         m_GameCamera;
    class EditorCamera* m_EditorCamera;
    char            m_QuadTreesAllocated;
    Camera*         m_ActiveCamera;
    Node*           m_ClosestNode;
    float           m_CollisionProbeMinDistance;
    Vector4f        m_CollisionPos_1;
    Vector4f        m_CollisionResolvedPos_1;
    int             field_98;
    int             field_9C;
    int             field_A0;
    int             field_A4;
    class CollisionProbe*       m_SharedProbe;
    class TransactionBuffer*    m_RewindBuffer1;
    class TransactionBuffer*    m_RewindBuffer2;
    int             field_B4;
    int             m_NextRewindUpdateTime;
    int             field_BC;
    int             field_C0;
    int             m_RewindResumeTimeMs;
    int             field_C8;
    std::vector<int>            m_List_1;
    std::vector<AuxQuadTree*>   m_QuadTreesList;
    std::vector<int>  m_ParticleSystemsList;
    std::vector<CollisionInfo*> m_CollisionListList;
    int             m_NodesWithUpdateOrBlockingScripts;
    char            m_InitMode;
    float           m_TimeMultiplier;
    float           field_118;
    float           m_RewindTimeMultiplier;
    bool            m_FixedFramerate;
    float           m_FixedFramerateVal;
    int             field_128;
    std::vector<int>    m_List_5;
    std::vector<int>    m_List_6;
    std::vector<int>    m_List_7;
    std::vector<int>    m_List_8;
    SaveLoadState   m_SaveLoadState;
    int             m_SavePointOperationError;
    String          m_SaveDir;
    int             m_MemoryCardIndex;
    int             m_SaveSlotIndex;
    Node*           m_SaveData;
    int             m_SaveGameSize;
    class MemoryCards*  m_MemoryCards;
    Vector4f        m_CameraPosition;
    char            field_1A8;
    char            m_WindMode;
    bool            field_1AA;
    bool            m_WindPause;
    bool            m_FlushRewindRequested;
    Node*           m_LoadedBlocks[8];  //  NOTE: Blocks = maps.
    char            field_1D0[8];
    class FrameBuffer*   m_FrameBuffers[31];
    class FrameBuffer*   m_FrameBuffer_1;
    class FrameBuffer*   m_FrameBuffer_2;
    unsigned int    m_RenderTimeMs;
    UINT64          m_StartTimeMs;
    int             field_268;

public:
    Scene(); // @896D40
    virtual ~Scene();   //  @8971F0
    virtual void Destroy() override;    //  @895E40
    virtual void Update() override; //  @897450
    virtual void Render() override; //  @896370

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }

    void            Start(); // @89A100
    void            Load(const char* sceneName); // @8980C0
    void            FinishCreation(const char* logTitle); // @8935F0
    void            StoreGameCamera(); // @893480
    void            EnumSceneCamerasAndUpdate(); // @893870
    void            AllocateRewindBuffer(); // @894C50
    void            BuildSceneTree(); // @896BA0
    void            FreeRewindBuffer(); // @896CD0
    void            ResetRewindBuffer(bool); // @894A80
    void            _896BA0();  //  @896BA0
    void            ReleaseQuadTreeAndRenderlist(); // @896C30
    void            LoadResourceBlockIntoSceneBuffer(const char* assetname, AssetInfo::ActualAssetInfo* assetinfo); // @892E40
    void            CreateSavePoint(unsigned int memcardind, unsigned int slotind, const char* const savedirectory, Node* summarynode, unsigned int savesize); // @895B80
    void            RestoreSavePoint(unsigned int memcardind, unsigned int slotind, const char* const savedirectory, Node* summarynode, MemoryCards* memcards); // @895C00
    void            LoadSavePointSummary(unsigned int memcardind, unsigned int slotind, const char* const savedirectory, Node* summarynode); // @895CB0
    void            ProfileMemory(const int);   //  @893460
    void            ClearNodesLists();  //  @894950
    void            _895F50();  //  @895F50
    Folder_*        GetLoadedBlockByIndex(const unsigned int index) const;   //  @893290
    void            LoadSceneSession(void) const;   //  @8956D0
    void            InstantiateAssetsToLists(); //  @896810
    void            AddCollisionList(CollisionInfo* list); //  @896B40
    void            RemoveCollisionList(CollisionInfo* list);   //  @895AD0
    void            LoadMap(const unsigned int blockind, Node* foldernode); //  @8932A0
    float           GetTimeMultiplier() const;  //  @4B3FC0
    void            SetTimeMultiplier(const float multiplier);  //  @70EE70
    float           GetRewindTimeMultiplier() const;    //  @755240
    void            SetRewindTimeMultiplier(const float mult);  //  @892C70
    char            GetInitMode() const;    //  @892C90
    void            SetInitMode(const char initmode);   //  @70EEA0
    float           GetWindSize() const;    //  @896110 //  NOTE: wind = rewind.
    void            _894810(const int index, const bool enabled);    //  @894810
    const int       GetPlaymode() const;    //  @4B3FB0
    const float     GetRewindResumeTime() const;    //  @892D30
    const int       GetRewindResumeTimeMs() const;  //  @917880
    void            SetWindMode(const int* args);   //  @892CC0
    void            SetWindPause(const int* args);  //  @892CF0
    void            SetPauseMode(const int* args);  //  @89A5A0
    void            FlushRewind(const int* args);   //  @892D20
    void            ResetGame(const int* args); //  @892D50
    void            GetLoadedBlock(int* args);  //  @89A4F0
    void            SetLoadedBlock(int* args);  //  @89A510
    void            UpdateLoadedBlocks(int* args);  //  @89A810
    void            UpdateLoadedBlocks_Impl(const int dummy, Node* callbackEntity); //  @8986E0
    void            BuildFastFindNodeVector(int* args); //  @892CA0
    void            DeleteFastFindNodeVector(int* args);    //  @892CB0
    void            UnloadLoadedFolders();  //  @8953E0
    void            Reset();    //  @89A1A0
    void            CountBlockingScriptNodes(); //  @895480
    void            SyncEditorCamera(const bool kdtreealloc, const int gamepadindex);   //  @893780
    void            AnnotateSphere_Impl(const Vector4f& pos, const int a2, const int a3, const int a4) const;   //  @8935E0
    void            AnnotateLine_Impl(const Vector4f& lineStart, const Vector4f& lineEnd, const int a3, const int a4) const;    //  @8935D0
    void            AnnotatePoint_Impl(const Vector4f& point, const int a2, const int a3) const;    //  @8935C0
    void            SetParticleSystemUsed(const int particleSystemIndex, const bool used);  //  @8947D0

    static int      RealTimeMs; // @A3DCCC
    static int      GameTimeMs; // @A3DCD4
    static int      NextUpdateTime; // @A3DCD0
    static int      TotalFrames; // @A3DCE4
    static int      NewFrameNumber; // @A3DCE0
    static bool     IsRewindBufferInUse; // @A1207C
    static bool     LoadingAssetBlock; // @A3CE60
    static const unsigned int RewindBufferSize_1 = 204800; // @A12088
    static const unsigned int RewindBufferSize_2 = 309248; // @A1208C
    static float    RealTimePassed; // @A3DCC0
    static float    TimePassed; // @A3DCC4
    static UINT64   CreationTime; // @A3DD00
    static int     *_A3CEE4; // @A3CEE4 //  NOTE:   it looks like this stores entities properties with format (int, int*), 
                                        //          where first int is a bitfield (propertyIndex, entity Id, entity block Id) and second is a pointer to the actual property value.
    static int     *_A3CEE8; // @A3CEE8 //  NOTE: this and above are allocated and tied to transaction buffer.
    static int      _A3DA80[100]; // @A3DA80
    static int      _A3D8D8[100]; // @A3D8D8
    static int      _A3DC38[4]; // @A3DC38
    static int      UpdateOrBlockingListSize;   //  @A3DD08
    static bool     _A3D858;    //  @A3D858

    static Scene   *SceneInstance;      // @A3DCBC
    static AuxQuadTree* MainQuadTree;   // @A3DCE8
    static AuxQuadTree* AuxQuadTree;    //  @A3DCEC

    struct QuadTreeNode
    {
        short   field_0[4];
        int     field_8[4];
        short   field_18;
        char    field_1A;
        char    field_1B;
        short   field_1C;
        short   field_1E;
    };

    struct EntityReference
    {
        int            *m_AddressRegionBeginPtr;
        int            *m_AddressRegionEndPtr;
    };

    static unsigned int QuadTreesAllocated; //  @A3DD70
    static QuadTreeNode*    QuadTrees;  //  @A3B580
    static short    _A120E8;    //  @A120E8
    static int      _A3DD40;    //  @A3DD40
    static std::vector<EntityReference>    *DanglingEntityReferences;  //  @A3CEEC
    static std::map<int*, int>             *DanglingEntityReferencesMap;    //  @A3CEF0

    static int      PreBlocksUnloadedCommand; // @A3DCF8
    static int      BlocksUnloadedCommand; // @A3DCFC
    static int      InvalidatePlaceholderModelCommand; // @A12098
    static int      RewindOrRetryFinishedCommand; // @A1209C
    static float    FrameRateHistory[100];  //  @A3DA80 //  NOTE: only calculated, but not used anywhere. Maybe an editor leftover.
    static int      FrameRateHistoryIndex;  //  @A3DCDC
    static float    FrameTimeTotal; //  @A3DCD8 //  NOTE: only calculated, but not used anywhere. Maybe an editor leftover.
    static bool     _A3D890;

private:
    static const int    RewindCollectInterval = 2000;   //  @A12084 //  NOTE: interval when rewind buffer is updated.

public:
    static void     CreateQuadTrees(const unsigned int num, const AllocatorIndex allocind); //  @89A370
    static void     TriggerScriptForAllChildren(int scriptId, Node* node, int* args); // @892F10
    static void     Register(); // @899CC0
    static Scene*   Create(AllocatorIndex); // @89A7A0
};

extern EntityType* tScene; // @A3DCB8

ASSERT_CLASS_SIZE(Scene, 620);
ASSERT_CLASS_SIZE(Scene::QuadTreeNode, 32);
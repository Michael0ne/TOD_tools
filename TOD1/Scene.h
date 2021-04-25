#pragma once
#include "Folder.h"
#include <vector>

enum PlayMode
{
	MODE_PLAY = 0,
	MODE_UNKNOWN_1 = 1,
	MODE_PAUSE = 2,
	MODE_UNKNOWN_3 = 3
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

//#pragma pack(4)
class Scene : public Folder_
{
	friend class MemoryCards;
	friend class Camera;
	friend class Node;
	friend class SceneSaveLoad;
protected:
	MeshColorAsset* m_StaticLighting;
	int				m_PlayMode;	//	NOTE: 2 - menu.
public:
	class Camera*	m_GameCamera;
	class EditorCamera* m_EditorCamera;
protected:
	char			m_QuadTreesAllocated;
	class Camera*	m_ActiveCamera;
	Node*			m_ClosestNode;
	float			m_CollisionProbeMinDistance;
	Vector4f		m_CollisionPos_1;
	Vector4f		m_CollisionResolvedPos_1;
	int				field_98;
	int				field_9C;
	int				field_A0;
	int				field_A4;
	class CollisionProbe* m_SharedProbe;
	class TransactionBuffer* m_RewindBuffer1;
	class TransactionBuffer* m_RewindBuffer2;
	float			field_B4;
	float			field_B8;
	float			field_BC;
	int				field_C0;
	int				m_RewindResumeTimeMs;
	int				field_C8;
	std::vector<int>m_List_1;
	std::vector<AuxQuadTree*> m_QuadTreesList;
	std::vector<class ParticleSystem*> m_ParticleSystemsList;
	std::vector<CollisionList*> m_CollisionListList;
	int				m_NodesWithUpdateOrBlockingScripts;
	char			m_InitMode;
	float			m_TimeMultiplier;
	float			field_118;
	float			m_RewindTimeMultiplier;
public:
	bool			m_FixedFramerate;
	float			m_FixedFramerateVal;
protected:
	int				field_128;
	std::vector<int>m_List_5;
	std::vector<int>m_List_6;
	std::vector<int>m_List_7;
	std::vector<int>m_List_8;
	SaveLoadState	m_SaveLoadState;
	int				m_SavePointOperationError;
	String			m_SaveDir;
	int				m_MemoryCardIndex;
	int				m_SaveSlotIndex;
	Node*			m_SaveData;
	int				m_SaveGameSize;
	class MemoryCards* m_MemoryCards;
	Vector4f		m_CameraPosition;
	char			field_1A8;
	char			m_WindMode;
	bool			field_1AA;
	bool			m_WindPause;
	bool			m_FlushRewindRequested;
	Node*			m_LoadedBlocks[8];
	char			field_1D0[8];
	class RenderBuffer92*	m_SceneBufferArray[31];
	class RenderBuffer92*	m_Buffer_1;
	class RenderBuffer92*	m_Buffer_2;
public:
	UINT64			m_StartTimeMs;
protected:
	int				field_268;

public:
	Scene();	//	@896D40

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

	void			Start();	//	@89A100
	void			Load(const char* sceneName);	//	@8980C0
	void			RefreshChildNodes();	//	@88C2B0
	void			FinishCreation(const char* logTitle);	//	@8935F0
	void			StoreGameCamera();	//	@893480
	void			EnumSceneCamerasAndUpdate();	//	@893870
	void			UpdateLoadedBlocks(int unk1, Node* unk2);	//	@8986E0
	void			_896810();	//	@896810
	void			AllocateRewindBuffer();	//	@894C50
	void			BuildSceneTree();	//	@896BA0
	void			FreeRewindBuffer();	//	@896CD0
	void			ResetRewindBuffer(bool);	//	@894A80
	void			ReleaseQuadTreeAndRenderlist();	//	@896C30
	void			LoadResourceBlockIntoSceneBuffer(const char* assetname, AssetInfo::ActualAssetInfo* assetinfo);	//	@892E40
	void			CreateSavePoint(unsigned int memcardind, unsigned int slotind, const char* const savedirectory, const Node* summarynode, unsigned int savesize);	//	@895B80
	void			RestoreSavePoint(unsigned int memcardind, unsigned int slotind, const char* const savedirectory, const Node* summarynode, const MemoryCards* memcards);	//	@895C00
	void			LoadSavePointSummary(unsigned int memcardind, unsigned int slotind, const char* const savedirectory, const Node* summarynode);	//	@895CB0

	static int		RealTimeMs;	//	@A3DCCC
	static int		GameTimeMs;	//	@A3DCD4
	static int		NextUpdateTime;	//	@A3DCD0
	static int		TotalFrames;	//	@A3DCE4
	static int		NewFrameNumber;	//	@A3DCE0
	static bool		IsRewindBufferInUse;	//	@A1207C
	static bool		LoadingAssetBlock;	//	@A3CE60
	static const unsigned int RewindBufferSize_1;	//	@A12088
	static const unsigned int RewindBufferSize_2;	//	@A1208C
	static float	FrameRate;	//	@A3DCC0
	static float	FrameRate_1;	//	@A3DCC4
	static UINT64	CreationTime;	//	@A3DD00
	static int     *_A3CEE4;	//	@A3CEE4
	static int		_A3CEE8;	//	@A3CEE8
	static int		_A3DA80[100];	//	@A3DA80
	static int		_A3D8D8[100];	//	@A3D8D8
	static int		_A3DC38[4];	//	@A3DC38

	static Scene*	SceneInstance;	//	@A3DCBC
	static AuxQuadTree* SceneTree;	//	@A3DCE8

	static int		PreBlocksUnloadedCommand;	//	@A3DCF8
	static int		BlocksUnloadedCommand;	//	@A3DCFC
	static int		InvalidatePlaceholderModelCommand;	//	@A12098
	static int		RewindOrRetryFinishedCommand;	//	@A1209C

	static void		TriggerScriptForAllChildren(int scriptId, Scene* sceneNode, int* unk);	//	@892F10
	static void		Register();	//	@899CC0
	static Scene*	Create(AllocatorIndex);	//	@89A7A0
};

extern EntityType* tScene;	//	@A3DCB8

ASSERT_CLASS_SIZE(Scene, 616);	//	FIXME: actual size is 620 (0x26C).
#pragma once

#include "Folder.h"

#define SCENE_CLASS_SIZE 620

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
	STATUS_SAVE_DIR_EMPTY = 3,
	STATUS_CANT_READ_SAVE_DATA = 4
};

class Scene : public Folder_
{
protected:
	ResType::MeshColor* m_StaticLighting;
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
	List<int>		m_List_1;
	List<AuxQuadTree> m_QuadTreesList;
	List<class ParticleSystem> m_ParticleSystemsList;
	List<CollisionList> m_CollisionListList;
	int				m_NodesWithUpdateOrBlockingScripts;
	char			m_InitMode;
	float			m_TimeMultiplier;
	float			m_f118;
	float			m_RewindTimeMultiplier;
public:
	bool			m_FixedFramerate;
	float			m_FixedFramerateVal;
protected:
	int				field_128;
	List<int>		m_List_5;
	List<int>		m_List_6;
	List<int>		m_List_7;
	List<int>		m_List_8;
	int				m_SaveLoadState;	//	NOTE: 1 - saving game, 2 - loading game.
	int				m_SavePointOperationError;
	String			m_SaveDir;
	int				m_MemoryCardIndex;
	int				m_SaveSlotIndex;
	ScriptType_Entity* m_SaveData;
	int				m_SaveGameSize;
	class MemoryCards* m_MemoryCards;
	Vector4f		m_CameraPosition;
	char			field_1A8;
	char			m_WindMode;
	bool			field_1AA;
	bool			m_WindPause;
	bool			m_FlushRewindRequested;
	Node*			m_LoadedBlocks[6];
	int				field_1C8;
	int				field_1CC;
	char			field_1D0[8];
	class RenderBuffer*	m_SceneBufferArray[31];
	class RenderBuffer*	m_Buffer_1;
	class RenderBuffer*	m_Buffer_2;
public:
	UINT64			m_StartTimeMs;

public:
	Scene();	//	@896D40

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
	}

	void			Start();	//	@89A100
	void			Load(const char* sceneName);	//	@8980C0
	void			RefreshChildNodes();	//	@88C2B0
	void			FinishCreation(const char* logTitle);	//	@8935F0
	static bool		GameUpdate();	//	@93CEB0
	void			UpdateActiveCameraPosition();	//	@893480
	void			EnumSceneCamerasAndUpdate();	//	@893870
	void			UpdateLoadedBlocks(int unk1, Node* unk2);	//	@8986E0
	void			_896810();	//	@896810
	void			AllocateRewindBuffer();	//	@894C50
	void			FreeRewindBuffer();	//	@896CD0
	void			ResetRewindBuffer(bool);	//	@894A80
	void			ReleaseQuadTreeAndRenderlist();	//	@896C30
	void			LoadResourceBlockIntoSceneBuffer(const char* assetname, AssetInfo::ActualAssetInfo* assetinfo);	//	@892E40

	static int		RealTimeMs;	//	@A3DCCC
	static int		GameTimeMs;	//	@A3DCD4
	static int		NextUpdateTime;	//	@A3DCD0
	static int		TotalFrames;	//	@A3DCE4
	static int		NewFrameNumber;	//	@A3DCE0
	static bool		IsRewindBufferInUse;	//	@A1207C
	static bool		LoadingAssetBlock;	//	@A3CE60

	static Scene*	SceneInstance;	//	@A3DCBC
	static AuxQuadTree* SceneTree;	//	@A3DCE8

	static void		TriggerScriptForAllChildren(int scriptId, Scene* sceneNode, int* unk);	//	@892F10
};

extern ScriptType_Entity* tScene;	//	@A3DCB8

static_assert(sizeof(Scene) == SCENE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Scene));
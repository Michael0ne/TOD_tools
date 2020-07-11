#pragma once

#include "Folder.h"

#define SCENE_CLASS_SIZE 620

class Scene_Buffer
{
private:
	int	field_0;
	int	field_4;
	int	field_8;
	int	field_C;
	int	field_10;
	int	field_14;
	int	field_18;
	int	field_1C;
	int	field_20;
	int	field_24;
	int	field_28;
	int	field_2C;
	int	field_30;
	int	field_34;
	int	field_38;
	int	field_3C;
	int	field_40;
	int	field_44;
	int	field_48;
	int	field_4C;
	int	field_50;
	int	field_54;
	int	field_58;

public:
	Scene_Buffer(int unk1, char unk2, int unk3);	//	@436B00
};

enum ePlayModes
{
	MODE_UNKNOWN = 0,
	MODE_UNKNOWN_1 = 1,
	MODE_UNKNOWN_2 = 2,
	MODE_UNKNOWN_3 = 3
};

class Scene : public Folder
{
private:
	int field_58;
	int m_PlayMode;
	class ScriptType_Entity* m_pCamera;
	int* field_64;
	char m_bQuadTreesAllocated;
	class Camera* m_CameraEntity;
	int field_70;
	int field_74;
	int field_78;
	int field_7C;
	int field_80;
	int field_84;
	int field_88;
	int field_8C;
	int field_90;
	int field_94;
	int field_98;
	int field_9C;
	int field_A0;
	int field_A4;
	class CollisionProbe* m_pSharedProbe;
	class RewindBuffer* m_pRewindBuffer1;
	class RewindBuffer* m_pRewindBuffer2;
	int field_B4;
	int field_B8;
	int field_BC;
	int field_C0;
	int m_RewindResumeTimeMs;
	int field_C8;
	List<int> m_List_1;
	List<int> m_List_2;
	List<int> m_List_3;
	List<int> m_List_4;
	int m_NodesWithUpdateOrBlockingScripts;
	char m_InitMode;
	float m_TimeMultiplier;
	float m_f118;
	float m_RewindTimeMultiplier;
	bool m_FixedFramerate;
	char field_121[3];
	float m_FixedFramerateVal;
	int field_128;
	List<int> m_List_5;
	List<int> m_List_6;
	List<int> m_List_7;
	List<int> m_List_8;
	int field_16C;
	int m_nSavePointOperationError;
	String m_sSaveDir;
	int m_nMemoryCardIndex;
	int m_nSaveSlotIndex;
	ScriptType_Entity* field_18C;
	int m_nSaveGameSize;
	class MemoryCards* m_MemoryCards;
	Vector4f m_CameraPosition;
	char field_1A8;
	char m_WindMode;
	char field_1AA;
	char m_WindPause;
	char m_FlushRewindRequested;
	Node* m_LoadedBlocks;
	int field_1B4;
	int field_1B8;
	int field_1BC;
	int field_1C0;
	int field_1C4;
	int field_1C8;
	int field_1CC;
	int field_1D0;
	int field_1D4;
	int field_1D8;
	int field_1DC;
	int field_1E0;
	int field_1E4;
	int* field_1E8;
	int* field_1EC;
	int field_1F0;
	int field_1F4;
	int field_1F8;
	int field_1FC;
	int field_200;
	int field_204;
	int field_208;
	int field_20C;
	int field_210;
	int field_214;
	int field_218;
	int field_21C;
	int field_220;
	int* field_224;
	int field_228;
	int field_22C;
	int field_230;
	int field_234;
	int field_238;
	int field_23C;
	int field_240;
	int field_244;
	int field_248;
	int field_24C;
	int field_250;
	Scene_Buffer* m_Buffer_1;
	Scene_Buffer* m_Buffer_2;
	int m_nLoadTime[2];
	int m_nTimeMilliseconds;
	int field_268;

public:
	Scene();	//	@896D40
};

extern Scene* g_Scene;

static_assert(sizeof(Scene) == SCENE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Scene));
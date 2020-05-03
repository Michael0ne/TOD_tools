#pragma once

#include "stdafx.h"

#include "Node.h"
#include "EditorCamera.h"
#include "Camera.h"
#include "AuxQuadTree.h"

#include "StringsPool.h"

#define SCENE_CLASS_SIZE 620

class Scene;
class CollisionProbe;
class RewindBuffer;

struct Scene_Buffer92 {
private:
	int field_0;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;
	int field_38;
	int field_3C;
	int field_40;
	int field_44;
	int field_48;
	int field_4C;
	int field_50;
	int field_54;
	int field_58;

public:
	void	Reset();	//	@436AE0
};

struct Scene_Buffer24;

struct Scene__vtable {
	void(__thiscall* Destroy)(Scene* _this, bool freememory);	//	@89A7E0
	void(__thiscall* _895E40)(Scene* _this);	//	@895E40
	void(__thiscall* _484CC0)(Node* _this, int unk_1);	//	@484CC0
	int* (__thiscall* FindNode)(Node* _this, const char* searchNode);	//	@88EED0
	void* (__thiscall* _88EC20)(Node* _this, int unk_1);	//	@88EC20
	void(__thiscall* _QuadTreeRefresh)(Scene* _this);	//	@88DE70
	int(__thiscall* Update)(Scene* _this);	//	@897450
	void(__cdecl* nullsub_1)();	//	@8CB190
	__int64(__thiscall* _896370)(Scene* _this);	//	@896370
	char(__stdcall* _ReturnZero_1)(int unk_1, int unk_2);	//	@484DB0
	double(__stdcall* _ReturnMinusOne)(int unk_1, int unk_2);	//	@8F8650
	void(__cdecl* stub1)();	//	@88C600
	int(__thiscall* _88C610)(Node* _this);	//	@88C610
	void(__cdecl* nullsub_2)();	//	@883EC0
	void(__cdecl* nullsub_3)();	//	@8CB190
	void(__cdecl* nullsub_4)();	//	@883EC0
	void(__stdcall* ReturnEmptyString)(String* outString);	//	@484E80
	Node* (__stdcall* _484DC0)(Node* _this);	//	@484DC0
};

class Scene
{
private:
	Scene__vtable* lpVtbl;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	void* m_pPositionVtable;
	int field_28;
	int field_2C;
	AuxQuadTree* m_pQuadTree;
	int field_34;
	int field_38;
	int field_3C;
	int field_40;
	EditorCamera* m_pEditorCamera;
	Fragment* m_pUnkFragment;
	char* m_szUnkBlockName;
	int field_50;
	Scene_Buffer24* m_pBuffer24;
	int field_58;
	int m_nPlayMode;
	Entity* m_pCamera;
	Camera* m_pUnkEntityPtr;
	char m_bQuadTreesAllocated[4];
	Camera* m_pCameraEntity;
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
	CollisionProbe* m_pSharedProbe;
	RewindBuffer* m_pRewindBuffer1;
	RewindBuffer* m_pRewindBuffer2;
	int field_B4;
	int field_B8;
	int field_BC;
	int field_C0;
	int m_nRewindResumeTimeMs;
	int field_C8;
	int field_CC;
	int field_D0;
	int field_D4;
	int field_D8;
	int field_DC;
	int field_E0;
	int field_E4;
	int field_E8;
	int field_EC;
	int field_F0;
	int field_F4;
	int field_F8;
	int field_FC;
	int field_100;
	int field_104;
	int field_108;
	int m_nNodesWithUpdateOrBlockingScripts;
	int m_nInitMode;
	float m_fTimeMultiplier;
	float field_118;
	float m_fRewindTimeMultiplier;
	bool m_bFixedFramerate;	//	120
	char field_121[3];
	float m_fFixedFramerateVal;	//	124
	int field_128;
	int field_12C;
	int field_130;
	int field_134;
	int field_138;
	int field_13C;
	int field_140;
	int field_144;
	int field_148;
	int field_14C;
	int field_150;
	int field_154;
	int field_158;
	int field_15C;
	int field_160;
	int field_164;
	int field_168;
	int field_16C;
	int m_nSavePointOperationError;
	String m_sSaveDir;
	int m_nMemoryCardIndex;
	int m_nSaveSlotIndex;
	int* field_188;
	int m_nSaveGameSize;
	int field_190;
	Vector4f m_vCameraPosition;
	byte field_1A4;
	byte m_nWindMode;
	byte field_1A6;
	byte m_bWindPause;
	byte m_bFlushRewindRequested[4];
	int field_1AC[1];
	int field_1B0;
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
	int* field_1E4;
	int* field_1E8;
	int field_1EC;
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
	int* field_220;
	int field_224;
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
	Scene_Buffer92* m_pBuffer_92_1;
	Scene_Buffer92* m_pBuffer_92_2;
	int m_nLoadTime;
	int m_nTimeMilliseconds_1;
	int m_nTimeMilliseconds;
	int field_264;

public:
	static bool		Update();	//	@93CEB0
	static bool		Instantiate(const char* szSceneName);	//	@93CE00
	void		Start();	//	@89A100
	void	UpdateCamera();	//	@893480
	void	ForceLodCalculation(int unk);	//	@88D100

	void	SetFramerate(float frate)
	{
		m_bFixedFramerate = true;
		m_fFixedFramerateVal = 1.0f / frate;
	}

	EditorCamera* GetEditorCamera()
	{
		return m_pEditorCamera;
	}

	Entity* GetCamera()
	{
		return m_pCamera;
	}

	Camera* GetCameraEntity()
	{
		return m_pCameraEntity;
	}

	static int& UnkInt_1;	//	@A3DCE4
};

extern Scene* g_Scene;	//	@A3DCBC

static_assert(sizeof(Scene) == SCENE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE("Scene"));
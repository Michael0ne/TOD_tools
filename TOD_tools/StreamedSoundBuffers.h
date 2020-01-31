#pragma once

#include "stdafx.h"

//	TODO: better name.
struct SoundBufferInstance
{
	int *m_nUnkInt1;
	int m_nUnkInt2;
	int m_nUnkInt3;
	int m_nUnkInt4;
};

/*
 *------------------------------------------------------------
 *------------------------------------------------------------
 *----------------- Handles streamed sounds-------------------
 *------------------------------------------------------------
 *--------- Game's alias is 'StreamedSoundBuffers' -----------
 *------------------------------------------------------------
 * -----------------------------------------------------------
*/
class StreamedSoundBuffers
{
private:
	int m_nMaxConcurrentSounds;
	int m_nCurrentlyPlaying;
	int m_nUnkInt2;
	int m_nUnkInt3;
	BYTE m_nUnkByte1;
	BYTE m_bGlobalPauseCalled;
	BYTE m_bGlobalPause;
	BYTE m_nUnkByte4;
	int m_nUnkInt5;
	int m_nSoundSystem;
	int m_nUnkInt6;
	int m_nUnkInt7;
	int m_nUnkInt8;
	int m_nUnkInt9;
	int m_nUnkInt10;
	int m_nUnkInt11;
	int m_nUnkInt12;
	int m_nUnkInt13;
	int m_nUnkInt14;
	int m_nUnkInt15;
	void *(__stdcall *m_GenericSoundLibMethods)();
	int *m_pUnkObjPtr1;
	void *(__cdecl *m_pDieselPowerMethods)();
	int m_nUnkInt19;
	int m_nUnkInt20;
	int *m_pUnkObjPtr2;
	int *m_pUnkObjArrayPtr3;
	int m_nUnkDieselPowerSoundsCount;
	int m_nUnkInt24;
	int m_nUnkInt25;
	int *m_pUnkObjArrayPtr4;
	int m_nAllocatedTotal;
	int m_nUnkInt27;
	int m_nUnkInt28;
	int m_nUnkInt29;
	int m_nUnkInt30;
	int m_nUnkInt31;
	int m_nUnkInt32;
	int m_nUnkInt33;
	int m_nUnkInt34;
	int m_nUnkInt35;
	int m_nUnkInt36;
	int m_nUnkInt37;
	int m_nUnkInt38;
	int m_nUnkInt39;
	int m_nUnkInt40;
	int m_nUnkInt41;
	int m_nUnkInt42;
	int m_nUnkInt43;
	int m_nUnkInt44;
	int m_nUnkInt45;
	int m_nUnkInt46;
	int m_nUnkInt47;
	int m_nUnkInt48;
	int m_nUnkInt49;
	int m_nUnkInt50;
	int m_nUnkInt51;
	int m_nUnkInt52;
	int m_nUnkInt53;
	int m_nUnkInt54;
	int m_nUnkInt55;
	int m_nUnkInt56;
	int m_nUnkInt57;
	int m_nUnkInt58;
	int m_nUnkInt59;
	SoundBufferInstance *m_pInstances;
	int m_nUnkInt61;
	int m_nUnkInt62;
	int m_nUnkInt63;
	int m_nUnkInt64;
	int m_nUnkInt65;
	int m_nUnkInt66;
	int m_nUnkInt67;
	int m_nUnkInt68;
	int m_nUnkInt69;
	int m_nUnkInt70;
	int m_nUnkInt71;
	int m_nUnkInt72;

public:
	//	>> 43EAD0
	void					Dump() const;

	//	>> 43CDA0
	static void				SetDefaultFxVolumeVar(float fVol);
	//	>> 43CDB0
	static void				SetDefaultAmbienceVolumeVar(float fVol);
	//	>> 43CDC0
	static void				SetDefaultMusicVolumeVar(float fVol);
	//	>> 43CDD0
	static void				SetDefaultSpeaksVolumeVar(float fVol);
	//	>> 43CDE0
	//static float			GetDefaultFxVolumeVar() { return g_fDefaultFxVolume; }
	//	>> 43CDF0
	//static float			GetDefaultAmbienceVolumeVar() { return g_fDefaultAmbienceVolume; }
	//	>> 43CE00
	//static float			GetDefaultMusicVolumeVar() { return g_fDefaultMusicVolume; }
	//	>> 43CE10
	//static float			GetDefaultSpeaksVolumeVar() { return g_fDefaultSpeaksVolume; }
	//	>> 43CE20
	void					_unkVolumeSet();
	//	>> 43CE90
	void					Init();
	//	>> 43CFD0
	void					sub_43CFD0(int* pOut);
	//	>> 43D060
	int						sub_43D060(int nUnk);
	//	>> 43D0D0
	bool					InitPrimarySoundFormat(int nChannels, int nSampleRate, int nBits);
	//	>> 43D180
	float					GetDefaultVolumeForType(int nWhat);
	//	>> 43D1D0
	void					SetGlobalPause(bool bPause);
	//	>> 43D200
	void					nullsub_35();
	//	>> 43D210
	static void				RememberSoundRenderer(int soundRendererId);
	//	>> 43D280
	static void				GetSoundRenderer();
	//	>> 43D310
	void					InitiDirectSound(char bUnk, int nSampleRate);
	//	>> 43D560
	void					SetListener3DPos(const Vector3<float>& vPos);
	//	>> 43D6F0
	void					SetListener3DOrient(const Quaternion<float>& orient);
	//	>> 43DFD0
	void					GetListener3DOrientation(Quaternion<float>& pOutOrient);
	//	>> 43E080
	void					SelectAudioRenderer(bool bUnk, int nSampleRate);
	//	>> 43E450
	void					_unkDestroyBuffers();
	//	>> 43E640
	void					WaitForSoftPause();
	//	>> 43E7B0
	int						sub_43E7B0();
	//	>> 43E800
	void					MeasureWaitForSoftPause();
};


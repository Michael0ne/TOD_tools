#pragma once

#include "stdafx.h"

/*
 *------------------------------------------------------------
 *------------------------------------------------------------
 *--------------- Handles streamed sounds---------------------
 *------------------------------------------------------------
 *--------- Game's alias is 'StreamedSoundBuffers' -----------
 *------------------------------------------------------------
 * -----------------------------------------------------------
*/
class SoundManager
{
private:
	int		m_nMaxConcurrentSounds;		//	+ 0x0 = originally 41, can be altered via configpc.txt.
	int		m_nCurrentlyPlaying;		//	+ 0x4 = number of currently playing sounds.
	int		m_nUnkInt2;
	int		m_nUnkInt3;
	int		m_nUnkInt4;
	int		m_nUnkInt5;
	int		m_nSoundSystem;				//	+ 0x = probably a enum. 3 is for DieselPower.
	int		m_nUnkInt6;
	int		m_nUnkInt7;
	int		m_nUnkInt8;
	int		m_nUnkInt9;
	int		m_nUnkInt10;
	int		m_nUnkInt11;
	int		m_nUnkInt12;
	int		m_nUnkInt13;
	int		m_nUnkInt14;
	int		m_nUnkInt15;
	int		m_nUnkInt16;
	int		m_nUnkInt17;
	int		m_nUnkInt18;
	int		m_nUnkInt19;
	int		m_nUnkInt20;
	int		m_nUnkInt21;
	int		m_nUnkInt22;
	int		m_nUnkInt23;
	int		m_nUnkInt24;
	int		m_nUnkInt25;
	int		m_nUnkInt26;
	int		m_nAllocatedTotal;			//	+ 0x = number of actual allocated StreamedSoundBuffers.

public:
	//	>> 43EAD0
	void					Dump();

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


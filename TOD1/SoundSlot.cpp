#include "SoundSlot.h"
#include "Scene.h"
#include "StreamedSoundBuffers.h"

#pragma message(TODO_IMPLEMENTATION)
const char* SoundSlot::GetStreamingSound()
{
	delete m_StreamingSoundName;

	if (!m_StreamingSoundResource)
		return nullptr;

	char buf[MAX_PATH] = {};
	strcpy(buf, m_StreamingSoundResource->AddResToOpenListAndReturnName());
	strcpy(strstr(buf, ".stream"), ".wav");

	m_StreamingSoundName = new char[strlen(buf) + 1];
	strcpy(m_StreamingSoundName, buf);

	return m_StreamingSoundName;
}

#pragma message(TODO_IMPLEMENTATION)
void SoundSlot::DeallocateStream()
{
	if (!m_StreamBuffer)
		return;

	//for (; m_SoundEmittersList.m_Elements;)
		//*(m_SoundEmittersList.m_Elements++).field_24 = NULL;
	//m_SoundEmittersList.m_CurrIndex = NULL;

	//delete m_StreamBuffer;
}

#pragma message(TODO_IMPLEMENTATION)
void SoundSlot::StopSound()
{
	/*
	if (!m_SoundResource ||
		!m_StreamBuffer ||
		!g_StreamedSoundBuffers ||
		!m_SoundResource->m_MonoStream)
		return;

	if (m_StreamBuffer)
		m_StreamBuffer->Stop(m_StreamBufferIndex);
	else
		if (m_SoundResource->m_MonoStream)
			m_SoundResource->m_MonoStream->Stop(m_StreamBufferIndex);

	if (Scene::SceneInstance->m_PlayMode == 1)
		DeallocateStream();
	*/
}

void SoundSlot::GetSoundName(String& outName)
{
	if (m_SoundResource)
		outName = m_SoundResource->AddResToOpenListAndReturnName();
	else
		if (m_Sound.m_nLength)
			outName = m_Sound;
		else
			outName = GetStreamingSound();
}

#pragma message(TODO_IMPLEMENTATION)
SoundSlot::~SoundSlot()
{
	MESSAGE_CLASS_DESTROYED(SoundSlot);

	StopSound();
	DeallocateStream();	//	NOTE: original code has this function inlined, so no need to dup it here.

	delete m_StreamingSoundName;

	//	TODO: delete lists...
}

String* SoundSlot::GetResourceName(String* strResName)
{
	return (*strResName = GetStreamingSound(), strResName);
}

#pragma message(TODO_IMPLEMENTATION)
SoundSlot::SoundSlot() : Node(NODE_MASK_EMPTY)
{
	MESSAGE_CLASS_CREATED(SoundSlot);

	m_SoundResource = nullptr;
	m_StreamingSoundResource = nullptr;
	field_64 = 1;
	m_Sound;
	field_7C = 1;

	//m_SoundEmittersList = List<class SoundEmitter>(0x1A300);

	m_Flags = m_Flags & 0xFF230111 | 0x1020111;
	m_Volume = 10000;
	m_Randompitch_Length = 0;
	m_MinRange = 1.0f;
	m_MaxRange = 100.0f;
	m_RollOff = 1.0f;
	m_Pitch = 10000;
	m_Randomvolume_Length = 0;
	m_StreamBuffer = nullptr;
	m_StreamingSoundName = nullptr;
	m_DopplerFactor = -1;
	m_StreamAllocationCallback = 0;
	m_LfeLevel = 0;
	m_PausedInstanceIndex = -1;
	
	//	TODO: initialize more lists here.
}
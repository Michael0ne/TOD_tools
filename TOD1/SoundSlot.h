#pragma once
#include "Node.h"
#include "SoundAsset.h"
#include "StreamedSoundInfoAsset.h"

class SoundSlot : public Node
{
protected:
    float						m_MinRange;
    float						m_MaxRange;
    float						m_RollOff;
    char*						m_StreamingSoundName;
    SoundAsset				   *m_SoundResource;
    int							field_64;
    String						m_Sound;
    StreamedSoundInfoAsset     *m_StreamingSoundResource;	//	TODO: i think type is correct, check again.
    int							field_7C;
    class StreamBuffer*			m_StreamBuffer;
    unsigned short				m_Volume;
    unsigned short				m_Pitch;
    int							m_StreamBufferIndex;
    int							m_PausedInstanceIndex;
    unsigned short				m_Randompitch_Length;
    unsigned short				m_Randomvolume_Length;
    Node*						m_StreamAllocationCallback;
    int							m_SoundEmittersList[4];
    unsigned int				m_Flags;
    unsigned char				m_DopplerFactor;
    unsigned char				m_LfeLevel;

    const char*					GetStreamingSound();	//	@89CA40
    void						DeallocateStream();	//	@89D0C0

private:
    void						StopSound();	//	@89D320
    void                        GetSoundName(String& outName);    //  @89F080

public:
    virtual ~SoundSlot();	//	@89F290
    virtual String*				GetResourceName(String*);	//	@89F270

    SoundSlot();	//	@89D590
};

ASSERT_CLASS_SIZE(SoundSlot, 176);
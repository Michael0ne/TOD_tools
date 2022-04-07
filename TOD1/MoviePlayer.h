#pragma once
#include "Node.h"
#include "FrameBuffer.h"
#include "Texture.h"

class MoviePlayer : public Node
{
    struct FrameInfo
    {
        float               m_ScaleX;
        float               m_ScaleY;
        int                 m_DisplacementX;
        int                 m_DisplacementY;
        float               m_Opacity;
        String              m_MovieName;
        char                m_MovieStopped;
        char                m_PlayingMovie;
        float               m_AspectRatioX;
        float               m_AspectRatioY;
        float               field_30;
        float               field_34;
        float               field_38;
        float               field_3C;
        Texture            *m_CurrentFrameTexture;
        File               *m_MovieFile;

        FrameInfo(); // @442F00
        ~FrameInfo();   //  @443060
        
        void                Stop(); //  @443010
        void                ProcessFrame(FrameBuffer* fb); //  @442A90
        void                OpenMovie();    //  @442F50
        bool                _442A70() const;    //  @442A70 //  NOTE: possibly 'IsStreamed'.
        void                PlayAllocated();    //  @442A80
    };

protected:
    FrameInfo               m_FrameInfo;
    float                   m_Volume;
    float                   m_ScaleX;
    float                   m_ScaleY;
    short                   m_DisplacementX;
    short                   m_DisplacementY;
    float                   m_Opacity;
    FrameBuffer            *m_FrameBuffer;
    Node                   *m_SubtitleNode;
    int                     m_StopEventScriptId;
    int                     m_StreamScriptId;
    union
    {
        struct
        {
            unsigned        HasFrameBuffer : 1;
            unsigned        MovieClosed : 1;
        };
    }                       m_Flags;

public:
    inline MoviePlayer() : Node(NODE_MASK_EMPTY)
    {
        MESSAGE_CLASS_CREATED(MoviePlayer);

        m_StreamScriptId = -1;
        m_StopEventScriptId = -1;
        m_DisplacementX = 0;
        m_ScaleX = 1;
        m_ScaleY = 1;
        m_DisplacementY = 0;
        m_Opacity = 1;
        m_FrameBuffer = nullptr;
        m_SubtitleNode = nullptr;
        m_Flags.HasFrameBuffer = 0;
        m_Flags.MovieClosed = 1;
        m_Volume = 0;
    }

    virtual ~MoviePlayer(); //  @88ADC0

    virtual void            Update();  //  @88AC60
    virtual void            Render();   //  @88AE50
    virtual String*         GetResourceName(String* resname);   //  @88B8F0

    const char*             GetMovie() const;   //  @88B020
    void                    SetRenderSubtitleNode(int* args);   //  @88ABC0
    void                    DeallocateStream(int* args);    //  @88B830
    void                    AllocateStream(int* args);  //  @88B7E0
    void                    PlayWithStopEventAllocated(int* args);  //  @88B790
    void                    PlayWithStopEvent(int* args);   //  @88B7B0
    void                    Stop(int* args);    //  @88AB90
    void                    PlayAllocated(int* args);   //  @88AB80

private:
    void                    Play(int args); //  @88B740
    inline void             DeallocateStream_Impl()
    {
        LogDump::LogA("Deallocating movie\n");
        if (Instance == this)
        {
            Instance = nullptr;
            MovieOpen = false;
            Scene::SceneInstance->_896BA0();
            Scene::SceneInstance->AllocateRewindBuffer();
        }
    }

public:
    static String           MovieName; // @A3D864
    static MoviePlayer     *Instance;   //  @A3D860
    static bool             MovieOpen;  //  @A35ED4

private:
    static void            *BinkHandle; //  @A35ED8
    static int              StopPressedCommand;    //  @A11C30
    static int              PlayPressedCommand; //  @A11C34
public:

    static MoviePlayer*     Create(AllocatorIndex); //  @88B880
    static void             Register(); //  @88B330
};

extern EntityType          *tMoviePlayer;    //  @A3D85C

ASSERT_CLASS_SIZE(MoviePlayer, 192);
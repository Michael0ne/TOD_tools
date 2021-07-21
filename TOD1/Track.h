#pragma once
#include "Node.h"
#include "BuiltinType.h"

class Track : public Node
{
protected:
    Vector4f	m_LatestPos;
    Orientation	m_LatestOrient;
    int			field_70;
    int			field_74;
    int			field_78;
    int			field_7C;
    int			field_80;
    int			field_84;
    int			field_88;
    Orientation	m_Orient_2;
    Vector4f	m_LatestTangent;
    union
    {
        struct
        {
            unsigned Looping : 1;
            unsigned ConnectEnds : 1;
            unsigned PingPong : 1;
            unsigned Spline : 1;
            unsigned CustomSpeed : 1;
            unsigned ShowExample : 1;
            unsigned Reverse : 1;
            unsigned _7 : 1;
            unsigned _8 : 1;
            unsigned _9 : 1;
            unsigned _10 : 1;
            unsigned _11 : 1;
            unsigned _12 : 1;
            unsigned _13 : 1;
            unsigned _14 : 1;
            unsigned _15 : 1;
            unsigned _16 : 1;
            unsigned _17 : 1;
            unsigned _18 : 1;
            unsigned _19 : 1;
            unsigned _20 : 1;
            unsigned _21 : 1;
            unsigned _22 : 1;
            unsigned _23 : 1;
            unsigned _24 : 1;
            unsigned _25 : 1;
            unsigned _26 : 1;
            unsigned _27: 1;
            unsigned _28 : 1;
            unsigned _29 : 1;
            unsigned _30 : 1;
        }       m_FlagBits;
        unsigned int    m_Flags;
    }           m_TrackFlags;
    float		m_Framerate;
    Node*		m_TrackPoint;
    float		m_LatestTime;

public:
    inline Track() : Node(NODE_MASK_POSITION)
    {
        MESSAGE_CLASS_CREATED(Track);

        m_LatestOrient = BuiltinType::Orient;
        m_Orient_2 = BuiltinType::Orient;
        m_TrackFlags.m_FlagBits.Looping = true;
        m_TrackFlags.m_FlagBits.ConnectEnds = false;
        m_TrackFlags.m_FlagBits.PingPong = true;
        m_TrackFlags.m_FlagBits.Spline = true;
        m_TrackFlags.m_FlagBits.CustomSpeed = true;
        m_TrackFlags.m_FlagBits.ShowExample = true;

        field_7C = field_80 = field_84 = field_88 = field_78 = field_74 = 0;
        field_70 = m_TrackFlags.m_FlagBits.Reverse ? -1 : 1;
        m_Framerate = 1.0f;
        m_LatestTangent = BuiltinType::ZeroVector;
    }

    static void     Register(); //  @923B30

private:
    const float     GetFramerate() const;   //  @9220F0
    void            SetFramerate(const float fr);   //  @9220D0

    const bool      GetIsLooping() const;   //  @921DA0
    void            SetIsLooping(const bool looping);   //  @921D70

    const bool      GetIsPingpong() const;  //  @921DD0
    void            SetIsPingpong(const bool pingpong); //  @921DB0

    const bool      GetIsReverse() const;   //  @921E00
    void            SetIsReverse(const bool reverse);   //  @921DE0

    const bool      GetIsConnectEnds() const;   //  @921E30
    void            SetIsConnectEnds(const bool connectends);   //  @921E10

    const bool      GetIsSpline() const;    //  @921CF0
    void            SetIsSpline(const bool spline); //  @921CD0

    const bool      GetIsCustomSpeed() const;   //  @921D20
    void            SetIsCustomSpeed(const bool customspeed);   //  @921D00

    const bool      GetIsShowExample() const;   //  @921D90
    void            SetIsShowExample(const bool showexample);   //  @921CB0

    void            UpdateTracking(float* args);  //  @9240B0
    const float     UpdateTracking_Impl(Node* trackpoint, const float time, const int, const float);    //  @923A70

    void            GetLatestPos(float* args) const;  //  @923F20

    void            GetLatestOrient(float* args) const;   //  @923F70

    void            GetLatestTrackPoint(int* args) const; //  @923FD0

    void            GetLatestTime(float* args) const; //  @923FE0

    void            GetNextFrameNode(int* args) const;  //  @924040
    Node*           GetNextFrameNode_Impl(Node* trackpoint, const int, const bool) const;  //  @922200

    void            GetLatestTangent(float* args) const;  //  @923FF0

    static Track*	Create();	//	@924070
};

extern EntityType* tTrack;	//	@A3E14C

ASSERT_CLASS_SIZE(Track, 188);
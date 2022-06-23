#pragma once
#include "stdafx.h"

struct BinkWrapper;
typedef BinkWrapper* BINKPTR;

struct BinkWrapper
{
    int32_t	Width;
    int32_t	Height;
    int32_t	Frames;
    int32_t	FrameNum;
    int32_t	LastFrameNum;

    int32_t	field_14;
    int32_t	field_18;
    int32_t	field_1C;
    int32_t	field_20;
    int32_t	field_24;
    int32_t	field_28;
    int32_t	field_2C;
    int32_t	field_30;
    int32_t	field_34;
    int32_t	field_38;
    int32_t	field_3C;
    int32_t	field_40;
    int32_t	field_44;
    int32_t	field_48;
    int32_t	field_4C;
    int32_t	field_50;
    int32_t	field_54;
    int32_t	field_58;
    int32_t	field_5C;
    int32_t	field_60;
    int32_t	field_64;
    int32_t	field_68;
    int32_t	field_6C;
    int32_t	field_70;
    int32_t	field_74;
    int32_t	field_78;
    int32_t	field_7C;
    int32_t	field_80;
    int32_t	field_84;
    int32_t	field_88;
    int32_t	field_8C;
    int32_t	field_90;
    int32_t	field_94;
    int32_t	field_98;
    int32_t	field_9C;
    int32_t	field_A0;
    int32_t	field_A4;
    int32_t	field_A8;
    int32_t	field_AC;
    int32_t	field_B0;
    int32_t	field_B4;
    int32_t	field_B8;
    int32_t	field_BC;
    int32_t	field_C0;
    int32_t	field_C4;
    int32_t	field_C8;
    int32_t	field_CC;
    int32_t	field_D0;
    int32_t	field_D4;
    int32_t	field_D8;
    int32_t	field_DC;
    int32_t	field_E0;
    int32_t	field_E4;
    int32_t	field_E8;
    int32_t	field_EC;
    int32_t	field_F0;
    int32_t	field_F4;
    int32_t	field_F8;
    int32_t	field_FC;
    int32_t	field_100;
    int32_t	field_104;
    int32_t	field_108;
    int32_t	field_10C;
    int32_t	field_110;
    int32_t	field_114;
    int32_t	field_118;
    int32_t	field_11C;
    int32_t	field_120;
    int32_t	field_124;
    int32_t	field_128;
    int32_t	field_12C;
    int32_t	field_130;
    int32_t	field_134;
    int32_t	field_138;
    int32_t	field_13C;
    int32_t	field_140;
    int32_t	field_144;
    int32_t	field_148;
    int32_t	field_14C;
    int32_t	field_150;
    int32_t	field_154;
    int32_t	field_158;
    int32_t	field_15C;
    int32_t	field_160;
    int32_t	field_164;
    int32_t	field_168;
    int32_t	field_16C;
    int32_t	field_170;
    int32_t	field_174;
    int32_t	field_178;
    int32_t	field_17C;
    int32_t	field_180;
    int32_t	field_184;
    int32_t	field_188;
    int32_t	field_18C;
    int32_t	field_190;
    int32_t	field_194;
    int32_t	field_198;
    int32_t	field_19C;
    int32_t	field_1A0;
    int32_t	field_1A4;
    int32_t	field_1A8;
    int32_t	field_1AC;
    int32_t	field_1B0;
    int32_t	field_1B4;
    int32_t	field_1B8;
    int32_t	field_1BC;
    int32_t	field_1C0;
    int32_t	field_1C4;
    int32_t	field_1C8;
    int32_t	field_1CC;
    int32_t	field_1D0;
    int32_t	field_1D4;
    int32_t	field_1D8;
    int32_t	field_1DC;
    int32_t	field_1E0;
    int32_t	field_1E4;
    int32_t	field_1E8;
    int32_t	field_1EC;
    int32_t	field_1F0;
    int32_t	field_1F4;
    int32_t	field_1F8;
    int32_t	field_1FC;
    int32_t	field_200;
    int32_t	field_204;
    int32_t	field_208;
    int32_t	field_20C;
    int32_t	field_210;
    int32_t	field_214;
    int32_t	field_218;
    int32_t	field_21C;
    int32_t	field_220;
    int32_t	field_224;
    int32_t	field_228;
    int32_t	field_22C;
    int32_t	field_230;
    int32_t	field_234;
    int32_t	field_238;
    int32_t	field_23C;
    int32_t	field_240;
    int32_t	field_244;
    int32_t	field_248;
    int32_t	field_24C;
    int32_t	field_250;
    int32_t	field_254;
    int32_t	field_258;
    int32_t	field_25C;
    int32_t	field_260;
    int32_t	field_264;
    int32_t	field_268;
    int32_t	field_26C;
    int32_t	field_270;
    int32_t	field_274;
    int32_t	field_278;
    int32_t	field_27C;
    int32_t	field_280;
    int32_t	field_284;
    int32_t	field_288;
    int32_t	field_28C;
    int32_t	field_290;
    int32_t	field_294;
    int32_t	field_298;
    int32_t	field_29C;
    int32_t	field_2A0;
    int32_t	field_2A4;
    int32_t	field_2A8;
    int32_t	field_2AC;
    int32_t	field_2B0;
    int32_t	field_2B4;
    int32_t	field_2B8;
    int32_t	field_2BC;
    int32_t	field_2C0;
    int32_t	field_2C4;
    int32_t	field_2C8;
    int32_t	field_2CC;
    int32_t	field_2D0;
    int32_t	field_2D4;
    int32_t	field_2D8;
    int32_t	field_2DC;
    int32_t	field_2E0;
    int32_t	field_2E4;
    int32_t	field_2E8;
    int32_t	field_2EC;
    int32_t	field_2F0;
    int32_t	field_2F4;
    int32_t	field_2F8;
    int32_t	field_2FC;
    int32_t	field_300;
    int32_t	field_304;
    int32_t	field_308;
    int32_t	field_30C;
    int32_t	field_310;
    int32_t	field_314;
    int32_t	field_318;
    int32_t	field_31C;
    int32_t	field_320;
    int32_t	field_324;
    int32_t	field_328;
    int32_t	field_32C;
    int32_t	field_330;
    int32_t	field_334;
    int32_t	field_338;
    int32_t	field_33C;
    int32_t	field_340;
    int32_t	field_344;
    int32_t	field_348;
    int32_t	field_34C;
    int32_t	field_350;
    int32_t	field_354;
    int32_t	field_358;
    int32_t	field_35C;
    int32_t	field_360;
    int32_t	field_364;
    int32_t	field_368;
    int32_t	field_36C;
    int32_t	field_370;
    int32_t	field_374;
    int32_t	field_378;
    int32_t	field_37C;
    int32_t	field_380;
    int32_t	field_384;
    int32_t	field_388;
    int32_t	field_38C;
    int32_t	field_390;
    int32_t	field_394;
    int32_t	field_398;

    #pragma message(TODO_IMPLEMENTATION)
    static BINKPTR BinkOpen(HANDLE binkHandle, const int binkOpenFlags)
    {
        MESSAGE_NOT_IMPLEMENTED(__func__);
        return NULL;
    }

    #pragma message(TODO_IMPLEMENTATION)
    static int32_t BinkGetError()
    {
        MESSAGE_NOT_IMPLEMENTED(__func__);
        return NULL;
    }

    #pragma message(TODO_IMPLEMENTATION)
    static void BinkClose(BINKPTR binkInterfacePtr)
    {
        MESSAGE_NOT_IMPLEMENTED(__func__);
    }

    #pragma message(TODO_IMPLEMENTATION)
    static void BinkDoFrame(BINKPTR binkInterfacePtr)
    {
        MESSAGE_NOT_IMPLEMENTED(__func__);
    }

    #pragma message(TODO_IMPLEMENTATION)
    static void BinkCopyToBuffer(
        BINKPTR binkInterfacePtr,
        void* destinationBuffer,
        const int32_t destinationPitch,
        const uint32_t destinationHeight,
        const uint32_t destinationX,
        const uint32_t destinationY,
        const uint32_t flags)
    {
        MESSAGE_NOT_IMPLEMENTED(__func__);
    }

    #pragma message(TODO_IMPLEMENTATION)
    static void BinkNextFrame(BINKPTR binkInterfacePtr)
    {
        MESSAGE_NOT_IMPLEMENTED(__func__);
    }

    #pragma message(TODO_IMPLEMENTATION)
    static bool BinkWait(BINKPTR binkInterfacePtr)
    {
        MESSAGE_NOT_IMPLEMENTED(__func__);
        return false;
    }
};

ASSERT_CLASS_SIZE(BinkWrapper, 0x39C);
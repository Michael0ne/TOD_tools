#include "QuadTree.h"
#include "Scene.h"

QuadTreeInfo* QuadTreeInfo::Instance;
uint32_t QuadTree::TreesCreated;

QuadTree::QuadTree(const uint32_t nodes, const uint32_t a2)
{
    if (!QuadTreeInfo::Instance)
    {
        QuadTreeInfo::Instance = new QuadTreeInfo();
        QuadTreeInfo::Instance->field_8 = (uint8_t*)MemoryManager::AllocatorsList[DEFAULT]->AllocateAligned(0x8000, 64, __FILE__, __LINE__);
        QuadTreeInfo::Instance->field_4 = (uint8_t*)MemoryManager::AllocatorsList[SCRATCHPAD]->AllocateAligned(256, 16, __FILE__, __LINE__);
    }

    LeafsTotal = nodes * 2;
    field_8 = a2;
    field_C = (int32_t)(Math::RoundWeird((float)a2) / Math::RoundWeird(2.f));
    field_10 = (int32_t)(Math::RoundWeird((float)(nodes / a2)) / Math::RoundWeird(2.f) + 1.0);

    TreesCreated++;

    field_0 = _89A3E0();

    Scene::QuadTreeNode& kdtreeNode = Scene::MainQuadTreeNodes[field_0];
    kdtreeNode.field_1B &= 252 | 2;
    kdtreeNode.field_18 = -1;
    kdtreeNode.field_1A = 0;

    for (uint32_t i = 0; i < 4; ++i)
    {
        kdtreeNode.field_0[i] = -1;
        kdtreeNode.field_8[i] = 0;
    }

    NoFreeNodes = false;

    QuadTreeInfo::Instance->field_24 = 0;
    QuadTreeInfo::Instance->field_20 = -1;
}

int16_t QuadTree::_89A3E0()
{
    if (Scene::_A120E8 == -1)
    {
        if (!NoFreeNodes)
        {
            LogDump::LogA("\n\n\t*** WARNING: QuadTree ran out of nodes (%d nodes allocated)! ***\n\n\n", Scene::QuadTreeNodesAllocated);
            NoFreeNodes = true;
        }

        return -1;
    }
    else
    {
        Scene::_A120E8 = Scene::MainQuadTreeNodes[Scene::_A120E8].field_0[0];
        return Scene::_A120E8;
    }
}

QuadTreeInfo::QuadTreeInfo()
{
    field_C = 0.f;
    field_10 = 0.f;
    field_14 = 0.f;
    field_0 = 0;
    field_4 = 0;
    field_18 = 0.f;
    field_1C = 0.f;
    field_20 = -1;
    field_24 = 0;
    field_28 = 0;
    field_30 = 0;
    field_34 = 0;
    field_38 = 0;
    field_3C = 0;
    field_40 = 0;
    field_44 = 0;
    field_48 = 0;
    field_4C = 0;
    field_50 = 0;
    field_54 = 0;
    field_58 = 0;
}
#include "ParticleSystem.h"
#include "ScriptDatabase.h"
#include "BuiltinType.h"
#include "FrameBuffer.h"
#include "NumberType.h"
#include "TruthType.h"
#include "IntegerType.h"
#include "StringType.h"
#include "VectorType.h"
#include "GeometryEffect.h"
#include "Scene.h"
#include "VirtualHud.h"
#include "GfxInternal.h"

bool ParticleSystem::LodAndFade;
Vector4f ParticleSystem::CameraOrigin = BuiltinType::ZeroVector;

EntityType* tParticleSystem;

ParticleSystem::ParticleSystem() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
    MESSAGE_CLASS_CREATED(ParticleSystem);

    field_E4 = 0;
    field_E8 = 1;
    field_EC = -1;
    m_FrameBuffer = nullptr;
    m_IndexInSceneList = -1;

    const Orientation orient;
    //  TODO: make quaternion from 2 vectors.
    SetOrient(orient);

    LodAndFade = Script::LodAndFade;

    m_Properties.CalculateEmitterLength();
}

ParticleSystem::~ParticleSystem()
{
    MESSAGE_CLASS_DESTROYED(ParticleSystem);

    delete m_FrameBuffer;
}

void ParticleSystem::Instantiate()
{
}

void ParticleSystem::Render()
{
}

#pragma message(TODO_IMPLEMENTATION)
void ParticleSystem::SpawnWithEmitter()
{
    Camera* camera = Scene::SceneInstance->m_ActiveCamera;
    if (!camera)
        return;

    /*
    *        0    1    2    3
    *   0 - 1_1, 1_2, 1_3, 1_4
    *   1 - 2_1, 2_2, 2_3, 2_4
    *   2 - 3_1, 3_2, 3_3, 3_4
    *   3 - 4_1, 4_2, 4_3, 4_4
    */
    DirectX::XMMATRIX mat;

    if (m_Properties.m_Flags.UsesVirtualHud)
    {
        Vector4f screenCoords;
        DirectX::XMMATRIX nodeMat;

        GetWorldMatrix(nodeMat);
        VirtualHud::VirtualHudInstance.LimitCoordsToScreen(screenCoords, *(Vector4f*)&nodeMat.r[3].m128_f32);

        *(Vector4f*)&mat.r[3].m128_f32 = screenCoords;
        *(Vector4f*)&mat.r[2].m128_f32 = *(Vector4f*)&nodeMat.r[3].m128_f32;
    }
    else
    {
        const ScreenResolution screenResolution = g_GfxInternal->GetViewportResolution();
    }

    Orientation orient;
    GetOrient(orient);
    m_Properties.SpawnParticle(camera->m_NodePosition, *(Vector4f*)&mat.r[3].m128_f32[0], orient);
}

const float ParticleSystem::GetEmitterFadeThreshold() const
{
    return (float)(m_Properties.m_EmitterFadeThreshold * (1.f / 255.f));
}

void ParticleSystem::SetEmitterFadeThreshold(const float threshold)
{
    if (threshold < 0.12f && !m_Flags.LSR)
    {
        SetFlags(0x100);
        m_Properties.m_EmitterFadeThreshold = (char)(0.12f * 255.f + 0.5f);
        m_Properties.CalculateEmitterLength();

        return;
    }

    if (threshold >= 0.f)
    {
        if (threshold <= 1.f)
        {
            m_Properties.m_EmitterFadeThreshold = (char)(0.12f * 255.f + 0.5f);
            m_Properties.CalculateEmitterLength();
            return;
        }

        m_Properties.m_EmitterFadeThreshold = -1;
        m_Properties.CalculateEmitterLength();
    }
    else
    {
        m_Properties.m_EmitterFadeThreshold = 0;
        m_Properties.CalculateEmitterLength();
    }
}

const float ParticleSystem::GetEmitterRadius() const
{
    return CharToFloatLimited(m_Properties.m_EmitterRadius, 100.f);
}

void ParticleSystem::SetEmitterRadius(const float radius)
{
    if (!m_Flags.ERR && radius == 20.f)
    {
        SetFlags(0x400);
        m_Properties.m_EmitterRadius = FloatToChar(322.56f);
        m_Properties.CalculateEmitterLength();
        return;
    }

    if (radius >= 0.f)
        m_Properties.m_EmitterRadius = FloatToChar(322.56f * (radius > 100.f ? 100.f : radius));
    else
        m_Properties.m_EmitterRadius = 0;

    m_Properties.CalculateEmitterLength();
}

const bool ParticleSystem::UseRealTime() const
{
    return m_Properties.m_Flags.UseRealTime;
}

void ParticleSystem::SetUseRealTime(const bool enabled)
{
    m_Properties.m_Flags.UseRealTime = enabled;
}

const bool ParticleSystem::GetSep6() const
{
    return false;
}

void ParticleSystem::SetSep6(const bool)
{
}

const int ParticleSystem::GetMaxParticles() const
{
    return m_Properties.m_MaxParticles - 1;
}

void ParticleSystem::SetMaxParticles(int maxparticles)
{
    maxparticles++;

    ParticleA* particlesArray = (ParticleA*)m_Properties.m_ParticlesArray;
    m_Properties.m_MaxParticles = maxparticles;

    if (!particlesArray)
        return;

    while (particlesArray)
    {
        particlesArray->SetParticlesListSize(maxparticles);
        particlesArray = (ParticleA*)particlesArray->m_Next;
    }
}

const float ParticleSystem::GetEmitterLifeTime() const
{
    return ShortToFloatLimited(m_Properties.m_EmitterLifeTime, 60.f);
}

void ParticleSystem::SetEmitterLifeTime(const float emitterlifetime)
{
}

const bool ParticleSystem::GetSep3() const
{
    return false;
}

void ParticleSystem::SetSep3(const bool)
{
}

#pragma message(TODO_IMPLEMENTATION)
const float ParticleSystem::GetParticleGravity() const
{
    return 0.f;
}

#pragma message(TODO_IMPLEMENTATION)
void ParticleSystem::SetParticleGravity(const float gravity)
{
}

void ParticleSystem::TriggerWithPosition(int* args)
{
}

void ParticleSystem::TriggerWithEntity(int* args)
{
}

void ParticleSystem::TriggerWithEntityPosition(int* args)
{
}

void ParticleSystem::TriggerDefault(int* args)
{
}

void ParticleSystem::sKillEmitters(int* args)
{
}

void ParticleSystem::StopEmitters(int* args)
{
}

void ParticleSystem::SpawnParticles(int* args)
{
}

#pragma message(TODO_IMPLEMENTATION)
void ParticleSystem::KillDefault(int* args)
{
}

void ParticleSystem::KillAll(int* args)
{
    m_Properties.KillAll();
}

#pragma message(TODO_IMPLEMENTATION)
void ParticleSystem::KillEmitters(const bool onlyUnloaded, const bool onlyEffects)
{
    auto entity = (ParticleSystem*)g_AssetManager->FindFirstEntity();
    if (!entity)
        return;

    while (true)
    {
        EntityType* scriptEntity = entity->m_ScriptEntity;
        if (!scriptEntity)
        {
            entity = (ParticleSystem*)g_AssetManager->FindNextEntity(entity);
            if (!entity)
                return;
        }
    }
}

void ParticleSystem::Register()
{
    tParticleSystem = new EntityType("ParticleSystem");
    tParticleSystem->InheritFrom(tNode);
    tParticleSystem->SetCreator((CREATOR)Create);

    tParticleSystem->RegisterProperty(tNUMBER, "EmitterFadeThreshold", (EntityGetterFunction)&GetEmitterFadeThreshold, (EntitySetterFunction)&SetEmitterFadeThreshold, "control=slider|min=0|max=0.5|step=0.01");
    tParticleSystem->RegisterProperty(tNUMBER, "EmitterRadius", (EntityGetterFunction)&GetEmitterRadius, (EntitySetterFunction)&SetEmitterRadius, "control=slider|min=0|max=50");
    tParticleSystem->RegisterProperty(tTRUTH, "UseRealTime", (EntityGetterFunction)&UseRealTime, (EntitySetterFunction)&SetUseRealTime, "control=truth");

    tParticleSystem->RegisterProperty(tTRUTH, "sep6", (EntityGetterFunction)&GetSep6, (EntitySetterFunction)&SetSep6, "control=drawline|name=Emitter Control");
    tParticleSystem->RegisterProperty(tINTEGER, "MaxParticles", (EntityGetterFunction)&GetMaxParticles, (EntitySetterFunction)&SetMaxParticles, "control=string");
    tParticleSystem->RegisterProperty(tNUMBER, "EmitterLifeTime", (EntityGetterFunction)&GetEmitterLifeTime, (EntitySetterFunction)&SetEmitterLifeTime, "control=slider|min=0|max=60");
    //tParticleSystem->RegisterProperty(tNUMBER, "EmitterDelay", (EntityGetterFunction)&GetEmitterDelay, (EntitySetterFunction)&SetEmitterDelay, "control=slider|min=0|max=10");
    //tParticleSystem->RegisterProperty(tTRUTH, "LinkNext", (EntityGetterFunction)&LinkNext, (EntitySetterFunction)&SetLinkNext, "control=truth");
    //tParticleSystem->RegisterProperty(tTRUTH, "InitialTrigger", (EntityGetterFunction)&InitialTrigger, (EntitySetterFunction)&SetInitialTrigger, "control=truth");

    //tParticleSystem->RegisterProperty(tTRUTH, "sep1", (EntityGetterFunction)&GetSep1, (EntitySetterFunction)&SetSep1, "control=drawline|name=Particle Appearence");
    //tParticleSystem->RegisterProperty(tSTRING, "Texture", (EntityGetterFunction)&GetTexture, (EntitySetterFunction)&SetTexture, "control=resource|type=*.bmp");
    //tParticleSystem->RegisterProperty(tSTRING, "ModelRes", (EntityGetterFunction)&GetModelRes, (EntitySetterFunction)&SetModelRes, "control=resource|type=*.model");
    //tParticleSystem->RegisterProperty(tNUMBER, "Opacity", (EntityGetterFunction)&GetOpacity, (EntitySetterFunction)&SetOpacity, "control=slider|min=0|max=1");
    //tParticleSystem->RegisterProperty(tNUMBER, "OpacityVariation", (EntityGetterFunction)&GetOpacityVariation, (EntitySetterFunction)&SetOpacityVariation, "control=slider|min=0|max=100|step=0.5");
    //tParticleSystem->RegisterProperty(tINTEGER, "BlendMode", (EntityGetterFunction)&GetBlendMode, (EntitySetterFunction)&SetBlendMode, "control=dropdown|Normal=0|Add=1");
    //tParticleSystem->RegisterProperty(tVECTOR, "ColorVector", (EntityGetterFunction)&GetColorVector, (EntitySetterFunction)&SetColorVector, "control=colorrgb");

    //tParticleSystem->RegisterProperty(tTRUTH, "sep2", (EntityGetterFunction)&GetSep2, (EntitySetterFunction)&SetSep2, "control=drawline|name=Particle Life Time");
    //tParticleSystem->RegisterProperty(tNUMBER, "LifeTime", (EntityGetterFunction)&GetLifeTime, (EntitySetterFunction)&SetLifeTime, "control=slider|min=0|max=10");
    //tParticleSystem->RegisterProperty(tNUMBER, "LifeTimeVariation", (EntityGetterFunction)&GetLifeTimeVariation, (EntitySetterFunction)&SetLifeTimeVariation, "control=slider|min=0|max=100|step=0.5");
    //tParticleSystem->RegisterProperty(tTRUTH, "ExplicitBirthOnly", (EntityGetterFunction)&ExplicitBirthOnly, (EntitySetterFunction)&SetExplicitBirthOnly, "control=truth");
    //tParticleSystem->RegisterProperty(tNUMBER, "BirthRate", (EntityGetterFunction)&GetBirthRate, (EntitySetterFunction)&SetBirthRate, "control=slider|min=0|max=1");
    //tParticleSystem->RegisterProperty(tNUMBER, "BirthRateVariation", (EntityGetterFunction)&GetBirthRateVariation, (EntitySetterFunction)&SetBirthRateVariation, "control=slider|min=0|max=100|step=0.5");
    //tParticleSystem->RegisterProperty(tNUMBER, "BirthDistance", (EntityGetterFunction)&GetBirthDistance, (EntitySetterFunction)&SetBirthDistance, "control=slider|min=0|max=1");
    //tParticleSystem->RegisterProperty(tNUMBER, "BirthDistFade", (EntityGetterFunction)&GetBirthDistFade, (EntitySetterFunction)&SetBirthDistFade, "control=slider|min=0|max=1");
    //tParticleSystem->RegisterProperty(tNUMBER, "FadeInTime", (EntityGetterFunction)&GetFadeInTime, (EntitySetterFunction)&SetFadeInTime, "control=slider|min=0|max=10");
    //tParticleSystem->RegisterProperty(tNUMBER, "FadeOutTime", (EntityGetterFunction)&GetFadeOutTime, (EntitySetterFunction)&SetFadeOutTime, "control=slider|min=0|max=10");

    //tParticleSystem->RegisterProperty(tTRUTH, "sep5", (EntityGetterFunction)&GetSep5, (EntitySetterFunction)&SetSep5, "control=drawline|name=Particle Size");
    //tParticleSystem->RegisterProperty(tNUMBER, "Size", (EntityGetterFunction)&GetSize, (EntitySetterFunction)&SetSize, "control=slider|min=0|max=10");
    //tParticleSystem->RegisterProperty(tNUMBER, "SizeVariation", (EntityGetterFunction)&GetSizeVariation, (EntitySetterFunction)&SetSizeVariation, "control=slider|min=0|max=400|step=2");
    //tParticleSystem->RegisterProperty(tNUMBER, "GrowthFactor", (EntityGetterFunction)&GetGrowthFactor, (EntitySetterFunction)&SetGrowthFactor, "control=slider|min=-99|max=201");
    //tParticleSystem->RegisterProperty(tTRUTH, "GrowthFade", (EntityGetterFunction)&GrowthFade, (EntitySetterFunction)&SetGrowthFade, "control=truth");

    //tParticleSystem->RegisterProperty(tTRUTH, "sep4", (EntityGetterFunction)&GetSep4, (EntitySetterFunction)&SetSep4, "control=drawline|name=Initial State");
    //tParticleSystem->RegisterProperty(tNUMBER, "RandomOffsetX", (EntityGetterFunction)&GetRandomOffsetX, (EntitySetterFunction)&SetRandomOffsetX, "control=slider|min=0|max=1");
    //tParticleSystem->RegisterProperty(tNUMBER, "RandomOffsetY", (EntityGetterFunction)&GetRandomOffsetY, (EntitySetterFunction)&SetRandomOffsetY, "control=slider|min=0|max=1");
    //tParticleSystem->RegisterProperty(tNUMBER, "RandomOffsetZ", (EntityGetterFunction)&GetRandomOffsetZ, (EntitySetterFunction)&SetRandomOffsetZ, "control=slider|min=0|max=1");
    //tParticleSystem->RegisterProperty(tNUMBER, "DirVariation", (EntityGetterFunction)&GetDirVariation, (EntitySetterFunction)&SetDirVariation, "control=slider|min=0|max=180");
    //tParticleSystem->RegisterProperty(tNUMBER, "DirScaleX", (EntityGetterFunction)&GetDirScaleX, (EntitySetterFunction)&SetDirScaleX, "control=slider|min=0|max=100|step=0.5");
    //tParticleSystem->RegisterProperty(tNUMBER, "DirScaleY", (EntityGetterFunction)&GetDirScaleY, (EntitySetterFunction)&SetDirScaleY, "control=slider|min=0|max=100|step=0.5");
    //tParticleSystem->RegisterProperty(tNUMBER, "DirScaleZ", (EntityGetterFunction)&GetDirScaleZ, (EntitySetterFunction)&SetDirScaleZ, "control=slider|min=0|max=100|step=0.5");
    //tParticleSystem->RegisterProperty(tTRUTH, "DirScaleZRing", (EntityGetterFunction)&DirScaleZRing, (EntitySetterFunction)&SetDirScaleZRing, "control=truth");
    //tParticleSystem->RegisterProperty(tNUMBER, "Speed", (EntityGetterFunction)&GetSpeed, (EntitySetterFunction)&SetSpeed, "control=slider|min=0|max=10");
    //tParticleSystem->RegisterProperty(tNUMBER, "SpeedVariation", (EntityGetterFunction)&GetSpeedVariation, (EntitySetterFunction)&SetSpeedVariation, "control=slider|min=0|max=100|step=0.5");
    //tParticleSystem->RegisterProperty(tNUMBER, "InheritEmitterSpeed", (EntityGetterFunction)&GetInheritEmitterSpeed, (EntitySetterFunction)&SetInheritEmitterSpeed, "control=slider|min=0|max=100");
    //tParticleSystem->RegisterProperty(tNUMBER, "RotationX", (EntityGetterFunction)&GetRotationX, (EntitySetterFunction)&SetRotationX, "control=slider|min=-2|max=2");
    //tParticleSystem->RegisterProperty(tNUMBER, "RotationY", (EntityGetterFunction)&GetRotationY, (EntitySetterFunction)&SetRotationY, "control=slider|min=-2|max=2");
    //tParticleSystem->RegisterProperty(tNUMBER, "RotationZ", (EntityGetterFunction)&GetRotationZ, (EntitySetterFunction)&SetRotationZ, "control=slider|min=-2|max=2");
    //tParticleSystem->RegisterProperty(tNUMBER, "RotationVariation", (EntityGetterFunction)&GetRotationVariation, (EntitySetterFunction)&SetRotationVariation, "control=slider|min=0|max=2");
    //tParticleSystem->RegisterProperty(tTRUTH, "is_3d", (EntityGetterFunction)&Is3D, (EntitySetterFunction)&SetIs3D, "control=truth");
    //tParticleSystem->RegisterProperty(tINTEGER, "prioritylayer", (EntityGetterFunction)&GetPriorityLayer, (EntitySetterFunction)&SetPriorityLayer, "control=dropdown|Plane0=0|Plane1=1|Plane2=2|Plane3=3|Plane4=4");
    //tParticleSystem->RegisterProperty(tTRUTH, "Has2DEmitter", (EntityGetterFunction)&Has3DEmitter, (EntitySetterFunction)&SetHas3DEmitter, "control=truth");
    //tParticleSystem->RegisterProperty(tTRUTH, "use_virtual_hud", (EntityGetterFunction)&UseVirtualHud, (EntitySetterFunction)&SetUseVirtualHud, "control=truth");
    //tParticleSystem->RegisterProperty(tINTEGER, "AxisAlign", (EntityGetterFunction)&GetAxisAlign, (EntitySetterFunction)&SetAxisAlign, "control=dropdown|None=0|X=1|Y=2|Z=3|XY=4|Speed=5");
    //tParticleSystem->RegisterProperty(tNUMBER, "TumbleSpeed", (EntityGetterFunction)&GetTumbleSpeed, (EntitySetterFunction)&SetTumbleSpeed, "control=slider|min=0|max=1");
    //tParticleSystem->RegisterProperty(tNUMBER, "TumbleAmp", (EntityGetterFunction)&GetTumbleAmp, (EntitySetterFunction)&SetTumbleAmp, "control=slider|min=0|max=1");
    //tParticleSystem->RegisterProperty(tNUMBER, "Turbulence", (EntityGetterFunction)&GetTurbulence, (EntitySetterFunction)&SetTurbulence, "control=slider|min=0|max=1");

    //tParticleSystem->RegisterProperty(tTRUTH, "sep3", (EntityGetterFunction)&GetSep3, (EntitySetterFunction)&SetSep3, "control=drawline|name=Static Forces");
    //tParticleSystem->RegisterProperty(tNUMBER, "ParticleGravity", (EntityGetterFunction)&GetParticleGravity, (EntitySetterFunction)&SetParticleGravity, "control=slider|min=-20|max=20");
    //tParticleSystem->RegisterProperty(tNUMBER, "GlobalWindFactor", (EntityGetterFunction)&GetGlobalWindFactor, (EntitySetterFunction)&SetGlobalWindFactor, "control=slider|min=0|max=100");
    //tParticleSystem->RegisterProperty(tNUMBER, "Friction", (EntityGetterFunction)&GetFriction, (EntitySetterFunction)&SetFriction, "control=slider|min=0|max=100|step=2");

    tParticleSystem->RegisterScript("Trigger(vector,quaternion)", (EntityFunctionMember)&TriggerWithPosition);
    tParticleSystem->RegisterScript("Trigger(entity)", (EntityFunctionMember)&TriggerWithEntity);
    tParticleSystem->RegisterScript("Trigger(entity,vector,quaternion)", (EntityFunctionMember)&TriggerWithEntityPosition);
    tParticleSystem->RegisterScript("TriggerDefault", (EntityFunctionMember)&TriggerDefault, 0, 0, 0, "control=button|text=Trigger");
    tParticleSystem->RegisterScript("KillEmitters(entity)", (EntityFunctionMember)&sKillEmitters);
    tParticleSystem->RegisterScript("StopEmitters(entity)", (EntityFunctionMember)&StopEmitters);
    tParticleSystem->RegisterScript("SpawnParticles(entity,integer)", (EntityFunctionMember)&SpawnParticles);
    tParticleSystem->RegisterScript("KillDefault", (EntityFunctionMember)&KillDefault);
    tParticleSystem->RegisterScript("KillAll", (EntityFunctionMember)&KillAll);

    tParticleSystem->PropagateProperties();
}

ParticleSystem* ParticleSystem::Create(AllocatorIndex)
{
    return new ParticleSystem;
}

float ParticleSystem::CharToFloatLimited(const unsigned char valueFrom, const float limitValue)
{
    const unsigned char fac0 = valueFrom & 7;
    const unsigned char fac1 = valueFrom & 248 | (fac0 != 0 ? 256 : 0);
    if (fac0 != 0)
        return (float)((fac1 << (fac0 - 1)) * (limitValue * 0.000031001986f));
    else
        return (float)(fac1 * (limitValue * 0.000031001986f));
}

float ParticleSystem::ShortToFloatLimited(const unsigned short valueFrom, const float limitValue)
{
    const unsigned char fac0 = valueFrom & 15;
    const unsigned char fac1 = valueFrom & 4080 | (fac0 != 0 ? 4096 : 0);
    if (fac0 != 0)
        return (float)((fac1 << (fac0 - 1)) * (limitValue * 0.0000000074651609f));
    else
        return (float)(fac1 * (limitValue * 0.0000000074651609f));
}

char ParticleSystem::FloatToChar(float value)
{
    //  TODO: check this!
    value += 0.5f;
    int valueInt = (int)value;
    int valueIntCopy = valueInt;

    unsigned char divisions = 0;
    while (valueInt >= 512)
    {
        valueInt /= 2;
        divisions++;
    }

    valueInt = (1 << divisions) + 2;
    valueInt += valueIntCopy;
    valueInt >>= divisions;
    valueInt &= 248;
    valueInt |= divisions >= 256 ? divisions : divisions + 1;

    return valueInt;
}

void ParticleSystem::KillNewEffects(const float gameTime)
{
    Entity* effectEntity = g_AssetManager->FindFirstEntity();
    while (effectEntity)
    {
        if (effectEntity->m_ScriptEntity)
        {
            EntityType* entScript = effectEntity->m_ScriptEntity;
            while (tParticleSystem != entScript)
            {
                entScript = entScript->Parent;
                if (!entScript)
                    break;
            }

            if (entScript)
            {
                Particle* particle = ((ParticleSystem*)effectEntity)->m_Properties.m_ParticlesArray;
                while (particle)
                {
                    Particle* particleCopy = particle;
                    if (particle->m_StartTime >= gameTime)
                        ((ParticleSystem*)effectEntity)->m_Properties.RemoveParticle((ParticleA*)particle);
                    particle = particleCopy->m_Next;
                }
            }

            entScript = effectEntity->m_ScriptEntity;
            while (tGeometryEffect != entScript)
            {
                entScript = entScript->Parent;
                if (!entScript)
                    break;
            }

            if (entScript)
            {
                Effect* effect = ((GeometryEffect*)effectEntity)->m_Effect;
                while (effect)
                {
                    Effect* parentEffect = effect->m_ParentEffect;
                    if (effect->m_StartTime >= gameTime)
                        ((GeometryEffect*)effectEntity)->RemoveEffect(effect);
                    effect = parentEffect;
                }
            }
        }

        effectEntity = g_AssetManager->FindNextEntity(effectEntity);
    }
}

#pragma message(TODO_IMPLEMENTATION)
ParticleSystemInfo::ParticleSystemInfo()
{
    MESSAGE_CLASS_CREATED(ParticleSystemInfo);

    field_54 = 0;
    field_58 = 0;
    field_5C = 0;
    field_5E = 0;
    m_GlobalWindFactor = 0;
    m_FadeOutTime = 0;
    //  m_BirthRateVariation =  //  TODO: encode value from float to char.
    m_Opacity = -1;
    m_FadeInTime = 0;
    m_BirthDistance = 0;
    m_OpacityVariation = 0;
    m_SizeVariation = 0;
    m_LifeTimeVariation = 0;
    m_RotationVariation = 0;
    m_SpeedVariation = 0;
    m_DirVariation = 0;
    m_DirScaleX = -1;
    m_BirthDistFade = 0;
    m_DirScaleY = -1;
    m_DirScaleZ = -1;
    m_Turbulence = 0;
    m_InheritEmitterSpeed = 0;
    //m_TumbleSpeed =   //  TODO: encode value from float to char.
    m_EmitterDelay = 0;
    m_TumbleAmp = 0;
    m_EmitterFadeThreshold = 31;
    //m_EmitterRadius =     //  TODO: encode value from float to char.
    m_Color = -1;
    m_PriorityLayer = 0;
    m_ParticlesArray = nullptr;
    m_Particles = 0;
    m_MeshTexture = 0;
    m_Mesh = 0;
    m_MaxParticles = 51;
}

void ParticleSystemInfo::CalculateEmitterLength()
{
}

ParticleA* ParticleSystemInfo::CreateNewParticle()
{
    ParticleA* newParticle = new ParticleA();
    newParticle->m_EmitterDelay = ParticleSystem::CharToFloatLimited(m_EmitterDelay, 5.f);
    newParticle->m_BirthDistFade = ParticleSystem::ShortToFloatLimited((*(int*)&m_BirthDistFade) >> 20, 60.f);
    newParticle->SetParticlesListSize(m_MaxParticles);

    if (m_ParticlesArray)
        m_ParticlesArray->m_Previous = newParticle;

    newParticle->m_Next = m_ParticlesArray;
    m_ParticlesArray = newParticle;

    return newParticle;
}

void ParticleSystemInfo::KillAll()
{
    Particle* currentParticlesArray = m_ParticlesArray;
    m_ParticlesArray = nullptr;

    while (currentParticlesArray)
    {
        currentParticlesArray = currentParticlesArray->m_Next;
        Particle* deletableParticle = m_ParticlesArray;

        if (currentParticlesArray == m_Particles)
            m_Particles = nullptr;

        delete deletableParticle;
    }
}

void ParticleSystemInfo::RemoveParticle(ParticleA* particleRef)
{
    if (particleRef == m_Particles)
        m_Particles = nullptr;

    if (particleRef == m_ParticlesArray)
        m_ParticlesArray = particleRef->m_Next;

    if (particleRef->m_Next)
        particleRef->m_Next->m_Previous = particleRef->m_Previous;

    if (particleRef->m_Previous)
        particleRef->m_Previous->m_Next = particleRef->m_Next;

    delete particleRef;
}

void ParticleSystemInfo::SpawnParticle(NodePosition* nodePosVtbl, const Vector4f& screenPosition, const Orientation& particleOrient)
{
    if (m_Particles && m_Particles->m_BirthDistFade == 0.f)
        m_Particles->m_BirthDistFade = (float)0.0000099999997;

    Vector4f camPos;
    m_Particles = CreateNewParticle();
    m_Particles->m_PositionMethods = nodePosVtbl;

    nodePosVtbl->GetPosition(&camPos);
    m_Particles->m_CameraPosition = camPos;
    m_Particles->m_ScreenPosition = screenPosition;
    m_Particles->m_Orientation = particleOrient;

    if (m_Flags.ExplicitBirthOnly && (m_Particles->field_60 & 0x20000) == 0)
        m_Particles->m_Amount++;
}
#include "GFXEffect.h"
#include "AssetManager.h"

int GFXEffect::TotalCreated;	//	@A3E050;

#pragma message(TODO_IMPLEMENTATION)
GFXEffect::~GFXEffect()
{
	//delete field_50;
	//delete field_54;
	//delete field_58;
	
	m_EffectProperties.ClearEffectProperties();

	TotalCreated--;
}

void GFXEffectProperties::ClearEffectProperties()
{
	g_AssetManager->DecreaseResourceReferenceCount(m_VignetteTexture);
	delete m_VignetteTexture;

	g_AssetManager->DecreaseResourceReferenceCount(m_NoiseTexture);
	delete m_NoiseTexture;
}

GFXEffect::GFXEffect() : Node(NODE_MASK_QUADTREE)
{
	MESSAGE_CLASS_CREATED(GFXEffect);

	m_EffectProperties;

	field_50 = field_54 = field_58 = nullptr;

	++TotalCreated;
}
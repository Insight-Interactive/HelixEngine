#include "RendererPCH.h"

#include "ITextureManager.h"

#include "RendererCore.h"
#include "ITexture.h"


void HManagedTexture::WaitForLoad() const
{
	while ((volatile bool&)m_IsLoading)
	{
		std::this_thread::yield();
	}
}

void HManagedTexture::Unload()
{
	if (GTextureManager)
	{
		GTextureManager->DestroyTexture(m_MapKey);
	}
}

void FTextureManager::DestroyDefaultTextures()
{
	for (uint32 i = 0; i < DT_NumDefaultTextures; ++i)
	{
		// Release the resources but dont destroy.
		GDefaultTextures[i]->Destroy();
		GDefaultTextures[i] = NULL;
	}
}


HTextureRef::HTextureRef(const HTextureRef& ref)
	: m_Ref(ref.m_Ref)
{
	if (m_Ref != nullptr)
		++m_Ref->m_ReferenceCount;
}

HTextureRef::HTextureRef(HManagedTexture* tex) : m_Ref(tex)
{
	if (m_Ref != nullptr)
		++m_Ref->m_ReferenceCount;
}

HTextureRef::~HTextureRef()
{
	if (m_Ref != nullptr && --m_Ref->m_ReferenceCount == 0)
		m_Ref->Unload();
}

void HTextureRef::operator= (std::nullptr_t)
{
	if (m_Ref != nullptr)
		--m_Ref->m_ReferenceCount;

	m_Ref = nullptr;
}

void HTextureRef::operator= (HTextureRef& rhs)
{
	if (m_Ref != nullptr)
		--m_Ref->m_ReferenceCount;

	m_Ref = rhs.m_Ref;

	if (m_Ref != nullptr)
		++m_Ref->m_ReferenceCount;
}

bool HTextureRef::IsValid() const
{
	return m_Ref && m_Ref->IsValid();
}

const HTexture* HTextureRef::Get() const
{
	return DCast<HTexture*>(m_Ref);
}

const HTexture* HTextureRef::operator->() const
{
	HE_ASSERT(m_Ref != nullptr);
	return DCast<HTexture*>(m_Ref);
}

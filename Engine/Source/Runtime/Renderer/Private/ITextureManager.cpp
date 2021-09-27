#include "RendererPCH.h"

#include "ITextureManager.h"

#include "RendererCore.h"
#include "ITexture.h"


void IManagedTexture::WaitForLoad() const
{
	while ((volatile bool&)m_IsLoading)
	{
		std::this_thread::yield();
	}
}

void IManagedTexture::Unload()
{
	if (GTextureManager)
	{
		GTextureManager->DestroyTexture(m_MapKey);
	}
}

void ITextureManager::DestroyDefaultTextures()
{
	for (uint32 i = 0; i < DT_NumDefaultTextures; ++i)
	{
		// Release the resources but dont destroy.
		GDefaultTextures[i]->Destroy();
		GDefaultTextures[i] = NULL;
	}
}


TextureRef::TextureRef(const TextureRef& ref)
	: m_Ref(ref.m_Ref)
{
	if (m_Ref != nullptr)
		++m_Ref->m_ReferenceCount;
}

TextureRef::TextureRef(IManagedTexture* tex) : m_Ref(tex)
{
	if (m_Ref != nullptr)
		++m_Ref->m_ReferenceCount;
}

TextureRef::~TextureRef()
{
	if (m_Ref != nullptr && --m_Ref->m_ReferenceCount == 0)
		m_Ref->Unload();
}

void TextureRef::operator= (std::nullptr_t)
{
	if (m_Ref != nullptr)
		--m_Ref->m_ReferenceCount;

	m_Ref = nullptr;
}

void TextureRef::operator= (TextureRef& rhs)
{
	if (m_Ref != nullptr)
		--m_Ref->m_ReferenceCount;

	m_Ref = rhs.m_Ref;

	if (m_Ref != nullptr)
		++m_Ref->m_ReferenceCount;
}

bool TextureRef::IsValid() const
{
	return m_Ref && m_Ref->IsValid();
}

const ITexture* TextureRef::Get() const
{
	return DCast<ITexture*>(m_Ref);
}

const ITexture* TextureRef::operator->() const
{
	HE_ASSERT(m_Ref != nullptr);
	return DCast<ITexture*>(m_Ref);
}

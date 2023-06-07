// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "CriticalSection.h"

template <typename AssetType>
class AssetRef;

template <typename AssetType>
class ManagedAsset
{
	friend class AssetRef<AssetType>;
	friend class MaterialManager;
	friend class FTextureManager;
	friend class FFontManager;
public:
	ManagedAsset()
		: m_MapKey( "<Unnamed ManagedAsset>" )
		, m_IsValid( false )
		, m_IsLoading( true )
		, m_ReferenceCount( 1 )
	{
	}
	virtual ~ManagedAsset()
	{
	}

	void WaitForLoad() const;
	AssetType& GetAsset();
	const AssetType& GetAsset() const;

protected:
	ManagedAsset( const String& HashName )
		: m_MapKey( HashName )
		, m_IsValid( false )
		, m_IsLoading( true )
		, m_ReferenceCount( 1 )
	{
	}

	void SetName( const String& Name );
	void SetLoadCompleted( bool Completed );

protected:
	bool IsValid() const;
	void Unload();

	String m_MapKey; // For deleting from the map later.
	bool m_IsValid;
	bool m_IsLoading;
	uint64 m_ReferenceCount;
};

template <typename AssetType>
class AssetRef
{
public:
	AssetRef( const AssetRef& Ref );
	AssetRef( ManagedAsset<AssetType>* pAsset = nullptr );
	~AssetRef();
	AssetRef& operator=( const AssetRef& Other )
	{
		this->m_Ref = Other.m_Ref;
		return *this;
	}

	void operator= ( std::nullptr_t );
	void operator= ( AssetRef& rhs );

	// Check that this points to a valid texture (which loaded successfully)
	bool IsValid() const;

	// Get the Asset.
	AssetType& GetAsset();
	const AssetType& GetAsset() const;

	AssetType* operator->();
	const AssetType* operator->() const;

private:
	ManagedAsset<AssetType>* m_Ref;

};


// Managed Asset implementation
//

template <typename AssetType>
FORCEINLINE void ManagedAsset<AssetType>::WaitForLoad() const
{
	while ((volatile bool&)m_IsLoading)
		std::this_thread::yield();
}

template <typename AssetType>
FORCEINLINE void ManagedAsset<AssetType>::Unload()
{
	//GMaterialManager.DestroyMaterial( m_MapKey );
}

template <typename AssetType>
FORCEINLINE AssetType& ManagedAsset<AssetType>::GetAsset()
{
	return SCast<AssetType&>(*this);
}

template <typename AssetType>
FORCEINLINE const AssetType& ManagedAsset<AssetType>::GetAsset() const
{
	return SCast<const AssetType&>( *this );
}

template <typename AssetType>
FORCEINLINE void ManagedAsset<AssetType>::SetName( const String& Name )
{
	m_MapKey = Name;
}

template <typename AssetType>
FORCEINLINE void ManagedAsset<AssetType>::SetLoadCompleted( bool Completed )
{
	m_IsLoading = !Completed;
	m_IsValid = Completed;
}

template <typename AssetType>
FORCEINLINE bool ManagedAsset<AssetType>::IsValid() const
{ 
	return m_IsValid; 
}


// Asset Ref implementation
//

template <typename AssetType>
FORCEINLINE AssetRef<AssetType>::AssetRef( const AssetRef& Ref )
	: m_Ref( Ref.m_Ref )
{
	if (m_Ref != nullptr)
		++m_Ref->m_ReferenceCount;
}

template <typename AssetType>
FORCEINLINE AssetRef<AssetType>::AssetRef( ManagedAsset<AssetType>* pAsset )
	: m_Ref( pAsset )
{
	if (m_Ref != nullptr)
		++m_Ref->m_ReferenceCount;
}

template <typename AssetType>
FORCEINLINE AssetRef<AssetType>::~AssetRef()
{
	if (m_Ref != nullptr && --m_Ref->m_ReferenceCount == 0)
		m_Ref->Unload();
}

template <typename AssetType>
FORCEINLINE void AssetRef<AssetType>::operator= ( std::nullptr_t )
{
	if (m_Ref != nullptr)
		--m_Ref->m_ReferenceCount;

	m_Ref = nullptr;
}

template <typename AssetType>
FORCEINLINE void AssetRef<AssetType>::operator= ( AssetRef<AssetType>& rhs )
{
	if (m_Ref != nullptr)
		--m_Ref->m_ReferenceCount;

	m_Ref = rhs.m_Ref;

	if (m_Ref != nullptr)
		++m_Ref->m_ReferenceCount;
}

template <typename AssetType>
FORCEINLINE bool AssetRef<AssetType>::IsValid() const
{
	return m_Ref && m_Ref->IsValid();
}

template <typename AssetType>
FORCEINLINE AssetType& AssetRef<AssetType>::GetAsset()
{
	return static_cast<AssetType&>(*m_Ref);
}

template <typename AssetType>
FORCEINLINE const AssetType& AssetRef<AssetType>::GetAsset() const
{
	return SCast<AssetType&>( *m_Ref );
}

template <typename AssetType>
FORCEINLINE AssetType* AssetRef<AssetType>::operator->()
{
	HE_ASSERT( m_Ref != nullptr );
	return (AssetType*)m_Ref;
}

template <typename AssetType>
FORCEINLINE const AssetType* AssetRef<AssetType>::operator->() const
{
	HE_ASSERT( m_Ref != nullptr );
	return (const AssetType*)m_Ref;
}


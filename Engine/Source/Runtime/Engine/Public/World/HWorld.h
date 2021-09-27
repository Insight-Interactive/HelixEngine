// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/HObject.h"

#include "World/CameraManager.h"
#include "AssetRegistry/SerializeableInterface.h"


class HLevel;
class HCameraComponent;
class APlayerCharacter;
class ICommandContext;
class ViewportContext;

/*
	The "World" is a high-level construct that contains all entities in the playable space.
	The world contains a level which contains all actors. The name level is used interchangably 
	for ease of use to the user.
*/
class HWorld : public HObject, public SerializeableInterface
{
	friend class WorldOutlinePanel;
	friend class HEditorEngine;
	using Super = HObject;
public:
	HWorld();
	~HWorld();

	void Initialize( const Char* LevelURL );
	void Flush();
	float GetDeltaTime() const;

	void BeginPlay();
	void Tick(float DeltaTime);
	void Render(ICommandContext& CmdContext);

	CameraManager* GetCameraManager();
	ViewportContext* GetOwningViewport();
	HCameraComponent* GetCurrentSceneRenderCamera();
	void SetCurrentSceneRenderCamera(HCameraComponent* pCamera);
	void SetViewport( ViewportContext* pViewport );

	HLevel* GetCurrentLevel();
	void AddPlayerCharacterRef( APlayerCharacter* pCharacter );
	APlayerCharacter* GetPlayerCharacter( uint32 Index );

protected:
	virtual void Serialize( const Char* Filename ) override;
	virtual void Serialize( WriteContext& Value ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

protected:
	HLevel* m_pLevel;
	APlayerCharacter* m_pPlayerCharacter;
	std::vector<APlayerCharacter*> m_PlayerCharacterRefs;
	String m_Filepath;

	HCameraComponent* m_RenderingCamera;
	ViewportContext* m_pViewport;

	CameraManager m_CameraManager;
};


//
// Inline function implementations
//

inline CameraManager* HWorld::GetCameraManager()
{
	return &m_CameraManager;
}

inline HCameraComponent* HWorld::GetCurrentSceneRenderCamera()
{
	return m_RenderingCamera;
}

inline void HWorld::SetCurrentSceneRenderCamera(HCameraComponent* pCamera)
{
	m_RenderingCamera = pCamera;
}

inline ViewportContext* HWorld::GetOwningViewport()
{
	return m_pViewport;
}

inline void HWorld::SetViewport( ViewportContext* pViewport )
{
	m_pViewport = pViewport;
}

inline HLevel* HWorld::GetCurrentLevel()
{
	return m_pLevel;
}

inline void HWorld::AddPlayerCharacterRef( APlayerCharacter* pCharacter )
{
	m_PlayerCharacterRefs.push_back( pCharacter );
}

inline APlayerCharacter* HWorld::GetPlayerCharacter( uint32 Index )
{
	HE_ASSERT( Index >= 0 && Index < m_PlayerCharacterRefs.size() );
	return m_PlayerCharacterRefs[Index];
}

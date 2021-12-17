// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/HObject.h"

#include "World/CameraManager.h"
#include "AssetRegistry/SerializeableInterface.h"
#include "World/Scene.h"


class HLevel;
class HCameraComponent;
class APlayerCharacter;
class FCommandContext;
class FViewportContext;
struct FDebugLineRenderInfo;

/*
	The "World" is a high-level construct that contains all entities in the playable space.
	The world contains a level which contains all actors. The name level is used interchangably 
	for ease of use to the user.
*/
class HWorld : public HObject, public FSerializeableInterface
{
	friend class WorldOutlinePanel;
	friend class HEditorEngine;
	using Super = HObject;
public:
	HWorld();
	virtual ~HWorld();

	void Initialize( const Char* LevelURL );
	void Flush();
	float GetDeltaTime() const;

	void BeginPlay();
	void Tick(float DeltaTime);
	void Render(FCommandContext& CmdContext);

	HCameraManager* GetCameraManager();
	FViewportContext* GetOwningViewport();
	HCameraComponent* GetCurrentSceneRenderCamera();
	void SetCurrentSceneRenderCamera(HCameraComponent* pCamera);
	void SetViewport( FViewportContext* pViewport );

	HScene* GetScene();
	HLevel* GetCurrentLevel();
	void AddPlayerCharacterRef( APlayerCharacter* pCharacter );
	APlayerCharacter* GetPlayerCharacter( uint32 Index );

	void DrawDebugLine( const FDebugLineRenderInfo& LineInfo );

protected:
	/*
		Reload the world.
	*/
	void Reload();

	/*
		Reload the world and call begin play on all actors.
	*/
	void ReloadAndBeginPlay();

	virtual void Serialize( const Char* Filename ) override;
	virtual void Serialize( WriteContext& Value ) override;
	virtual void Deserialize( const ReadContext& Value ) override;

protected:
	HLevel* m_pLevel;
	HScene m_Scene;
	APlayerCharacter* m_pPlayerCharacter;
	std::vector<APlayerCharacter*> m_PlayerCharacterRefs;
	String m_Filepath;

	HCameraComponent* m_RenderingCamera;
	FViewportContext* m_pRenderingViewport;

	HCameraManager m_CameraManager;
};


//
// Inline function implementations
//

FORCEINLINE HScene* HWorld::GetScene()
{
	return &m_Scene;
}

FORCEINLINE HCameraManager* HWorld::GetCameraManager()
{
	return &m_CameraManager;
}

FORCEINLINE HCameraComponent* HWorld::GetCurrentSceneRenderCamera()
{
	return m_RenderingCamera;
}

FORCEINLINE void HWorld::SetCurrentSceneRenderCamera(HCameraComponent* pCamera)
{
	m_RenderingCamera = pCamera;
}

FORCEINLINE FViewportContext* HWorld::GetOwningViewport()
{
	return m_pRenderingViewport;
}

FORCEINLINE HLevel* HWorld::GetCurrentLevel()
{
	return m_pLevel;
}

FORCEINLINE void HWorld::AddPlayerCharacterRef( APlayerCharacter* pCharacter )
{
	m_PlayerCharacterRefs.push_back( pCharacter );
}

FORCEINLINE APlayerCharacter* HWorld::GetPlayerCharacter( uint32 Index )
{
	HE_ASSERT( Index >= 0 && Index < m_PlayerCharacterRefs.size() );
	return m_PlayerCharacterRefs[Index];
}

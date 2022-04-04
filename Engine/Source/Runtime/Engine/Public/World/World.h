// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/HObject.h"

#include "World/CameraManager.h"
#include "AssetRegistry/SerializeableInterface.h"
#include "World/Scene.h"
#include "World/Level.h"
#include "PhysicsScene.h"


class HLevel;
class FCommandContext;
class HCameraComponent;
class APlayerCharacter;
class FViewportContext;
class HColliderComponent;
class HPlaneColliderComponent;
class HCubeColliderComponent;
class HSphereColliderComponent;
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
	void Initialize();
	void Flush();
	float GetDeltaTime() const;

	void BeginPlay();
	void Tick( float DeltaTime );
	void Render( FCommandContext& CmdContext );

	HCameraManager* GetCameraManager();
	FViewportContext* GetOwningViewport();
	HCameraComponent* GetCurrentSceneRenderCamera();
	void SetCurrentSceneRenderCamera( HCameraComponent* pCamera );
	void SetViewport( FViewportContext* pViewport );

	void AddSphereColliderComponent( HSphereColliderComponent* pSphere, bool StartDisabled = false );
	void AddPlaneColliderComponent( HPlaneColliderComponent* pPlane, bool StartDisabled = false );
	void AddCubeColliderComponent( HCubeColliderComponent* pCube, bool StartDisabled = false );
	void RemoveColliderComponent( HColliderComponent* pSphere );
	HScene& GetScene();
	PhysicsScene& GetPhysicsScene();
	HLevel& GetCurrentLevel();
	void AddPlayerCharacterRef( APlayerCharacter* pCharacter );
	APlayerCharacter* GetPlayerCharacter( uint32 Index = 0 );

	AActor* CreateEmptyActorInstance( const HName& Name );
	template <typename ActorType>
	ActorType* CreateDynamicActorinstance( const HName& Name );

	void DrawDebugLine( const FDebugLineRenderInfo& LineInfo );

	void PausePhysics();
	void UnPausePhysics();

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

	void RegisterScenes();


protected:
	HLevel m_Level;
	HScene m_Scene;
	PhysicsScene m_PhysicsScene;

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

FORCEINLINE PhysicsScene& HWorld::GetPhysicsScene()
{
	return m_PhysicsScene;
}

FORCEINLINE HScene& HWorld::GetScene()
{
	return m_Scene;
}

FORCEINLINE HCameraManager* HWorld::GetCameraManager()
{
	return &m_CameraManager;
}

FORCEINLINE HCameraComponent* HWorld::GetCurrentSceneRenderCamera()
{
	return m_RenderingCamera;
}

FORCEINLINE void HWorld::SetCurrentSceneRenderCamera( HCameraComponent* pCamera )
{
	m_RenderingCamera = pCamera;
}

FORCEINLINE FViewportContext* HWorld::GetOwningViewport()
{
	return m_pRenderingViewport;
}

FORCEINLINE HLevel& HWorld::GetCurrentLevel()
{
	return m_Level;
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

FORCEINLINE AActor* HWorld::CreateEmptyActorInstance( const HName& Name )
{
	return m_Level.CreateActor<AActor>( Name );
}

template <typename ActorType>
FORCEINLINE ActorType* HWorld::CreateDynamicActorinstance( const HName& Name )
{
	return m_Level.CreateActor<ActorType>( Name );
}

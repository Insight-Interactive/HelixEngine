// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/HObject.h"

#include "World/CameraManager.h"
#include "AssetRegistry/SerializeableInterface.h"
#include "World/Scene.h"
#include "World/Level.h"
#include "PhysicsScene.h"
#include "UI/Panel.h"
#include "UI/Label.h"

class HLevel;
class FCommandContext;
class HCameraComponent;
class ACharacter;
class FViewportContext;
class HColliderComponent;
class HPlaneColliderComponent;
class HCubeColliderComponent;
class HSphereColliderComponent;
class HCapsuleColliderComponent;
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
	friend class FViewportContext;
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

	void AddPanel( FUIPanel* pPanel );
	void RemovePanel( FUIPanel* pPanel );
	void AddSphereColliderComponent( HSphereColliderComponent* pSphere, bool IsStatic = false, bool IsTrigger = false );
	void AddPlaneColliderComponent( HPlaneColliderComponent* pPlane, bool IsStatic = false, bool IsTrigger = false );
	void AddCubeColliderComponent( HCubeColliderComponent* pCube, bool IsStatic = false, bool IsTrigger = false );
	void AddCapsuleColliderComponent( HCapsuleColliderComponent* pCapsule, bool IsStatic = false, bool IsTrigger = false );
	void RemoveColliderComponent( HColliderComponent* pSphere );
	HScene& GetScene();
	FUIPanel& GetDebugUIPanel();
	HPhysicsScene& GetPhysicsScene();
	bool IsLevelLoaded() const;
	HLevel& GetCurrentLevel();
	void AddPlayerCharacterRef( ACharacter* pCharacter );
	ACharacter* GetPlayerCharacter( uint32 Index = 0 );

	float GetMouseMoveDeltaX();
	float GetMouseMoveDeltaY();

	AActor* CreateEmptyActorInstance( const HName& Name );
	template <typename ActorType>
	ActorType* CreateDynamicActorinstance( const HName& Name );

	void DrawDebugLine( const FDebugLineRenderInfo& LineInfo );

	void PausePhysics();
	void UnPausePhysics();

	bool Raycast( const FVector3& Origin, const FVector3& UnitDirection, float Distance, FRaycastHitInfo* HitResults = nullptr, std::vector<HColliderComponent*>* IgnoreActors = nullptr );

protected:

	bool AdvancePhysics( float DeltaTime );
	
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

	std::vector<FUIPanel*>& GetUIPanels();


protected:
	std::vector<FUIPanel*> m_UIPanels;
	// UI
	FUIPanel m_DebugUI;
	FLabel m_FPSCounter;

	HLevel m_Level;
	HScene m_Scene;
	HPhysicsScene m_PhysicsScene;

	ACharacter* m_pPlayerCharacter;
	std::vector<ACharacter*> m_PlayerCharacterRefs;
	String m_Filepath;

	HCameraComponent* m_RenderingCamera;
	FViewportContext* m_pRenderingViewport;

	HCameraManager m_CameraManager;
};


//
// Inline function implementations
//

FORCEINLINE HPhysicsScene& HWorld::GetPhysicsScene()
{
	return m_PhysicsScene;
}

FORCEINLINE HScene& HWorld::GetScene()
{
	return m_Scene;
}

FORCEINLINE FUIPanel& HWorld::GetDebugUIPanel()
{
	return m_DebugUI;
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

FORCEINLINE bool HWorld::IsLevelLoaded() const
{
	return m_Level.IsLoading();
}

FORCEINLINE HLevel& HWorld::GetCurrentLevel()
{
	return m_Level;
}

FORCEINLINE void HWorld::AddPanel( FUIPanel* pPanel )
{
	auto iter = std::find( m_UIPanels.begin(), m_UIPanels.end(), pPanel );
	if (iter == m_UIPanels.end())
	{
		m_UIPanels.push_back(pPanel);
		return;
	}
	else
	{
		HE_LOG( Warning, TEXT( "Trying to add a UI panel to the world but it was already added!" ) );
	}
}

FORCEINLINE void HWorld::RemovePanel( FUIPanel* pPanel )
{
	auto iter = std::find( m_UIPanels.begin(), m_UIPanels.end(), pPanel );
	if (iter != m_UIPanels.end())
	{
		m_UIPanels.erase( iter );
		return;
	}
	else
	{
		HE_LOG( Warning, TEXT( "Trying to remove UI panel when it does not exist in the world! Was it added?" ) );
	}
}

FORCEINLINE void HWorld::AddPlayerCharacterRef( ACharacter* pCharacter )
{
	m_PlayerCharacterRefs.push_back( pCharacter );
}

FORCEINLINE ACharacter* HWorld::GetPlayerCharacter( uint32 Index )
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

FORCEINLINE std::vector<FUIPanel*>& HWorld::GetUIPanels()
{
	return m_UIPanels;
}

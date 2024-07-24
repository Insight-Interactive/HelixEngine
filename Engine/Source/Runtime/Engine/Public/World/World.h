// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

#include "GameFramework/HObject.h"

#include "World/CameraManager.h"
#include "World/Scene.h"
#include "World/Level.h"
#include "PhysicsScene.h"
#include "UI/Panel.h"
#include "UI/Label.h"
#include "Input/InputEnums.h"
#include "Path.h"


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
class HWorld : public HObject
{
	friend class WorldOutlinePanel;
	friend class HEditorEngine;
	friend class FViewportContext;
	friend class HControllerComponent; // To bind input dispatchers
	friend class HGameInstance; // To bind menu callbacks
	using Super = HObject;
public:
	HWorld();
	virtual ~HWorld();

	void Initialize( const FPath& LevelURL );
	void Initialize();
	void Save();
	void Flush();

	void BeginPlay();
	void Tick( float DeltaTime );
	void Render( FCommandContext& CmdContext );

	HCameraManager* GetCameraManager();
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
	ACharacter* GetPlayerCharacter();

	FVector2 GetMouseScreenPos();
	float GetMouseMoveDeltaX();
	float GetMouseMoveDeltaY();
	bool IsPressed( DigitalInput Key );
	bool IsFirstPressed( DigitalInput Key );
	bool IsReleased( DigitalInput Key );

	float GetWindowWidth();
	float GetWindowHeight();

	AActor* CreateEmptyActorInstance( const char* Name );
	template <typename ActorType>
	ActorType* CreateDynamicActorinstance( const char* Name );

	void DrawDebugLine( const FDebugLineRenderInfo& LineInfo );

	void PausePhysics();
	void UnPausePhysics();

	bool Raycast( const FVector3& Origin, const FVector3& UnitDirection, float Distance, FRaycastHitInfo* HitResults = nullptr, std::vector<HColliderComponent*>* IgnoreActors = nullptr );

protected:
	FViewportContext* GetOwningViewport();

	/*
		Reload the world.
	*/
	void Reload();

	/*
		Reload the world and call begin play on all actors.
	*/
	void ReloadAndBeginPlay();

	virtual void Serialize( const Char* Filename ) override;
	virtual void Deserialize( const JsonUtility::ReadContext& Value ) override;

	void RegisterScenes();

	std::vector<FUIPanel*>& GetUIPanels();

	PointLightDataHandle m_TestPointLight;

protected:
	std::vector<FUIPanel*> m_UIPanels;
	// UI
	FUIPanel m_DebugUI;
	FLabel m_FPSCounter;

	HLevel m_Level;
	HScene m_Scene;
	HPhysicsScene m_PhysicsScene;

	ACharacter* m_pPlayerCharacter;
	FPath m_LevelFilepath;

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
		HE_LOG( Warning, TEXT( "Trying to add a UI panel to the world but it was already added! Doing nothing for now." ) );
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

FORCEINLINE ACharacter* HWorld::GetPlayerCharacter()
{
	return m_pPlayerCharacter;
}

FORCEINLINE AActor* HWorld::CreateEmptyActorInstance( const char* Name )
{
	return m_Level.CreateActor<AActor>( Name );
}

template <typename ActorType>
FORCEINLINE ActorType* HWorld::CreateDynamicActorinstance( const char* Name )
{
	return m_Level.CreateActor<ActorType>( Name );
}

FORCEINLINE std::vector<FUIPanel*>& HWorld::GetUIPanels()
{
	return m_UIPanels;
}

#include "EnginePCH.h"

#include "GameFramework/Components/HColliderComponent.h"

#include "World/World.h"
#include "PhysicsScene.h"
#include "CommandContext.h"
#include "Renderer/ConstantBufferStructures.h"
#include "Renderer/ShaderRegisters.h"
#include "GameFramework/Components/HCameraComponent.h"


HColliderComponent::HColliderComponent( FComponentInitArgs& InitArgs )
	: HSceneComponent( InitArgs )
	, m_IsTrigger( false )
	, m_CollisionBoundsDrawEnabled( true )
	, m_Material( nullptr )
	, m_IsStatic ( false )
	, m_SimulationEnabled( true )
{
	m_MeshWorldCB.Create( L"[Collider Component] World CB" );
}

HColliderComponent::~HColliderComponent()
{
	m_MeshWorldCB.Destroy();
	delete m_Material;
}

void HColliderComponent::BeginPlay()
{
	Super::BeginPlay();

	if (m_SimulationEnabled)
		GetRigidBody().EnableSimulation(); // Simulation is disabled by default on level load to prevent simulation while using the editor.
}

void HColliderComponent::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	HRigidBody& rb = GetRigidBody();
	
	if (rb.IsSleeping())
		return;
	
	// Fetch the results of the simulation and apply them to the game.
	FTransform SimTransform = rb.GetSimulationWorldTransform();
	Super::SetPosition( SimTransform.GetPosition() );
	Super::SetRotation( SimTransform.GetRotation() );
}

void HColliderComponent::SetPosition( const FVector3& NewPos )
{
	Super::SetPosition( NewPos );
	GetRigidBody().SetSimulatedPosition( NewPos );
}

void HColliderComponent::SetRotation( const FQuat& NewRotation )
{
	Super::SetRotation( NewRotation );
	GetRigidBody().SetSimulatedRotation( NewRotation );
}

void HColliderComponent::SetScale( const FVector3& NewScale )
{
	Super::SetScale( NewScale );
	// TODO GetRigidBody().SetColliderScale(); 
}

void HColliderComponent::SetPosition( const float& X, const float& Y, const float& Z )
{
	Super::SetPosition( X, Y, Z );
	GetRigidBody().SetSimulatedPosition( { X, Y, Z } );
}

void HColliderComponent::SetRotation( const float& Pitch, const float& Yaw, const float& Roll )
{
	Super::SetRotation( Pitch, Yaw, Roll );
	FQuat DeltaPitch = FQuat::CreateFromAxisAngle( FVector3::Right, Pitch );
	FQuat DeltaYaw = FQuat::CreateFromAxisAngle( FVector3::Up, Yaw );
	FQuat DeltaRoll = FQuat::CreateFromAxisAngle( FVector3::Forward, Roll );
	//FQuat Rotation = FQuat::CreateFromYawPitchRoll( Yaw, Pitch, Roll );
	GetRigidBody().SetSimulatedRotation( DeltaPitch * DeltaYaw * DeltaRoll );
}

void HColliderComponent::SetScale( const float& X, const float& Y, const float& Z )
{
	Super::SetScale( X, Y, Z );
}

void HColliderComponent::Translate( const float& X, const float& Y, const float& Z )
{
	Super::Translate( X, Y, Z );
	GetRigidBody().SetSimulatedPosition( { X, Y, Z } );
}

void HColliderComponent::Scale( const float& X, const float& Y, const float& Z )
{
	Super::Scale( X, Y, Z );
}

void HColliderComponent::Render( FCommandContext& GfxContext )
{
	Super::Render(GfxContext);

#if HE_DEBUG
	if (!m_CollisionBoundsDrawEnabled)
		return;

	/*HCameraComponent* pCamera = GetWorld()->GetCurrentSceneRenderCamera();
	if (pCamera)
	{
		float DistSquared = FVector3::DistanceSquared( pCamera->GetWorldPosition(), GetWorldPosition() );
		if (DistSquared > (300.f * 300.f))
			return;
	}*/

	if (m_Material)
	{
		FColor Color = FColor::RedOpaque;
		if (GetIsTrigger())
			Color = FColor::WhiteOpaque;

		m_Material->SetVector3( "kColor", Color.ToVector3() );
		
		// Set the material information.
		m_Material->Bind( GfxContext );
	}

	if (m_MeshAsset.get() && m_MeshAsset->IsValid())
	{
		// Set the world buffer.
		MeshWorldCBData* pWorld = m_MeshWorldCB.GetBufferPointer();
		pWorld->kWorldMat = GetLocalMatrix().Transpose();

		m_MeshWorldCB.SetDirty( true );
		GfxContext.SetGraphicsConstantBuffer( kMeshWorld, m_MeshWorldCB );

		// TODO Request draw from model in model manager to render meshes of the same type in batches.
		// Render the geometry.
		GfxContext.SetPrimitiveTopologyType( PT_TiangleList );
		GfxContext.BindVertexBuffer( 0, m_MeshAsset->GetVertexBuffer() );
		GfxContext.BindIndexBuffer( m_MeshAsset->GetIndexBuffer() );
		GfxContext.DrawIndexedInstanced( m_MeshAsset->GetNumIndices(), 1, 0, 0, 0 );
	}
#endif
}

void HColliderComponent::Serialize( JsonUtility::WriteContext& Output )
{
	Output.Key( HE_STRINGIFY( HColliderComponent ) );
	Output.StartArray();
	{
		// Outer properties.
		Output.StartObject();
		{
			Super::Serialize( Output );
		}
		Output.EndObject();

		// Static mesh properties.
		Output.StartObject();
		{
			Output.Key( "IsTrigger" );
			Output.Bool( m_IsTrigger );

			Output.Key( "Density" );
			Output.Double( GetRigidBody().GetDensity() );

			Output.Key( "IsStatic" );
			Output.Bool( GetRigidBody().GetIsStatic() );

			Output.Key( "CollisionBoundsDrawEnabled" );
			Output.Bool( m_CollisionBoundsDrawEnabled );

			Output.Key( "EnableSimulation" );
			Output.Bool( GetRigidBody().IsSimulationEnabled() );

			// TODO: Save material refernece
		}
		Output.EndObject();
	}
	Output.EndArray();
}

void HColliderComponent::Deserialize( const JsonUtility::ReadContext& Value )
{
	Super::Deserialize( Value[0][HE_STRINGIFY( HSceneComponent )] );

	const JsonUtility::ReadContext& This = Value[1];
	JsonUtility::GetBoolean( This, "IsTrigger", m_IsTrigger );
	
	float Density = 0.f;
	JsonUtility::GetFloat( This, "Density", Density );
	GetRigidBody().SetDensity( Density );

	JsonUtility::GetBoolean( This, "IsStatic", m_IsStatic );

	JsonUtility::GetBoolean( This, "CollisionBoundsDrawEnabled", m_CollisionBoundsDrawEnabled );

	JsonUtility::GetBoolean( This, "EnableSimulation", m_SimulationEnabled );
}

void HColliderComponent::OnOwnerDeserializeComplete()
{
	GetRigidBody().SetGlobalPositionOrientation( GetWorldPosition(), GetRotation() );
}

void HColliderComponent::OnCreate()
{
	Super::OnCreate(); 

	m_Material = new FMaterialInstance();
	m_Material->CreateFromParent( "141d7fa2-8208-4ba2-ba33-3fa72163c4d8" );

	GetWorld()->GetScene().AddDebugCollider( this );
}

void HColliderComponent::OnDestroy()
{
	Super::OnDestroy();

	HWorld& World = *GetWorld();
	World.GetScene().RemoveDebugCollider( this );
	World.RemoveColliderComponent( this );
}

// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "EnginePCH.h"

#include "World/Scene.h"

#include "GameFramework/Components/HCameraComponent.h"
#include "World/World.h"
#include "Graphics/ShaderRegisters.h"
#include "GameFramework/Actor/ACharacter.h"


HScene::HScene( HWorld* pOwner )
	: Super( "Scene" )
	, m_pOwner( pOwner )
	, m_DrawCollision( true )
{

}

HScene::~HScene()
{

}

void HScene::RenderWorldGeo( FCommandContext& CmdContext )
{
	CmdContext.BeginDebugMarker( L"Render World Geo" );
	{
		for (uint32 i = 0; i < m_WorldGeo.size(); i++)
		{
			FWorldMesh& Mesh = *m_WorldGeo[i];

			Mesh.m_Material->Bind( CmdContext );

			CmdContext.SetGraphicsConstantBuffer( kMeshWorld, Mesh.m_MeshWorldCB );
			CmdContext.SetPrimitiveTopologyType( PT_TiangleList );
			CmdContext.BindVertexBuffer( 0, Mesh.m_Mesh->GetVertexBuffer() );
			CmdContext.BindIndexBuffer( Mesh.m_Mesh->GetIndexBuffer() );
			CmdContext.DrawIndexedInstanced( Mesh.m_Mesh->GetNumIndices(), 1, 0, 0, 0 );
		}
	}
	CmdContext.EndDebugMarker();
}

void HScene::RenderStaticLitOpaqueObjects(FCommandContext& CmdContext)
{
	CmdContext.BeginDebugMarker(L"Render Opaque Lit");
	{
		for (auto Iter = m_StaticMeshs.begin(); Iter != m_FirstTranslucentOrUnlit; ++Iter)
		{
			HStaticMeshComponent& StaticMesh = (**Iter);
			if (StaticMesh.GetMaterial()->GetShadingModel() == SM_DefaultLit)
				StaticMesh.Render(CmdContext);
		}
	}
	CmdContext.EndDebugMarker();
}

void HScene::RenderDebugMeshes( FCommandContext& CmdContext )
{
	CmdContext.BeginDebugMarker( L"Render Debug Meshes" );
	{
		if (m_DrawCollision)
		{

			CmdContext.BeginDebugMarker( L"World Collision" );
			{
				for (uint32 i = 0; i < m_WorldGeo.size(); i++)
				{
					FWorldMesh& Mesh = *m_WorldGeo[i];
					if (Mesh.m_Collision.IsValid())
					{
						/*ACharacter* Player = GetWorld()->GetPlayerCharacter();
						if (Player)
						{
							FVector3 Scale, Position;
							FQuat Rotation;
							Mesh.m_MeshWorldCB->kWorldMat.Decompose( Scale, Rotation, Position );
							float DistSquared = FVector3::DistanceSquared( Player->GetRootComponent()->GetWorldPosition(), Position);
							if (DistSquared > (150.f * 150.f))
								continue;
						}*/

						if (Mesh.m_CollisionWireframeMaterial)
						{
							FColor Color = FColor::BlackOpaque;
							Mesh.m_CollisionWireframeMaterial->SetVector3( "kColor", Color.ToVector3() );

							// Set the material information.
							Mesh.m_CollisionWireframeMaterial->Bind( CmdContext );

							CmdContext.SetGraphicsConstantBuffer( kMeshWorld, Mesh.m_MeshWorldCB );
							CmdContext.SetPrimitiveTopologyType( PT_TiangleList );
							CmdContext.BindVertexBuffer( 0, Mesh.m_Mesh->GetVertexBuffer() );
							CmdContext.BindIndexBuffer( Mesh.m_Mesh->GetIndexBuffer() );
							CmdContext.DrawIndexedInstanced( Mesh.m_Mesh->GetNumIndices(), 1, 0, 0, 0 );
						}
					}
				}
			}
			CmdContext.EndDebugMarker();

			CmdContext.BeginDebugMarker( L"Collider components" );
			{
				for (auto Iter = m_DebugColliderMeshs.begin(); Iter != m_DebugColliderMeshs.end(); ++Iter)
				{
					HColliderComponent& Collider = (**Iter);
					Collider.Render( CmdContext );
				}
			}
			CmdContext.EndDebugMarker();
		}
	}
	CmdContext.EndDebugMarker();
}

void HScene::RenderStaticTranslucentAndUnlitObjects(FCommandContext& CmdContext)
{
	CmdContext.BeginDebugMarker(L"Render Translucent Lit");
	{
		for (auto Iter = m_FirstTranslucentOrUnlit; Iter != m_StaticMeshs.end(); ++Iter)
		{
			HStaticMeshComponent& StaticMesh = (**Iter);

			EShadingModel ShadingMod = StaticMesh.GetMaterial()->GetShadingModel();
			StaticMesh.Render(CmdContext);
		}
	}
	CmdContext.EndDebugMarker();

	CmdContext.BeginDebugMarker(L"Render Lights");
	{
		for (size_t i = 0; i < m_PointLights.size(); ++i)
		{
			m_PointLights[i]->Render(CmdContext);
		}
	}
	CmdContext.EndDebugMarker();
}
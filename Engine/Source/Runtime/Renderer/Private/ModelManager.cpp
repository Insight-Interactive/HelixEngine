#include <RendererPCH.h>

#include "ModelManager.h"

#include "miniz.c"
#include "ofbx.h"
#include "ofbx.cpp"


#include "FileSystem.h"
#include "StringHelper.h"
#include "VertexLayouts.h"


extern StaticGeometryManager GStaticGeometryManager;


void ManagedStaticMeshGeometry::WaitForLoad() const
{
	while ((volatile bool&)m_IsLoading)
		std::this_thread::yield();
}

void ManagedStaticMeshGeometry::Unload()
{
	GStaticGeometryManager.DestroyMesh(m_MapKey);
}

void StaticGeometryManager::DestroyMesh(const String& Key)
{
	auto Iter = m_ModelCache.find(Key);
	if (Iter != m_ModelCache.end())
		m_ModelCache.erase(Iter);

}

bool StaticGeometryManager::MeshExists(const String& Name)
{
	auto Iter = m_ModelCache.find(Name);
	return Iter != m_ModelCache.end();
}

void StaticGeometryManager::FlushCache()
{
	m_ModelCache.clear();
}

void StaticGeometryManager::LoadFBXFromFile(const char* FilePath)
{
	ManagedStaticMeshGeometry* pMesh = NULL;

	// Read the data file from disk.
	//
	DataBlob Memory = FileSystem::ReadRawData(FilePath);
	HE_ASSERT(Memory.IsValid()); // Failed to load model.

	const ofbx::u8* pData = (const ofbx::u8*)Memory.GetBufferPointer();
	std::vector<Vertex3D> Verticies;
	std::vector<uint32> Indices;

	// TODO: Only FBX file format is supported currently. Add more file formats.
	// Process the mesh as an fbx file.
	//
	ofbx::IScene* pScene = ofbx::load(
		pData,
		(int)Memory.GetDataSize(),
		(ofbx::u64)ofbx::LoadFlags::TRIANGULATE
	);
	if (pScene == NULL)
	{
		R_LOG(Error, TEXT("[Static Geometry Manager][ofbx] An error occured when loading a model: %s"), ofbx::getError());
		return;
	}

	// Parse the mesh.
	//
	const ofbx::GlobalSettings* s = pScene->getGlobalSettings();
	for (int i = 0; i < pScene->getMeshCount(); ++i)
	{
		const ofbx::Mesh& Mesh = *pScene->getMesh(i);
		std::vector<Vertex3D> Verticies; Verticies.reserve(Mesh.getGeometry()->getVertexCount());
		std::vector<uint32> Indices; Indices.reserve(Mesh.getGeometry()->getIndexCount());
		
		String Name = StringHelper::GetFilenameFromDirectory(Mesh.name);
		uint64 HashName = std::hash<String>{}(Mesh.name);
		{
			ScopedCriticalSection Gaurd(m_MapMutex);

			auto Iter = m_ModelCache.find(Name);
			if (Iter != m_ModelCache.end())
			{
				pMesh = Iter->second.get();
				pMesh->WaitForLoad();
				continue;
			}
			else
			{
				ParseFBXMesh(Mesh, Verticies, Indices);

				pMesh = new ManagedStaticMeshGeometry(Name);
				pMesh->SetHashName(HashName);
				pMesh->Create(Verticies.data(), (uint32)Verticies.size() * sizeof(Vertex3D), (uint32)Verticies.size(), sizeof(Vertex3D), Indices.data(), (uint32)Indices.size() * sizeof(uint32), (uint32)Indices.size());
				pMesh->SetLoadCompleted(true);

				m_ModelCache[Name].reset(pMesh);
			}
		}
	}
}

StaticMeshGeometryRef StaticGeometryManager::RegisterGeometry(const std::string& Name, void* Verticies, uint32 VertexDataSizeInBytes, uint32 NumVerticies, uint32 VertexSize, void* Indices, uint32 IndexDataSizeInBytes, uint32 NumIndices)
{
#if HE_WITH_EDITOR
	HE_ASSERT(MeshExists(Name) == false); // Trying to register a mesh that already exist or has the same name as a mesh that is already registered.
#endif

	uint64 HashName = std::hash<std::string>{}(Name);
	ManagedStaticMeshGeometry* pMesh = new ManagedStaticMeshGeometry(Name);
	pMesh->SetHashName(HashName);
	pMesh->Create(Verticies, VertexDataSizeInBytes, NumVerticies, VertexSize, Indices, IndexDataSizeInBytes, NumIndices);
	pMesh->SetLoadCompleted(true);

	m_ModelCache[Name].reset(pMesh);

	return pMesh;
}

void StaticGeometryManager::ParseFBXMesh(const ofbx::Mesh& Mesh, std::vector<Vertex3D>& OutVerticies, std::vector<uint32>& OutIndices)
{
	const ofbx::Geometry& Geometry = *(Mesh.getGeometry());

	const ofbx::Vec3* RawVerticies = Geometry.getVertices();
	int VertexCount = Geometry.getVertexCount();

	for (int i = 0; i < VertexCount; ++i)
	{
		Vertex3D Vertex;

		Vertex.Position.x = (float)RawVerticies[i].x;
		Vertex.Position.y = (float)RawVerticies[i].y;
		Vertex.Position.z = (float)RawVerticies[i].z;

		if (Geometry.getNormals() != nullptr)
		{
			const ofbx::Vec3* normals = Geometry.getNormals();

			Vertex.Normal.x = (float)normals[i].x;
			Vertex.Normal.y = (float)normals[i].y;
			Vertex.Normal.z = (float)normals[i].z;
		}

		if (Geometry.getUVs() != nullptr)
		{
			const ofbx::Vec2* uvs = Geometry.getUVs();
			int count = Geometry.getIndexCount();

			Vertex.UV0.x = (float)uvs[i].x;
			Vertex.UV0.y = (float)uvs[i].y;

			const ofbx::Vec3* tans = Geometry.getTangents();
			if (tans)
			{
				Vertex.Tangent.x = (float)tans[i].x;
				Vertex.Tangent.y = (float)tans[i].y;
				Vertex.Tangent.z = (float)tans[i].z;

				FVector3 TempTangent((float)tans[i].x, (float)tans[i].y, (float)tans[i].z);
				FVector3 Normal(Vertex.Normal.x, Vertex.Normal.y, Vertex.Normal.z);
				FVector3 BiTangent = Normal.Cross(TempTangent);

				Vertex.BiTangent.x = BiTangent.x;
				Vertex.BiTangent.y = BiTangent.y;
				Vertex.BiTangent.z = BiTangent.z;
			}
			else
			{
				Vertex.UV0 = FVector2(0.0f, 0.0f);
				Vertex.Tangent = FVector3(0.0f, 0.0f, 0.0f);
				Vertex.BiTangent = FVector3(0.0f, 0.0f, 0.0f);
			}

			if (Geometry.getColors() != NULL)
			{
				const ofbx::Vec4* Colors = Geometry.getColors();
				Vertex.Color.x = (float)Colors[0].x;
				Vertex.Color.y = (float)Colors[0].y;
				Vertex.Color.z = (float)Colors[0].z;
				Vertex.Color.w = (float)Colors[0].w;
			}
		}

		OutVerticies.push_back(Vertex);
	}

	const int* RawFaceIndicies = Geometry.getFaceIndices();
	int IndexCount = Geometry.getIndexCount();
	for (int i = 0; i < IndexCount; ++i)
	{
		int idx = RawFaceIndicies[i];

		// If the index is negative in fbx that means it is the last index of that polygon.
		// So, make it positive and subtrct one.
		if (idx < 0)
			idx = -(idx + 1);

		OutIndices.push_back(idx);
	}

}

StaticMeshGeometryRef::StaticMeshGeometryRef(const StaticMeshGeometryRef& ref)
	: m_Ref(ref.m_Ref)
{
	if (m_Ref != nullptr)
		++m_Ref->m_ReferenceCount;
}

StaticMeshGeometryRef::StaticMeshGeometryRef(ManagedStaticMeshGeometry* tex)
	: m_Ref(tex)
{
	if (m_Ref != nullptr)
		++m_Ref->m_ReferenceCount;
}

StaticMeshGeometryRef::~StaticMeshGeometryRef()
{
	if (m_Ref != nullptr && --m_Ref->m_ReferenceCount == 0)
		m_Ref->Unload();
}

void StaticMeshGeometryRef::operator= (std::nullptr_t)
{
	if (m_Ref != nullptr)
		--m_Ref->m_ReferenceCount;

	m_Ref = nullptr;
}

void StaticMeshGeometryRef::operator= (StaticMeshGeometryRef& rhs)
{
	if (m_Ref != nullptr)
		--m_Ref->m_ReferenceCount;

	m_Ref = rhs.m_Ref;

	if (m_Ref != nullptr)
		++m_Ref->m_ReferenceCount;
}

bool StaticMeshGeometryRef::IsValid() const
{
	return m_Ref && m_Ref->IsValid();
}

StaticMeshGeometry* StaticMeshGeometryRef::Get()
{
	return (StaticMeshGeometry*)m_Ref;
}

StaticMeshGeometry* StaticMeshGeometryRef::operator->()
{
	HE_ASSERT(m_Ref != nullptr);
	return (StaticMeshGeometry*)m_Ref;
}


#include "RendererPCH.h"

#include "AnimationManager.h"

#include "StringHelper.h"
#include "Transform.h"


HAnimation FAnimationManager::LoadAnimation( const String& Filename )
{
	return FindOrLoadAnimationMeshFromFile( Filename );
}

void ProcessAnimation( const aiScene* pScene, FAnimation& outAnim)
{
    aiAnimation* pAnimation = pScene->mAnimations[0];
    outAnim.m_Name = pAnimation->mName.C_Str();
    outAnim.m_Duration = pAnimation->mDuration;
    outAnim.m_TicksPerSecond = pAnimation->mTicksPerSecond;

    for (uint32 i = 0; i < pAnimation->mNumChannels; i++)
    {
        aiNodeAnim* pBoneAnim = pAnimation->mChannels[i];
        const Char* BoneName = pBoneAnim->mNodeName.C_Str();

        if (outAnim.m_KeyFrames.find( BoneName ) == outAnim.m_KeyFrames.end())
            outAnim.m_KeyFrames[BoneName] = std::vector<FAnimationKeyFrame>( pBoneAnim->mNumPositionKeys );

        for (uint32 j = 0; j < pBoneAnim->mNumPositionKeys; j++)
        {
            FAnimationKeyFrame& animatedFrame = outAnim.m_KeyFrames[BoneName][j];

            animatedFrame.m_Timestamp = (float)pBoneAnim->mPositionKeys[j].mTime;
            const aiVector3D& keyPos = pBoneAnim->mPositionKeys[j].mValue;
            const aiQuaternion& keyRot = pBoneAnim->mRotationKeys[j].mValue;
            animatedFrame.m_AnimatedTransform.SetPosition( keyPos.x, keyPos.y, keyPos.z );
            animatedFrame.m_AnimatedTransform.SetRotation( FQuat(keyRot.x, keyRot.y, keyRot.z, keyRot.w) );
        }
    }

}

ManagedAnimation* FAnimationManager::FindOrLoadAnimationMeshFromFile( const String& FilePath )
{
	String AnimName = StringHelper::GetFilenameFromDirectory( FilePath );
    
    ManagedAnimation* pAnimation = nullptr;
    {
        ScopedCriticalSection Guard( m_Mutex );

        auto iter = m_AnimationCache.find( AnimName );
        if (iter != m_AnimationCache.end())
        {
            // If a texture was already created make sure it has finished loading before
            // returning a point to it.
            pAnimation = iter->second.get();
            pAnimation->WaitForLoad();
            return pAnimation;
        }
        else
        {
            // If it's not found, create a new managed texture and start loading it.
            pAnimation = new FAnimation();
            //pTexture->SetName( key );
            m_AnimationCache[AnimName].reset( pAnimation );
        }
    }

    Assimp::Importer Importer;
    const aiScene* pScene = Importer.ReadFile( FilePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_PopulateArmatureData );
    if (!pScene) {
        R_LOG( Error, TEXT( "Failed to read animation from file: %s" ), StringHelper::UTF8ToUTF16( Importer.GetErrorString() ).c_str() );
        HE_ASSERT( false );
        return nullptr;
    }
    ProcessAnimation( pScene, pAnimation->GetAsset() );

    return pAnimation;
}


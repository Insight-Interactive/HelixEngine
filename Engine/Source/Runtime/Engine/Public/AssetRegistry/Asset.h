#pragma once


enum EAssetType
{
	AT_Invalid,
	AT_Mesh,
};


class HAsset
{
public:
	virtual void Serialize( const char* Filename ) = 0;
	virtual void DeSerialize( const char* Filename ) = 0;

protected:
	HAsset(const EAssetType& Type)
		: m_Type(Type)
	{
	}
	virtual ~HAsset()
	{
	}

protected:
	EAssetType m_Type;
};

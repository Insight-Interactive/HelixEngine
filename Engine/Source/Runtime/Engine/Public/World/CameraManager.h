// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once


class HWorld;
class HCameraComponent;

class HCameraManager
{
public:
	HCameraManager(HWorld* pWorld);
	~HCameraManager();

	void CacheCamera( HCameraComponent* pCamera );
	void RemoveCachedCamera( HCameraComponent* pCamera );

	void Tick( float DeltaTime );

private:
	std::vector<HCameraComponent*> m_Cameras;
	HWorld* m_pWorld;
};

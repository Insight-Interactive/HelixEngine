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

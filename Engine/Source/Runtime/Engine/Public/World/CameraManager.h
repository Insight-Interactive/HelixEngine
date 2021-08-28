#pragma once


class HWorld;
class HCameraComponent;

class CameraManager
{
public:
	CameraManager(HWorld* pWorld);
	~CameraManager();

	void CacheCamera( HCameraComponent* pCamera );
	void RemoveCachedCamera( HCameraComponent* pCamera );

	void Tick( float DeltaTime );

private:
	std::vector<HCameraComponent*> m_Cameras;
	HWorld* m_pWorld;
};

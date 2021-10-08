#pragma once

#include "Engine/ViewportContext.h"


class PreferencesViewport : public FViewportContext
{
	using Super = FViewportContext;
public:

	virtual void Update( float DeltaTime ) override;
	virtual void Render() override;

protected:


};

#pragma once

#include "Engine/ViewportContext.h"


class PreferencesViewport : public ViewportContext
{
	using Super = ViewportContext;
public:

	virtual void Update( float DeltaTime ) override;
	virtual void Render() override;

protected:


};

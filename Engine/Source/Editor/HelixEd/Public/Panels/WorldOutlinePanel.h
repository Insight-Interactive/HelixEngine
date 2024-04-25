// Copyright 2021 Insight Interactive. All Rights Reserved.
#pragma once

#include "Panels/Panel.h"


class HWorld;

class WorldOutlinePanel : public Panel
{
public:
	WorldOutlinePanel();
	virtual ~WorldOutlinePanel();

	virtual void UnInitialize() override;

	virtual void Tick( float DeltaTime ) override;
	virtual void Render( FCommandContext& CmdCtx ) override;

	void SetWorld( HWorld* pWorld );
	HWorld* GetWorld();

private:
	void SetTitlebarText();

protected:
	HWorld* m_pWorld;

};


//
// Inline function implementations
//

inline void WorldOutlinePanel::SetWorld( HWorld* pWorld )
{
	m_pWorld = pWorld;
}

inline HWorld* WorldOutlinePanel::GetWorld()
{
	return m_pWorld;
}

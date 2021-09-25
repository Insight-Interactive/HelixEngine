#pragma once

#include "Panels/Panel.h"

class HObject;

class DetailsPanel : public Panel
{
public:
	DetailsPanel();
	virtual ~DetailsPanel();

	virtual void Initialize() override;
	virtual void UnInitialize() override;

	virtual void Tick( float DeltaTime ) override;
	virtual void Render( ICommandContext& CmdCtx ) override;

	void SetSelectedObject( HObject* pSelectedObject );

protected:
	HObject* m_pSelectedObject;

};

//
// Inline function implementations
//

inline void DetailsPanel::SetSelectedObject( HObject* pSelectedObject )
{
	m_pSelectedObject = pSelectedObject;
}

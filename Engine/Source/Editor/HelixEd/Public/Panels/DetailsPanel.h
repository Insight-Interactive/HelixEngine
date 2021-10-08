#pragma once

#include "Panels/Panel.h"

class HObject;
class AActor;

class DetailsPanel : public Panel
{
public:
	DetailsPanel();
	virtual ~DetailsPanel();

	virtual void Initialize() override;
	virtual void UnInitialize() override;

	virtual void Tick( float DeltaTime ) override;
	virtual void Render( FCommandContext& CmdCtx ) override;

	void SetSelectedObject( HObject* pSelectedObject );

protected:
	void PreviewActor( AActor* pActor );

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

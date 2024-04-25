#pragma once

class FCommandContext;

class FUIWidget
{
	friend class FUIPanel;
public:

	void SetPosition( const FVector2& NewPosition ) { m_Position = NewPosition; }
	const FVector2& GetPosition() const { return m_Position; }

protected:
	FUIWidget() 
		: m_Position(0.f, 0.f)
	{
	}
	virtual ~FUIWidget() {}

	virtual void Initialize()
	{
	}
	virtual void UnInitialize()
	{
	}

	virtual void Render( FCommandContext& UIContext )
	{
	}

protected:
	FVector2 m_Position;

};

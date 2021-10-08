#pragma once

#include "Panels/Panel.h"

using MenuItemCallback = std::function<void( )>;

class MenuBarPanel : public Panel
{
public:
	MenuBarPanel();
	virtual ~MenuBarPanel();


	virtual void Initialize() override;
	virtual void UnInitialize() override;

	virtual void Tick( float DeltaTime ) override;
	virtual void Render( FCommandContext& CmdCtx ) override;

	template <typename ClassType, typename MethodTarget>
	void AddMenuItem( const Char* MenuName, const Char* ItemName, ClassType* Class, void(MethodTarget::* Fn)() );

private:
	std::unordered_map< String, std::vector<std::pair<String, MenuItemCallback>> > m_MenuItems;

};


//
// Inline function implementations
//

template <typename ClassType, typename MethodTarget>
inline void MenuBarPanel::AddMenuItem( const Char* MenuName, const Char* ItemName, ClassType* Class, void(MethodTarget::* Fn)() )
{
	m_MenuItems[MenuName].emplace_back(ItemName, std::bind( Fn, Class));
}

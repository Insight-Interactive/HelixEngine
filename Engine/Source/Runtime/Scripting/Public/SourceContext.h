#pragma once


struct lua_State;

class SourceContext
{
public:
	SourceContext();
	~SourceContext();

	void Setup();
	void Teardown();

private:
	lua_State* m_pLuaSrcContext;

};

// Copyright 2024 Insight Interactive. All Rights Reserved.
/*
	File: EventEmitter.h
	
	Source: None
	
	Author Garrett Courtney

	Description:
	Emits events to all listeners attached to it.
*/
#pragma once


template <typename CallbackReturnType, typename CallbackInputParamType>
class EventEmitter
{
public:
	
	template <typename ClassTarget>
	void AddListener( ClassTarget* Class, void( ClassTarget::* Fn )( CallbackInputParamType ) );

	void EmitEvent( CallbackInputParamType EventData );

protected:
	EventEmitter()
	{
	}
	virtual ~EventEmitter()
	{
	}

private:
	std::vector< std::function<CallbackReturnType(CallbackInputParamType)> > m_Listeners;

};

//
// Inline function implementations
//

template <typename CallbackReturnType, typename CallbackInputParamType>
template <typename ClassTarget>
FORCEINLINE void EventEmitter<CallbackReturnType, CallbackInputParamType>::AddListener( ClassTarget* Class, void( ClassTarget::* Fn )( CallbackInputParamType ) )
{
	m_Listeners.push_back( std::bind( Fn, Class, std::placeholders::_1 ) );
}

template <typename CallbackReturnType, typename CallbackInputParamType>
FORCEINLINE void EventEmitter<CallbackReturnType, CallbackInputParamType>::EmitEvent( CallbackInputParamType EventData )
{
	for (size_t i = 0; i < m_Listeners.size(); ++i)
	{
		m_Listeners[i]( EventData );
	}
}

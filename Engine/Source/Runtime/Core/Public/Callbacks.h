#pragma once

class HEngine;
enum EWindowMode;


/*
	Invokes a member function for a class.
	@param ObjectPtr - The object to invoke the function on.
	@param Fn - The function to call on the object.
	@param ... - Arguments to pass to the function.
*/
#define HE_INVOKE_MEMBER_FN( ObjectPtr, Fn, ... ) (( ObjectPtr )->*( Fn ))(__VA_ARGS__)


/*
	void Engine::EngineFunction();
*/
typedef void (HEngine::*EngineCallbackFn)();

/*
	void SomeFunction(void* pValue);
*/
typedef void (OutVoidInVoidPtrFn)(void*);

/*
	void SomeFunction();
*/
typedef void (OutVoidInVoidFn)(void);

/*
	void SomeFunction(float);
*/
typedef void (OutVoidInFloatFn)(float);

/*
	void SomeFunction(EWindowMode Mode);
*/
typedef void (OutVoidInEWindowModeFn)(EWindowMode);

/*
	void SomeFunction(void* pData);
*/
typedef void(*JobEntryPointFn)(void*);

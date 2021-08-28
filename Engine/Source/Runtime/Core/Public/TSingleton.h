#pragma once

#include "CoreFwd.h"

template <typename ClassType>
class TSingleton
{
public:
	static FORCEINLINE ClassType* GetInstance();
	static FORCEINLINE bool IsValid();


private:
	static void InValidate();

	static ClassType* s_Instance;

protected:
	FORCEINLINE TSingleton();
	FORCEINLINE virtual ~TSingleton();
};

//
// TSingleton implementation
//

template <typename ClassType>
ClassType* TSingleton<ClassType>::s_Instance = NULL;

template <typename ClassType>
FORCEINLINE TSingleton<ClassType>::TSingleton()
{
	if (s_Instance == NULL)
	{
		s_Instance = (ClassType*)this;
	}
}

template <typename ClassType>
FORCEINLINE TSingleton<ClassType>::~TSingleton()
{
	InValidate();
}

template <typename ClassType>
/*static*/ FORCEINLINE ClassType* TSingleton<ClassType>::GetInstance()
{
	HE_ASSERT(s_Instance != NULL); // Trying to get an instance of a class that has not been initialized.
	return s_Instance;
}

template <typename ClassType>
/*static*/ FORCEINLINE bool TSingleton<ClassType>::IsValid()
{
	return s_Instance != NULL;
}

template <typename ClassType>
/*static*/ FORCEINLINE void TSingleton<ClassType>::InValidate()
{
	s_Instance = NULL;
}

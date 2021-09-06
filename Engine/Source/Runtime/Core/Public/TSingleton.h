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

	static ClassType* SInstance;

protected:
	FORCEINLINE TSingleton();
	FORCEINLINE virtual ~TSingleton();
};

//
// TSingleton implementation
//

template <typename ClassType>
ClassType* TSingleton<ClassType>::SInstance = NULL;

template <typename ClassType>
FORCEINLINE TSingleton<ClassType>::TSingleton()
{
	if (SInstance == NULL)
	{
		SInstance = (ClassType*)this;
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
	HE_ASSERT(SInstance != NULL); // Trying to get an instance of a class that has not been initialized.
	return SInstance;
}

template <typename ClassType>
/*static*/ FORCEINLINE bool TSingleton<ClassType>::IsValid()
{
	return SInstance != NULL;
}

template <typename ClassType>
/*static*/ FORCEINLINE void TSingleton<ClassType>::InValidate()
{
	SInstance = NULL;
}

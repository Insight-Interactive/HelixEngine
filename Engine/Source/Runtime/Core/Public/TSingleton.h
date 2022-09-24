#pragma once

#include "CoreFwd.h"

/*
	Sets a single instance of a class to be the only instance available for its type.
*/
template <typename ClassType, bool bShouldHeapAllocate = false>
class TSingleton
{
public:
	static ClassType* GetInstance();
	static bool IsValid();

private:
	static void InValidate();

	static ClassType* SInstance;

protected:
	TSingleton();
	virtual ~TSingleton();
};

//
// TSingleton implementation
//

template <typename ClassType, bool bShouldHeapAllocate>
ClassType* TSingleton<ClassType, bShouldHeapAllocate>::SInstance = NULL;

template <typename ClassType, bool bShouldHeapAllocate>
FORCEINLINE TSingleton<ClassType, bShouldHeapAllocate>::TSingleton()
{
	if constexpr (bShouldHeapAllocate)
	{
		SInstance = new ClassType();
	}
	else
	{
		if (SInstance == NULL)
		{
			SInstance = (ClassType*)this;
		}
	}
	HE_ASSERT( SInstance != NULL );
}

template <typename ClassType, bool bShouldHeapAllocate>
FORCEINLINE TSingleton<ClassType, bShouldHeapAllocate>::~TSingleton()
{
	InValidate();
}

template <typename ClassType, bool bShouldHeapAllocate>
/*static*/ FORCEINLINE ClassType* TSingleton<ClassType, bShouldHeapAllocate>::GetInstance()
{
	HE_ASSERT(SInstance != NULL); // Trying to get an instance of a class that has not been initialized.
	return SInstance;
}

template <typename ClassType, bool bShouldHeapAllocate>
/*static*/ FORCEINLINE bool TSingleton<ClassType, bShouldHeapAllocate>::IsValid()
{
	return SInstance != NULL;
}

template <typename ClassType, bool bShouldHeapAllocate>
/*static*/ FORCEINLINE void TSingleton<ClassType, bShouldHeapAllocate>::InValidate()
{
	if constexpr (bShouldHeapAllocate)
	{
		if (SInstance != NULL)
		{
			delete SInstance;
		}
	}
	SInstance = NULL;
}

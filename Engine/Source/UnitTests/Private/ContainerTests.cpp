// Copyright 2024 Insight Interactive. All Rights Reserved.
#include "PCH.h"

#include "UnitTestMacros.h"

#include "CoreFwd.h"
#include "Containers/TArray.h"
#include "Containers/TDynamicArray.h"


namespace ContainerTests
{

	int Run()
	{
		UT_LOG(Log, TEXT("Starting Container varification tests."));

		// TArray
		{
			TArray<uint32, 10> Arr;
			for (uint32 i = 0; i < Arr.Length(); ++i)
			{
				Arr[i] = i;
				HE_ASSERT(Arr[i] == i);
			}

			uint32 Val = Arr.At(2);
			HE_ASSERT(Val == 2);

			HE_ASSERT(Arr.Length() == 10);
		}

		// TDynamicArray
		{
			// Test EmplaceBack
			{
				class TestClass
				{
				public:
					TestClass(int _Val, int _Val1)
						: Val(_Val)
						, Val1(_Val1)
					{
					}
					~TestClass()
					{
					}
					TestClass(const TestClass&) noexcept
					{
						HE_ASSERT(false);
					}
					TestClass(const TestClass&&) noexcept
					{
						HE_ASSERT(false);
					}

					int Val;
					int Val1;
				};
				TDynamicArray<TestClass> DynArr;
				int Target1 = 1, Target2 = 2;
				DynArr.EmplaceBack(Target1, Target2);
				HE_ASSERT(DynArr[0].Val == Target1);
				HE_ASSERT(DynArr[0].Val1 == Target2);
			}

			// Test Trim
			{
				// Ensure trim doesnt compromise the data.
				TDynamicArray<uint8> DynArr;
				DynArr.Reserve(20);
				for (uint8 i = 0; i < DynArr.Size(); i++)
				{
					DynArr[i] = i + 1;
				}
				DynArr.TrimExcess();
				for (uint8 i = 0; i < DynArr.Size(); i++)
				{
					HE_ASSERT(DynArr[i] == i + 1);
				}
			}

			// Test PushBack.
			{
				TDynamicArray<uint32> DynArr;
				DynArr.PushBack(1);
				DynArr.PushBack(2);
				DynArr.PushBack(3);
				DynArr.PushBack(4);
				for (uint32 i = 0; i < DynArr.Size(); i++)
				{
					HE_ASSERT(DynArr[i] == i + 1);
				}
			}

			// Ensure resizing didn't compromise the data.
			{
				TDynamicArray<uint32> DynArr(10);
				for (uint32 i = 0; i < 10; ++i)
				{
					DynArr[i] = i * 2;
				}
				for (uint32 i = 0; i < 10; ++i)
				{
					HE_ASSERT(DynArr[i] == i*2);
				}
			}

			// Ensure shrinking doesnt corrupt the data.
			{
				TDynamicArray<uint32> DynArr(4);
				UT_LOG(Log, TEXT("Container Size: %u"), DynArr.Size());
				for (uint32 i = 0; i < DynArr.Size(); i++)
				{
					DynArr[i] = i * 2;
				}
				DynArr.Resize(2);
				HE_ASSERT(DynArr[1] == 2);
			}

			// Make sure resizing does not copy construct elements.
			{
				class TestClass
				{
				public:
					TestClass()
					{
					}
					~TestClass()
					{
					}
					TestClass(const TestClass&) noexcept
					{
						HE_ASSERT(false);
					}
					TestClass(const TestClass&&) noexcept
					{
						HE_ASSERT(false);
					}
				};
				TDynamicArray<TestClass> DynArr;
				DynArr.Resize(10);
			}

			// Test RemoveAt
			{
				TDynamicArray<uint8> DynArr(8);
				for (uint8 i = 0; i < DynArr.Size(); i++)
				{
					DynArr.PushBack(i + 1);
				}
				DynArr.RemoveAt(3);
			}

			// Test PopBack
			{
				TDynamicArray<uint8> DynArr(3);
				for (uint8 i = 0; i < DynArr.Size(); i++)
				{
					DynArr.PushBack(i + 1);
				}
				DynArr.PopBack();

				HE_ASSERT(DynArr[1] == 2);
			}
		}

		UT_LOG(Log, TEXT("Finished Container varification tests. All tests passed."));
		return 0;
	}
}

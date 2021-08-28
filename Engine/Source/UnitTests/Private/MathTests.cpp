#include "PCH.h"

#include "MathCore.h"

namespace MathTests
{
	int Run()
	{
		// Addition
		{
			FVector3 one(1.f);
			FVector3 two(2.f);

			FVector3 res = one + two;
			HE_ASSERT(res == FVector3(3.f));
		}

		// Subtraction
		{
			FVector3 one(1.f);
			FVector3 two(2.f);

			FVector3 res = one - two;
			HE_ASSERT(res == FVector3(-1.f));
		}

		// Multiplication
		{
			FVector3 one(3.f);
			FVector3 two(2.f);

			FVector3 res = one * two;
			HE_ASSERT(res == FVector3(6.f));
		}

		// Division
		{
			FVector3 one(10.f);
			FVector3 two(5.f);

			FVector3 res = one / two;
			HE_ASSERT(res == FVector3(2.f));
		}

		// Dot product
		{
			FVector3 A(1.f, 4.f, 2.f);
			FVector3 B(3.f, 8.f, 9.f);

			float Res = A.Dot(B);
			HE_ASSERT(Res == 27.f);
		}

		// Cross product
		{

			FVector3 A = FVector3::Left;
			FVector3 B = FVector3::Right;

			FVector3 Res = A.Cross(B);
			HE_ASSERT(Res == FVector3::Forward);
		}

		// Vector size
		{
			HE_ASSERT(sizeof(FVector3) == sizeof(float) * 3.f);
		}

		return 0;
	}
}

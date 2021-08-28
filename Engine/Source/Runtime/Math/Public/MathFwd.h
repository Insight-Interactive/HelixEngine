#pragma once

#if MATH_MODULE
#	define MATH_API //__declspec(dllexport)
#else
#	define MATH_API //__declspec(dllimport)
#endif

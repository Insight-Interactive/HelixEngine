#pragma once

#include "DataTypes.h"


// ---------------
//      Types
// ---------------

// Represents a hashed string.
typedef int32 StringHashValue;


// This requires SSE4.2 which is present on Intel Nehalem (Nov. 2008)
// and AMD Bulldozer (Oct. 2011) processors.  I could put a runtime
// check for this, but I'm just going to assume people playing with
// DirectX 12 on Windows 10 have fairly recent machines.
#if defined _M_X64 && !defined HE_PROSPERO
#   define ENABLE_SSE_CRC32 1
#   include <intrin.h>
#else
#   define ENABLE_SSE_CRC32 0
#endif

#if ENABLE_SSE_CRC32
#   pragma intrinsic(_mm_crc32_u32)
#   pragma intrinsic(_mm_crc32_u64)
#endif

template <typename T> 
FORCEINLINE T AlignUpWithMask(T value, size_t mask)
{
    return (T)(((size_t)value + mask) & ~mask);
}

template <typename T> 
FORCEINLINE T AlignDownWithMask(T value, size_t mask)
{
    return (T)((size_t)value & ~mask);
}

template <typename T> 
FORCEINLINE T AlignUp(T value, size_t alignment)
{
    return AlignUpWithMask(value, alignment - 1);
}

template <typename T> 
FORCEINLINE T AlignDown(T value, size_t alignment)
{
    return AlignDownWithMask(value, alignment - 1);
}

FORCEINLINE uint64 HashRange(const uint32* const Begin, const uint32* const End, int64 Hash)
{
#if ENABLE_SSE_CRC32
    const uint64* Iter64 = (const uint64*)AlignUp(Begin, 8);
    const uint64* const End64 = (const uint64* const)AlignDown(End, 8);

    // If not 64-bit aligned, start with a single u32
    if ((uint32*)Iter64 > Begin)
        Hash = _mm_crc32_u32((uint32)Hash, *Begin);

    // Iterate over consecutive u64 values
    while (Iter64 < End64)
        Hash = _mm_crc32_u64((uint32)Hash, *Iter64++);

    // If there is a 32-bit remainder, accumulate that
    if ((uint32*)Iter64 < End)
        Hash = _mm_crc32_u32((uint32)Hash, *(uint32*)Iter64);
#else
    // An inexpensive hash for CPUs lacking SSE4.2
    for (const uint32* Iter = Begin; Iter < End; ++Iter)
        Hash = 16777619U * Hash ^ *Iter;
#endif

    return Hash;
}

template <typename T> 
FORCEINLINE size_t HashState(const T* StateDesc, size_t Count = 1, size_t Hash = 2166136261U)
{
    static_assert((sizeof(T) & 3) == 0 && alignof(T) >= 4, "State object is not word-aligned");
    return HashRange((uint32*)StateDesc, (uint32*)(StateDesc + Count), Hash);
}

template <typename CharType>
FORCEINLINE StringHashValue StringHash( const CharType* String, uint64 Length)
{
    int32 HashResult = 0;
    while ((*String != NULL) && (Length-- != 0))
    {
        HashResult += *String;
        HashResult += (HashResult << 10);
        HashResult ^= (HashResult >> 6);
        String++;
    }

    HashResult += (HashResult << 3);
    HashResult ^= (HashResult >> 11);
    HashResult += (HashResult << 15);

    return HashResult;
}


template <typename CharType>
FORCEINLINE StringHashValue StringHash(const CharType* String)
{
    int32 HashResult = 0;
    while (*String != NULL)
    {
        HashResult += *String;
        HashResult += (HashResult << 10);
        HashResult ^= (HashResult >> 6);
        String++;
    }

    HashResult += (HashResult << 3);
    HashResult ^= (HashResult >> 11);
    HashResult += (HashResult << 15);

    return HashResult;
}

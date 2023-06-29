#pragma once

#include "MathFwd.h"
#include "CoreFwd.h"

#include <DirectXMath.h>

using namespace DirectX;

struct FVector2;
struct FVector3;
struct FVector4;
struct FMatrix;
struct FQuat;
struct Plane;


constexpr float HE_PI = 3.141592654f;
constexpr float HE_2PI = 6.283185307f;
constexpr float HE_1DIVPI = 0.318309886f;
constexpr float HE_1DIV2PI = 0.159154943f;
constexpr float HE_PIDIV2 = 1.570796327f;
constexpr float HE_PIDIV4 = 0.785398163f;

namespace Math
{
    template <typename T>
    inline T Clamp( const T& Value, const T& Min, const T& Max )
    {
        const T Val = Value < Min ? Min : Value;
        return Val > Max ? Max : Val;
    }

    template <typename T>
    inline T DegreesToRadians( const T& Degrees )
    {
        return Degrees * (HE_PI / 180.0f);
    }

    template <typename T>
    inline T RadiansToDegrees( const T& Radians )
    {
        return Radians * (180.0f / HE_PI);
    }
}

struct FAngles
{
    FAngles()
        : pitch( 0.f )
        , yaw( 0.f )
        , roll( 0.f )
    {
    }
    
    float pitch;
    float yaw;
    float roll;

};

//------------------------------------------------------------------------------
// 2D vector
struct MATH_API FVector2 : public XMFLOAT2
{
    FVector2() noexcept : XMFLOAT2(0.f, 0.f) {}
    constexpr explicit FVector2(float ix) noexcept : XMFLOAT2(ix, ix) {}
    constexpr FVector2(float ix, float iy) noexcept : XMFLOAT2(ix, iy) {}
    explicit FVector2(const float* pArray) noexcept : XMFLOAT2(pArray) {}
    FVector2(FXMVECTOR V) noexcept { XMStoreFloat2(this, V); }
    FVector2(const XMFLOAT2& V) noexcept { this->x = V.x; this->y = V.y; }
    explicit FVector2(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; }

    FVector2(const FVector2&) = default;
    FVector2& operator=(const FVector2&) = default;

    FVector2(FVector2&&) = default;
    FVector2& operator=(FVector2&&) = default;

    operator XMVECTOR() const noexcept { return XMLoadFloat2(this); }

    // Comparison operators
    bool operator == (const FVector2& V) const noexcept;
    bool operator != (const FVector2& V) const noexcept;

    // Assignment operators
    FVector2& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; return *this; }
    FVector2& operator+= (const FVector2& V) noexcept;
    FVector2& operator-= (const FVector2& V) noexcept;
    FVector2& operator*= (const FVector2& V) noexcept;
    FVector2& operator*= (float S) noexcept;
    FVector2& operator/= (float S) noexcept;

    // Unary operators
    FVector2 operator+ () const noexcept { return *this; }
    FVector2 operator- () const noexcept { return FVector2(-x, -y); }

    String ToString() const;

    // Vector operations
    bool InBounds(const FVector2& Bounds) const noexcept;

    float Length() const noexcept;
    float LengthSquared() const noexcept;

    float Dot(const FVector2& V) const noexcept;
    void Cross(const FVector2& V, FVector2& result) const noexcept;
    FVector2 Cross(const FVector2& V) const noexcept;

    void Normalize() noexcept;
    void Normalize(FVector2& result) const noexcept;

    void Clamp(const FVector2& vmin, const FVector2& vmax) noexcept;
    void Clamp(const FVector2& vmin, const FVector2& vmax, FVector2& result) const noexcept;

    // Static functions
    static float Distance(const FVector2& v1, const FVector2& v2) noexcept;
    static float DistanceSquared(const FVector2& v1, const FVector2& v2) noexcept;

    static void Min(const FVector2& v1, const FVector2& v2, FVector2& result) noexcept;
    static FVector2 Min(const FVector2& v1, const FVector2& v2) noexcept;

    static void Max(const FVector2& v1, const FVector2& v2, FVector2& result) noexcept;
    static FVector2 Max(const FVector2& v1, const FVector2& v2) noexcept;

    static void Lerp(const FVector2& v1, const FVector2& v2, float t, FVector2& result) noexcept;
    static FVector2 Lerp(const FVector2& v1, const FVector2& v2, float t) noexcept;

    static void SmoothStep(const FVector2& v1, const FVector2& v2, float t, FVector2& result) noexcept;
    static FVector2 SmoothStep(const FVector2& v1, const FVector2& v2, float t) noexcept;

    static void Barycentric(const FVector2& v1, const FVector2& v2, const FVector2& v3, float f, float g, FVector2& result) noexcept;
    static FVector2 Barycentric(const FVector2& v1, const FVector2& v2, const FVector2& v3, float f, float g) noexcept;

    static void CatmullRom(const FVector2& v1, const FVector2& v2, const FVector2& v3, const FVector2& v4, float t, FVector2& result) noexcept;
    static FVector2 CatmullRom(const FVector2& v1, const FVector2& v2, const FVector2& v3, const FVector2& v4, float t) noexcept;

    static void Hermite(const FVector2& v1, const FVector2& t1, const FVector2& v2, const FVector2& t2, float t, FVector2& result) noexcept;
    static FVector2 Hermite(const FVector2& v1, const FVector2& t1, const FVector2& v2, const FVector2& t2, float t) noexcept;

    static void Reflect(const FVector2& ivec, const FVector2& nvec, FVector2& result) noexcept;
    static FVector2 Reflect(const FVector2& ivec, const FVector2& nvec) noexcept;

    static void Refract(const FVector2& ivec, const FVector2& nvec, float refractionIndex, FVector2& result) noexcept;
    static FVector2 Refract(const FVector2& ivec, const FVector2& nvec, float refractionIndex) noexcept;

    static void FTransform(const FVector2& v, const FQuat& quat, FVector2& result) noexcept;
    static FVector2 FTransform(const FVector2& v, const FQuat& quat) noexcept;

    static void FTransform(const FVector2& v, const FMatrix& m, FVector2& result) noexcept;
    static FVector2 FTransform(const FVector2& v, const FMatrix& m) noexcept;
    static void FTransform(_In_reads_(count) const FVector2* varray, size_t count, const FMatrix& m, _Out_writes_(count) FVector2* resultArray) noexcept;

    static void FTransform(const FVector2& v, const FMatrix& m, FVector4& result) noexcept;
    static void FTransform(_In_reads_(count) const FVector2* varray, size_t count, const FMatrix& m, _Out_writes_(count) FVector4* resultArray) noexcept;

    static void TransformNormal(const FVector2& v, const FMatrix& m, FVector2& result) noexcept;
    static FVector2 TransformNormal(const FVector2& v, const FMatrix& m) noexcept;
    static void TransformNormal(_In_reads_(count) const FVector2* varray, size_t count, const FMatrix& m, _Out_writes_(count) FVector2* resultArray) noexcept;

    // Constants
    static const FVector2 Up;
    static const FVector2 Down;
    static const FVector2 Left;
    static const FVector2 Right;
    static const FVector2 Zero;
    static const FVector2 One;
    static const FVector2 UnitX;
    static const FVector2 UnitY;
};

// Binary operators
FVector2 operator+ (const FVector2& V1, const FVector2& V2) noexcept;
FVector2 operator- (const FVector2& V1, const FVector2& V2) noexcept;
FVector2 operator* (const FVector2& V1, const FVector2& V2) noexcept;
FVector2 operator* (const FVector2& V, float S) noexcept;
FVector2 operator/ (const FVector2& V1, const FVector2& V2) noexcept;
FVector2 operator/ (const FVector2& V, float S) noexcept;
FVector2 operator* (float S, const FVector2& V) noexcept;


//------------------------------------------------------------------------------
// 3D vector
struct MATH_API FVector3 : public XMFLOAT3
{
    FVector3() noexcept : XMFLOAT3(0.f, 0.f, 0.f) {}
    constexpr explicit FVector3(float ix) noexcept : XMFLOAT3(ix, ix, ix) {}
    constexpr FVector3(float ix, float iy, float iz) noexcept : XMFLOAT3(ix, iy, iz) {}
    explicit FVector3(_In_reads_(3) const float* pArray) noexcept : XMFLOAT3(pArray) {}
    FVector3(FXMVECTOR V) noexcept { XMStoreFloat3(this, V); }
    FVector3(const XMFLOAT3& V) noexcept { this->x = V.x; this->y = V.y; this->z = V.z; }
    explicit FVector3(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; }

    FVector3(const FVector3&) = default;
    FVector3& operator=(const FVector3&) = default;

    FVector3(FVector3&&) = default;
    FVector3& operator=(FVector3&&) = default;

    operator XMVECTOR() const noexcept { return XMLoadFloat3(this); }

    // Comparison operators
    bool operator == (const FVector3& V) const noexcept;
    bool operator != (const FVector3& V) const noexcept;

    // Assignment operators
    FVector3& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; return *this; }
    FVector3& operator+= (const FVector3& V) noexcept;
    FVector3& operator-= (const FVector3& V) noexcept;
    FVector3& operator*= (const FVector3& V) noexcept;
    FVector3& operator*= (float S) noexcept;
    FVector3& operator/= (float S) noexcept;

    // Unary operators
    FVector3 operator+ () const noexcept { return *this; }
    FVector3 operator- () const noexcept;

    String ToString() const;

    // Vector operations
    bool InBounds(const FVector3& Bounds) const noexcept;

    float Length() const noexcept;
    float LengthSquared() const noexcept;

    float Dot(const FVector3& V) const noexcept;
    void Cross(const FVector3& V, FVector3& result) const noexcept;
    FVector3 Cross(const FVector3& V) const noexcept;

    void Normalize() noexcept;

    void Clamp(const FVector3& vmin, const FVector3& vmax) noexcept;
    void Clamp(const FVector3& vmin, const FVector3& vmax, FVector3& result) const noexcept;

    // Static functions
    static FVector3 Normalize( const FVector3& v1 ) noexcept;

    static float Distance(const FVector3& v1, const FVector3& v2) noexcept;
    static float DistanceSquared(const FVector3& v1, const FVector3& v2) noexcept;

    static void Min(const FVector3& v1, const FVector3& v2, FVector3& result) noexcept;
    static FVector3 Min(const FVector3& v1, const FVector3& v2) noexcept;

    static void Max(const FVector3& v1, const FVector3& v2, FVector3& result) noexcept;
    static FVector3 Max(const FVector3& v1, const FVector3& v2) noexcept;

    static void Lerp(const FVector3& v1, const FVector3& v2, float t, FVector3& result) noexcept;
    static FVector3 Lerp(const FVector3& v1, const FVector3& v2, float t) noexcept;

    static void SmoothStep(const FVector3& v1, const FVector3& v2, float t, FVector3& result) noexcept;
    static FVector3 SmoothStep(const FVector3& v1, const FVector3& v2, float t) noexcept;

    static void Barycentric(const FVector3& v1, const FVector3& v2, const FVector3& v3, float f, float g, FVector3& result) noexcept;
    static FVector3 Barycentric(const FVector3& v1, const FVector3& v2, const FVector3& v3, float f, float g) noexcept;

    static void CatmullRom(const FVector3& v1, const FVector3& v2, const FVector3& v3, const FVector3& v4, float t, FVector3& result) noexcept;
    static FVector3 CatmullRom(const FVector3& v1, const FVector3& v2, const FVector3& v3, const FVector3& v4, float t) noexcept;

    static void Hermite(const FVector3& v1, const FVector3& t1, const FVector3& v2, const FVector3& t2, float t, FVector3& result) noexcept;
    static FVector3 Hermite(const FVector3& v1, const FVector3& t1, const FVector3& v2, const FVector3& t2, float t) noexcept;

    static void Reflect(const FVector3& ivec, const FVector3& nvec, FVector3& result) noexcept;
    static FVector3 Reflect(const FVector3& ivec, const FVector3& nvec) noexcept;

    static void Refract(const FVector3& ivec, const FVector3& nvec, float refractionIndex, FVector3& result) noexcept;
    static FVector3 Refract(const FVector3& ivec, const FVector3& nvec, float refractionIndex) noexcept;

    static void FTransform(const FVector3& v, const FQuat& quat, FVector3& result) noexcept;
    static FVector3 FTransform(const FVector3& v, const FQuat& quat) noexcept;

    static void FTransform(const FVector3& v, const FMatrix& m, FVector3& result) noexcept;
    static FVector3 FTransform(const FVector3& v, const FMatrix& m) noexcept;
    static void FTransform(_In_reads_(count) const FVector3* varray, size_t count, const FMatrix& m, _Out_writes_(count) FVector3* resultArray) noexcept;

    static void FTransform(const FVector3& v, const FMatrix& m, FVector4& result) noexcept;
    static void FTransform(_In_reads_(count) const FVector3* varray, size_t count, const FMatrix& m, _Out_writes_(count) FVector4* resultArray) noexcept;

    static void TransformNormal(const FVector3& v, const FMatrix& m, FVector3& result) noexcept;
    static FVector3 TransformNormal(const FVector3& v, const FMatrix& m) noexcept;
    static void TransformNormal(_In_reads_(count) const FVector3* varray, size_t count, const FMatrix& m, _Out_writes_(count) FVector3* resultArray) noexcept;

    // Constants
    static const FVector3 Zero;
    static const FVector3 One;
    static const FVector3 UnitX;
    static const FVector3 UnitY;
    static const FVector3 UnitZ;
    static const FVector3 Up;
    static const FVector3 Down;
    static const FVector3 Right;
    static const FVector3 Left;
    static const FVector3 Forward;
    static const FVector3 Backward;
};

// Binary operators
FVector3 operator+ (const FVector3& V1, const FVector3& V2) noexcept;
FVector3 operator- (const FVector3& V1, const FVector3& V2) noexcept;
FVector3 operator* (const FVector3& V1, const FVector3& V2) noexcept;
FVector3 operator* (const FVector3& V, float S) noexcept;
FVector3 operator/ (const FVector3& V1, const FVector3& V2) noexcept;
FVector3 operator/ (const FVector3& V, float S) noexcept;
FVector3 operator* (float S, const FVector3& V) noexcept;


//------------------------------------------------------------------------------
// 4D vector
struct MATH_API FVector4 : public XMFLOAT4
{
    FVector4() noexcept : XMFLOAT4(0.f, 0.f, 0.f, 0.f) {}
    constexpr explicit FVector4(float ix) noexcept : XMFLOAT4(ix, ix, ix, ix) {}
    constexpr FVector4(float ix, float iy, float iz, float iw) noexcept : XMFLOAT4(ix, iy, iz, iw) {}
    explicit FVector4(_In_reads_(4) const float* pArray) noexcept : XMFLOAT4(pArray) {}
    FVector4(FXMVECTOR V) noexcept { XMStoreFloat4(this, V); }
    FVector4(const XMFLOAT4& V) noexcept { this->x = V.x; this->y = V.y; this->z = V.z; this->w = V.w; }
    explicit FVector4(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

    FVector4(const FVector4&) = default;
    FVector4& operator=(const FVector4&) = default;

    FVector4(FVector4&&) = default;
    FVector4& operator=(FVector4&&) = default;

    operator XMVECTOR() const  noexcept { return XMLoadFloat4(this); }

    // Comparison operators
    bool operator == (const FVector4& V) const noexcept;
    bool operator != (const FVector4& V) const noexcept;

    // Assignment operators
    FVector4& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
    FVector4& operator+= (const FVector4& V) noexcept;
    FVector4& operator-= (const FVector4& V) noexcept;
    FVector4& operator*= (const FVector4& V) noexcept;
    FVector4& operator*= (float S) noexcept;
    FVector4& operator/= (float S) noexcept;

    // Unary operators
    FVector4 operator+ () const noexcept { return *this; }
    FVector4 operator- () const noexcept;

    String ToString() const;

    // Vector operations
    bool InBounds(const FVector4& Bounds) const noexcept;

    float Length() const noexcept;
    float LengthSquared() const noexcept;

    float Dot(const FVector4& V) const noexcept;
    void Cross(const FVector4& v1, const FVector4& v2, FVector4& result) const noexcept;
    FVector4 Cross(const FVector4& v1, const FVector4& v2) const noexcept;

    void Normalize() noexcept;
    void Normalize(FVector4& result) const noexcept;

    void Clamp(const FVector4& vmin, const FVector4& vmax) noexcept;
    void Clamp(const FVector4& vmin, const FVector4& vmax, FVector4& result) const noexcept;

    // Static functions
    static float Distance(const FVector4& v1, const FVector4& v2) noexcept;
    static float DistanceSquared(const FVector4& v1, const FVector4& v2) noexcept;

    static void Min(const FVector4& v1, const FVector4& v2, FVector4& result) noexcept;
    static FVector4 Min(const FVector4& v1, const FVector4& v2) noexcept;

    static void Max(const FVector4& v1, const FVector4& v2, FVector4& result) noexcept;
    static FVector4 Max(const FVector4& v1, const FVector4& v2) noexcept;

    static void Lerp(const FVector4& v1, const FVector4& v2, float t, FVector4& result) noexcept;
    static FVector4 Lerp(const FVector4& v1, const FVector4& v2, float t) noexcept;

    static void SmoothStep(const FVector4& v1, const FVector4& v2, float t, FVector4& result) noexcept;
    static FVector4 SmoothStep(const FVector4& v1, const FVector4& v2, float t) noexcept;

    static void Barycentric(const FVector4& v1, const FVector4& v2, const FVector4& v3, float f, float g, FVector4& result) noexcept;
    static FVector4 Barycentric(const FVector4& v1, const FVector4& v2, const FVector4& v3, float f, float g) noexcept;

    static void CatmullRom(const FVector4& v1, const FVector4& v2, const FVector4& v3, const FVector4& v4, float t, FVector4& result) noexcept;
    static FVector4 CatmullRom(const FVector4& v1, const FVector4& v2, const FVector4& v3, const FVector4& v4, float t) noexcept;

    static void Hermite(const FVector4& v1, const FVector4& t1, const FVector4& v2, const FVector4& t2, float t, FVector4& result) noexcept;
    static FVector4 Hermite(const FVector4& v1, const FVector4& t1, const FVector4& v2, const FVector4& t2, float t) noexcept;

    static void Reflect(const FVector4& ivec, const FVector4& nvec, FVector4& result) noexcept;
    static FVector4 Reflect(const FVector4& ivec, const FVector4& nvec) noexcept;

    static void Refract(const FVector4& ivec, const FVector4& nvec, float refractionIndex, FVector4& result) noexcept;
    static FVector4 Refract(const FVector4& ivec, const FVector4& nvec, float refractionIndex) noexcept;

    static void FTransform(const FVector2& v, const FQuat& quat, FVector4& result) noexcept;
    static FVector4 FTransform(const FVector2& v, const FQuat& quat) noexcept;

    static void FTransform(const FVector3& v, const FQuat& quat, FVector4& result) noexcept;
    static FVector4 FTransform(const FVector3& v, const FQuat& quat) noexcept;

    static void FTransform(const FVector4& v, const FQuat& quat, FVector4& result) noexcept;
    static FVector4 FTransform(const FVector4& v, const FQuat& quat) noexcept;

    static void FTransform(const FVector4& v, const FMatrix& m, FVector4& result) noexcept;
    static FVector4 FTransform(const FVector4& v, const FMatrix& m) noexcept;
    static void FTransform(_In_reads_(count) const FVector4* varray, size_t count, const FMatrix& m, _Out_writes_(count) FVector4* resultArray) noexcept;

    // Constants
    static const FVector4 Zero;
    static const FVector4 One;
    static const FVector4 UnitX;
    static const FVector4 UnitY;
    static const FVector4 UnitZ;
    static const FVector4 UnitW;
};

// Binary operators
FVector4 operator+ (const FVector4& V1, const FVector4& V2) noexcept;
FVector4 operator- (const FVector4& V1, const FVector4& V2) noexcept;
FVector4 operator* (const FVector4& V1, const FVector4& V2) noexcept;
FVector4 operator* (const FVector4& V, float S) noexcept;
FVector4 operator/ (const FVector4& V1, const FVector4& V2) noexcept;
FVector4 operator/ (const FVector4& V, float S) noexcept;
FVector4 operator* (float S, const FVector4& V) noexcept;


//------------------------------------------------------------------------------
// 4x4 FMatrix (assumes right-handed cooordinates)
struct MATH_API FMatrix : public XMFLOAT4X4
{
    FMatrix() noexcept
        : XMFLOAT4X4(1.f, 0, 0, 0,
            0, 1.f, 0, 0,
            0, 0, 1.f, 0,
            0, 0, 0, 1.f) {}
    constexpr FMatrix(float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33) noexcept
        : XMFLOAT4X4(m00, m01, m02, m03,
            m10, m11, m12, m13,
            m20, m21, m22, m23,
            m30, m31, m32, m33) {}
    explicit FMatrix(const FVector3& r0, const FVector3& r1, const FVector3& r2) noexcept
        : XMFLOAT4X4(r0.x, r0.y, r0.z, 0,
            r1.x, r1.y, r1.z, 0,
            r2.x, r2.y, r2.z, 0,
            0, 0, 0, 1.f) {}
    explicit FMatrix(const FVector4& r0, const FVector4& r1, const FVector4& r2, const FVector4& r3) noexcept
        : XMFLOAT4X4(r0.x, r0.y, r0.z, r0.w,
            r1.x, r1.y, r1.z, r1.w,
            r2.x, r2.y, r2.z, r2.w,
            r3.x, r3.y, r3.z, r3.w) {}
    FMatrix(const XMFLOAT4X4& M) noexcept { memcpy_s(this, sizeof(float) * 16, &M, sizeof(XMFLOAT4X4)); }
    FMatrix(const XMFLOAT3X3& M) noexcept;
    FMatrix(const XMFLOAT4X3& M) noexcept;

    explicit FMatrix(_In_reads_(16) const float* pArray) noexcept : XMFLOAT4X4(pArray) {}
    FMatrix(CXMMATRIX M) noexcept { XMStoreFloat4x4(this, M); }

    FMatrix(const FMatrix&) = default;
    FMatrix& operator=(const FMatrix&) = default;

    FMatrix(FMatrix&&) = default;
    FMatrix& operator=(FMatrix&&) = default;

    operator XMMATRIX() const noexcept { return XMLoadFloat4x4(this); }

    // Comparison operators
    bool operator == (const FMatrix& M) const noexcept;
    bool operator != (const FMatrix& M) const noexcept;

    // Assignment operators
    FMatrix& operator= (const XMFLOAT3X3& M) noexcept;
    FMatrix& operator= (const XMFLOAT4X3& M) noexcept;
    FMatrix& operator+= (const FMatrix& M) noexcept;
    FMatrix& operator-= (const FMatrix& M) noexcept;
    FMatrix& operator*= (const FMatrix& M) noexcept;
    FMatrix& operator*= (float S) noexcept;
    FMatrix& operator/= (float S) noexcept;

    FMatrix& operator/= (const FMatrix& M) noexcept;
    // Element-wise divide

    // Unary operators
    FMatrix operator+ () const noexcept { return *this; }
    FMatrix operator- () const noexcept;

    // Properties
    FVector3 Up() const noexcept { return FVector3(_21, _22, _23); }
    void Up(const FVector3& v) noexcept { _21 = v.x; _22 = v.y; _23 = v.z; }

    FVector3 Down() const  noexcept { return FVector3(-_21, -_22, -_23); }
    void Down(const FVector3& v) noexcept { _21 = -v.x; _22 = -v.y; _23 = -v.z; }

    FVector3 Right() const noexcept { return FVector3(_11, _12, _13); }
    void Right(const FVector3& v) noexcept { _11 = v.x; _12 = v.y; _13 = v.z; }

    FVector3 Left() const noexcept { return FVector3(-_11, -_12, -_13); }
    void Left(const FVector3& v) noexcept { _11 = -v.x; _12 = -v.y; _13 = -v.z; }

    FVector3 Forward() const noexcept { return FVector3(-_31, -_32, -_33); }
    void Forward(const FVector3& v) noexcept { _31 = -v.x; _32 = -v.y; _33 = -v.z; }

    FVector3 Backward() const noexcept { return FVector3(_31, _32, _33); }
    void Backward(const FVector3& v) noexcept { _31 = v.x; _32 = v.y; _33 = v.z; }

    FVector3 Translation() const  noexcept { return FVector3(_41, _42, _43); }
    void Translation(const FVector3& v) noexcept { _41 = v.x; _42 = v.y; _43 = v.z; }

    // FMatrix operations
    bool Decompose(FVector3& scale, FQuat& rotation, FVector3& translation) noexcept;

    FMatrix Transpose() const noexcept;
    void Transpose(FMatrix& result) const noexcept;

    FMatrix Invert() const noexcept;
    void Invert(FMatrix& result) const noexcept;

    float Determinant() const noexcept;

    // Static functions
    static FMatrix CreateBillboard(
        const FVector3& object, const FVector3& cameraPosition, const FVector3& cameraUp, _In_opt_ const FVector3* cameraForward = nullptr) noexcept;

    static FMatrix CreateConstrainedBillboard(
        const FVector3& object, const FVector3& cameraPosition, const FVector3& rotateAxis,
        _In_opt_ const FVector3* cameraForward = nullptr, _In_opt_ const FVector3* objectForward = nullptr) noexcept;

    static FMatrix CreateTranslation(const FVector3& position) noexcept;
    static FMatrix CreateTranslation(float x, float y, float z) noexcept;

    static FMatrix CreateScale(const FVector3& scales) noexcept;
    static FMatrix CreateScale(float xs, float ys, float zs) noexcept;
    static FMatrix CreateScale(float scale) noexcept;

    static FMatrix CreateRotationX(float radians) noexcept;
    static FMatrix CreateRotationY(float radians) noexcept;
    static FMatrix CreateRotationZ(float radians) noexcept;

    static FMatrix CreateFromAxisAngle(const FVector3& axis, float angle) noexcept;

    static FMatrix CreatePerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;
    static FMatrix CreatePerspective(float width, float height, float nearPlane, float farPlane) noexcept;
    static FMatrix CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane) noexcept;
    static FMatrix CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane) noexcept;
    static FMatrix CreateOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane) noexcept;

    static FMatrix CreateLookAt(const FVector3& position, const FVector3& target, const FVector3& up) noexcept;
    static FMatrix CreateWorld(const FVector3& position, const FVector3& forward, const FVector3& up) noexcept;

    static FMatrix CreateFromQuaternion(const FQuat& quat) noexcept;

    static FMatrix CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept;

    static FMatrix CreateShadow(const FVector3& lightDir, const Plane& plane) noexcept;

    static FMatrix CreateReflection(const Plane& plane) noexcept;

    static void Lerp(const FMatrix& M1, const FMatrix& M2, float t, FMatrix& result) noexcept;
    static FMatrix Lerp(const FMatrix& M1, const FMatrix& M2, float t) noexcept;

    static void FTransform(const FMatrix& M, const FQuat& rotation, FMatrix& result) noexcept;
    static FMatrix FTransform(const FMatrix& M, const FQuat& rotation) noexcept;

    // Constants
    static const FMatrix Identity;
};

// Binary operators
FMatrix operator+ (const FMatrix& M1, const FMatrix& M2) noexcept;
FMatrix operator- (const FMatrix& M1, const FMatrix& M2) noexcept;
FMatrix operator* (const FMatrix& M1, const FMatrix& M2) noexcept;
FMatrix operator* (const FMatrix& M, float S) noexcept;
FMatrix operator/ (const FMatrix& M, float S) noexcept;
FMatrix operator/ (const FMatrix& M1, const FMatrix& M2) noexcept;
// Element-wise divide
FMatrix operator* (float S, const FMatrix& M) noexcept;


//------------------------------------------------------------------------------
// FQuat
struct MATH_API FQuat : public XMFLOAT4
{
    FQuat() noexcept : XMFLOAT4(0, 0, 0, 1.f) {}
    constexpr FQuat(float ix, float iy, float iz, float iw) noexcept : XMFLOAT4(ix, iy, iz, iw) {}
    FQuat(const FVector3& v, float scalar) noexcept : XMFLOAT4(v.x, v.y, v.z, scalar) {}
    explicit FQuat(const FVector4& v) noexcept : XMFLOAT4(v.x, v.y, v.z, v.w) {}
    explicit FQuat(_In_reads_(4) const float* pArray) noexcept : XMFLOAT4(pArray) {}
    FQuat(FXMVECTOR V) noexcept { XMStoreFloat4(this, V); }
    FQuat(const XMFLOAT4& q) noexcept { this->x = q.x; this->y = q.y; this->z = q.z; this->w = q.w; }
    explicit FQuat(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

    FQuat(const FQuat&) = default;
    FQuat& operator=(const FQuat&) = default;

    FQuat(FQuat&&) = default;
    FQuat& operator=(FQuat&&) = default;

    operator XMVECTOR() const noexcept { return XMLoadFloat4(this); }

    // Comparison operators
    bool operator == (const FQuat& q) const noexcept;
    bool operator != (const FQuat& q) const noexcept;

    // Assignment operators
    FQuat& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
    FQuat& operator+= (const FQuat& q) noexcept;
    FQuat& operator-= (const FQuat& q) noexcept;
    FQuat& operator*= (const FQuat& q) noexcept;
    FQuat& operator*= (float S) noexcept;
    FQuat& operator/= (const FQuat& q) noexcept;

    // Unary operators
    FQuat operator+ () const  noexcept { return *this; }
    FQuat operator- () const noexcept;

    String ToString() const;

    // FQuat operations
    float Length() const noexcept;
    float LengthSquared() const noexcept;

    void Normalize() noexcept;
    void Normalize(FQuat& result) const noexcept;

    void Conjugate() noexcept;
    void Conjugate(FQuat& result) const noexcept;

    void Inverse(FQuat& result) const noexcept;

    float Dot(const FQuat& Q) const noexcept;

    FVector3 ToEulerAngles() const noexcept;

    // Static functions
    static FQuat CreateFromAxisAngle(const FVector3& axis, float angle) noexcept;
    static FQuat CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept;
    static FQuat CreateFromRotationMatrix(const FMatrix& M) noexcept;

    static void Lerp(const FQuat& q1, const FQuat& q2, float t, FQuat& result) noexcept;
    static FQuat Lerp(const FQuat& q1, const FQuat& q2, float t) noexcept;

    static void Slerp(const FQuat& q1, const FQuat& q2, float t, FQuat& result) noexcept;
    static FQuat Slerp(const FQuat& q1, const FQuat& q2, float t) noexcept;

    static void Concatenate(const FQuat& q1, const FQuat& q2, FQuat& result) noexcept;
    static FQuat Concatenate(const FQuat& q1, const FQuat& q2) noexcept;

    static FQuat Inverse( const FQuat& Quat );

    // Constants
    static const FQuat Identity;
};

// Binary operators
FQuat operator+ (const FQuat& Q1, const FQuat& Q2) noexcept;
FQuat operator- (const FQuat& Q1, const FQuat& Q2) noexcept;
FQuat operator* (const FQuat& Q1, const FQuat& Q2) noexcept;
FQuat operator* (const FQuat& Q, float S) noexcept;
FQuat operator/ (const FQuat& Q1, const FQuat& Q2) noexcept;
FQuat operator* (float S, const FQuat& Q) noexcept;


//-----------------------------------------------------------------------------
// Plane
struct Plane : public XMFLOAT4
{
    Plane() noexcept : XMFLOAT4(0.f, 1.f, 0.f, 0.f) {}
    constexpr Plane(float ix, float iy, float iz, float iw) noexcept : XMFLOAT4(ix, iy, iz, iw) {}
    Plane(const FVector3& normal, float d) noexcept : XMFLOAT4(normal.x, normal.y, normal.z, d) {}
    Plane(const FVector3& point1, const FVector3& point2, const FVector3& point3) noexcept;
    Plane(const FVector3& point, const FVector3& normal) noexcept;
    explicit Plane(const FVector4& v) noexcept : XMFLOAT4(v.x, v.y, v.z, v.w) {}
    explicit Plane(_In_reads_(4) const float* pArray) noexcept : XMFLOAT4(pArray) {}
    Plane(FXMVECTOR V) noexcept { XMStoreFloat4(this, V); }
    Plane(const XMFLOAT4& p) noexcept { this->x = p.x; this->y = p.y; this->z = p.z; this->w = p.w; }
    explicit Plane(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

    Plane(const Plane&) = default;
    Plane& operator=(const Plane&) = default;

    Plane(Plane&&) = default;
    Plane& operator=(Plane&&) = default;

    operator XMVECTOR() const noexcept { return XMLoadFloat4(this); }

    // Comparison operators
    bool operator == (const Plane& p) const noexcept;
    bool operator != (const Plane& p) const noexcept;

    // Assignment operators
    Plane& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }

    // Properties
    FVector3 Normal() const noexcept { return FVector3(x, y, z); }
    void Normal(const FVector3& normal) noexcept { x = normal.x; y = normal.y; z = normal.z; }

    float D() const noexcept { return w; }
    void D(float d) noexcept { w = d; }

    // Plane operations
    void Normalize() noexcept;
    void Normalize(Plane& result) const noexcept;

    float Dot(const FVector4& v) const noexcept;
    float DotCoordinate(const FVector3& position) const noexcept;
    float DotNormal(const FVector3& normal) const noexcept;

    // Static functions
    static void FTransform(const Plane& plane, const FMatrix& M, Plane& result) noexcept;
    static Plane FTransform(const Plane& plane, const FMatrix& M) noexcept;

    static void FTransform(const Plane& plane, const FQuat& rotation, Plane& result) noexcept;
    static Plane FTransform(const Plane& plane, const FQuat& rotation) noexcept;
    // Input quaternion must be the inverse transpose of the transformation
};

#include "MathCore.inl"

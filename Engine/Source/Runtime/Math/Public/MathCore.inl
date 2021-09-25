
// =======================================================================
// FVector2 Implementation
// =======================================================================


//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

inline bool FVector2::operator == (const FVector2& V) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(this);
    XMVECTOR v2 = XMLoadFloat2(&V);
    return XMVector2Equal(v1, v2);
}

inline bool FVector2::operator != (const FVector2& V) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(this);
    XMVECTOR v2 = XMLoadFloat2(&V);
    return XMVector2NotEqual(v1, v2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

inline FVector2& FVector2::operator+= (const FVector2& V) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(this);
    XMVECTOR v2 = XMLoadFloat2(&V);
    XMVECTOR X = XMVectorAdd(v1, v2);
    XMStoreFloat2(this, X);
    return *this;
}

inline FVector2& FVector2::operator-= (const FVector2& V) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(this);
    XMVECTOR v2 = XMLoadFloat2(&V);
    XMVECTOR X = XMVectorSubtract(v1, v2);
    XMStoreFloat2(this, X);
    return *this;
}

inline FVector2& FVector2::operator*= (const FVector2& V) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(this);
    XMVECTOR v2 = XMLoadFloat2(&V);
    XMVECTOR X = XMVectorMultiply(v1, v2);
    XMStoreFloat2(this, X);
    return *this;
}

inline FVector2& FVector2::operator*= (float S) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(this);
    XMVECTOR X = XMVectorScale(v1, S);
    XMStoreFloat2(this, X);
    return *this;
}

inline FVector2& FVector2::operator/= (float S) noexcept
{
    using namespace DirectX;
    assert(S != 0.0f);
    XMVECTOR v1 = XMLoadFloat2(this);
    XMVECTOR X = XMVectorScale(v1, 1.f / S);
    XMStoreFloat2(this, X);
    return *this;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

inline FVector2 operator+ (const FVector2& V1, const FVector2& V2) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(&V1);
    XMVECTOR v2 = XMLoadFloat2(&V2);
    XMVECTOR X = XMVectorAdd(v1, v2);
    FVector2 R;
    XMStoreFloat2(&R, X);
    return R;
}

inline FVector2 operator- (const FVector2& V1, const FVector2& V2) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(&V1);
    XMVECTOR v2 = XMLoadFloat2(&V2);
    XMVECTOR X = XMVectorSubtract(v1, v2);
    FVector2 R;
    XMStoreFloat2(&R, X);
    return R;
}

inline FVector2 operator* (const FVector2& V1, const FVector2& V2) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(&V1);
    XMVECTOR v2 = XMLoadFloat2(&V2);
    XMVECTOR X = XMVectorMultiply(v1, v2);
    FVector2 R;
    XMStoreFloat2(&R, X);
    return R;
}

inline FVector2 operator* (const FVector2& V, float S) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(&V);
    XMVECTOR X = XMVectorScale(v1, S);
    FVector2 R;
    XMStoreFloat2(&R, X);
    return R;
}

inline FVector2 operator/ (const FVector2& V1, const FVector2& V2) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(&V1);
    XMVECTOR v2 = XMLoadFloat2(&V2);
    XMVECTOR X = XMVectorDivide(v1, v2);
    FVector2 R;
    XMStoreFloat2(&R, X);
    return R;
}

inline FVector2 operator/ (const FVector2& V, float S) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(&V);
    XMVECTOR X = XMVectorScale(v1, 1.f / S);
    FVector2 R;
    XMStoreFloat2(&R, X);
    return R;
}

inline FVector2 operator* (float S, const FVector2& V) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(&V);
    XMVECTOR X = XMVectorScale(v1, S);
    FVector2 R;
    XMStoreFloat2(&R, X);
    return R;
}

//------------------------------------------------------------------------------
// Utility
//------------------------------------------------------------------------------

inline String FVector2::ToString() const
{
    std::stringstream Stream;
    Stream << "(" << x << ", " << y << ")";
    return Stream.str();
}

//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

inline bool FVector2::InBounds(const FVector2& Bounds) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(this);
    XMVECTOR v2 = XMLoadFloat2(&Bounds);
    return XMVector2InBounds(v1, v2);
}

inline float FVector2::Length() const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(this);
    XMVECTOR X = XMVector2Length(v1);
    return XMVectorGetX(X);
}

inline float FVector2::LengthSquared() const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(this);
    XMVECTOR X = XMVector2LengthSq(v1);
    return XMVectorGetX(X);
}

inline float FVector2::Dot(const FVector2& V) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(this);
    XMVECTOR v2 = XMLoadFloat2(&V);
    XMVECTOR X = XMVector2Dot(v1, v2);
    return XMVectorGetX(X);
}

inline void FVector2::Cross(const FVector2& V, FVector2& result) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(this);
    XMVECTOR v2 = XMLoadFloat2(&V);
    XMVECTOR R = XMVector2Cross(v1, v2);
    XMStoreFloat2(&result, R);
}

inline FVector2 FVector2::Cross(const FVector2& V) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(this);
    XMVECTOR v2 = XMLoadFloat2(&V);
    XMVECTOR R = XMVector2Cross(v1, v2);

    FVector2 result;
    XMStoreFloat2(&result, R);
    return result;
}

inline void FVector2::Normalize() noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(this);
    XMVECTOR X = XMVector2Normalize(v1);
    XMStoreFloat2(this, X);
}

inline void FVector2::Normalize(FVector2& result) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(this);
    XMVECTOR X = XMVector2Normalize(v1);
    XMStoreFloat2(&result, X);
}

inline void FVector2::Clamp(const FVector2& vmin, const FVector2& vmax) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(this);
    XMVECTOR v2 = XMLoadFloat2(&vmin);
    XMVECTOR v3 = XMLoadFloat2(&vmax);
    XMVECTOR X = XMVectorClamp(v1, v2, v3);
    XMStoreFloat2(this, X);
}

inline void FVector2::Clamp(const FVector2& vmin, const FVector2& vmax, FVector2& result) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(this);
    XMVECTOR v2 = XMLoadFloat2(&vmin);
    XMVECTOR v3 = XMLoadFloat2(&vmax);
    XMVECTOR X = XMVectorClamp(v1, v2, v3);
    XMStoreFloat2(&result, X);
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

inline float FVector2::Distance(const FVector2& v1, const FVector2& v2) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat2(&v1);
    XMVECTOR x2 = XMLoadFloat2(&v2);
    XMVECTOR V = XMVectorSubtract(x2, x1);
    XMVECTOR X = XMVector2Length(V);
    return XMVectorGetX(X);
}

inline float FVector2::DistanceSquared(const FVector2& v1, const FVector2& v2) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat2(&v1);
    XMVECTOR x2 = XMLoadFloat2(&v2);
    XMVECTOR V = XMVectorSubtract(x2, x1);
    XMVECTOR X = XMVector2LengthSq(V);
    return XMVectorGetX(X);
}

inline void FVector2::Min(const FVector2& v1, const FVector2& v2, FVector2& result) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat2(&v1);
    XMVECTOR x2 = XMLoadFloat2(&v2);
    XMVECTOR X = XMVectorMin(x1, x2);
    XMStoreFloat2(&result, X);
}

inline FVector2 FVector2::Min(const FVector2& v1, const FVector2& v2) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat2(&v1);
    XMVECTOR x2 = XMLoadFloat2(&v2);
    XMVECTOR X = XMVectorMin(x1, x2);

    FVector2 result;
    XMStoreFloat2(&result, X);
    return result;
}

inline void FVector2::Max(const FVector2& v1, const FVector2& v2, FVector2& result) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat2(&v1);
    XMVECTOR x2 = XMLoadFloat2(&v2);
    XMVECTOR X = XMVectorMax(x1, x2);
    XMStoreFloat2(&result, X);
}

inline FVector2 FVector2::Max(const FVector2& v1, const FVector2& v2) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat2(&v1);
    XMVECTOR x2 = XMLoadFloat2(&v2);
    XMVECTOR X = XMVectorMax(x1, x2);

    FVector2 result;
    XMStoreFloat2(&result, X);
    return result;
}

inline void FVector2::Lerp(const FVector2& v1, const FVector2& v2, float t, FVector2& result) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat2(&v1);
    XMVECTOR x2 = XMLoadFloat2(&v2);
    XMVECTOR X = XMVectorLerp(x1, x2, t);
    XMStoreFloat2(&result, X);
}

inline FVector2 FVector2::Lerp(const FVector2& v1, const FVector2& v2, float t) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat2(&v1);
    XMVECTOR x2 = XMLoadFloat2(&v2);
    XMVECTOR X = XMVectorLerp(x1, x2, t);

    FVector2 result;
    XMStoreFloat2(&result, X);
    return result;
}

inline void FVector2::SmoothStep(const FVector2& v1, const FVector2& v2, float t, FVector2& result) noexcept
{
    using namespace DirectX;
    t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
    t = t * t * (3.f - 2.f * t);
    XMVECTOR x1 = XMLoadFloat2(&v1);
    XMVECTOR x2 = XMLoadFloat2(&v2);
    XMVECTOR X = XMVectorLerp(x1, x2, t);
    XMStoreFloat2(&result, X);
}

inline FVector2 FVector2::SmoothStep(const FVector2& v1, const FVector2& v2, float t) noexcept
{
    using namespace DirectX;
    t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
    t = t * t * (3.f - 2.f * t);
    XMVECTOR x1 = XMLoadFloat2(&v1);
    XMVECTOR x2 = XMLoadFloat2(&v2);
    XMVECTOR X = XMVectorLerp(x1, x2, t);

    FVector2 result;
    XMStoreFloat2(&result, X);
    return result;
}

inline void FVector2::Barycentric(const FVector2& v1, const FVector2& v2, const FVector2& v3, float f, float g, FVector2& result) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat2(&v1);
    XMVECTOR x2 = XMLoadFloat2(&v2);
    XMVECTOR x3 = XMLoadFloat2(&v3);
    XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);
    XMStoreFloat2(&result, X);
}

inline FVector2 FVector2::Barycentric(const FVector2& v1, const FVector2& v2, const FVector2& v3, float f, float g) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat2(&v1);
    XMVECTOR x2 = XMLoadFloat2(&v2);
    XMVECTOR x3 = XMLoadFloat2(&v3);
    XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);

    FVector2 result;
    XMStoreFloat2(&result, X);
    return result;
}

inline void FVector2::CatmullRom(const FVector2& v1, const FVector2& v2, const FVector2& v3, const FVector2& v4, float t, FVector2& result) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat2(&v1);
    XMVECTOR x2 = XMLoadFloat2(&v2);
    XMVECTOR x3 = XMLoadFloat2(&v3);
    XMVECTOR x4 = XMLoadFloat2(&v4);
    XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);
    XMStoreFloat2(&result, X);
}

inline FVector2 FVector2::CatmullRom(const FVector2& v1, const FVector2& v2, const FVector2& v3, const FVector2& v4, float t) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat2(&v1);
    XMVECTOR x2 = XMLoadFloat2(&v2);
    XMVECTOR x3 = XMLoadFloat2(&v3);
    XMVECTOR x4 = XMLoadFloat2(&v4);
    XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);

    FVector2 result;
    XMStoreFloat2(&result, X);
    return result;
}

inline void FVector2::Hermite(const FVector2& v1, const FVector2& t1, const FVector2& v2, const FVector2& t2, float t, FVector2& result) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat2(&v1);
    XMVECTOR x2 = XMLoadFloat2(&t1);
    XMVECTOR x3 = XMLoadFloat2(&v2);
    XMVECTOR x4 = XMLoadFloat2(&t2);
    XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);
    XMStoreFloat2(&result, X);
}

inline FVector2 FVector2::Hermite(const FVector2& v1, const FVector2& t1, const FVector2& v2, const FVector2& t2, float t) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat2(&v1);
    XMVECTOR x2 = XMLoadFloat2(&t1);
    XMVECTOR x3 = XMLoadFloat2(&v2);
    XMVECTOR x4 = XMLoadFloat2(&t2);
    XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);

    FVector2 result;
    XMStoreFloat2(&result, X);
    return result;
}

inline void FVector2::Reflect(const FVector2& ivec, const FVector2& nvec, FVector2& result) noexcept
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat2(&ivec);
    XMVECTOR n = XMLoadFloat2(&nvec);
    XMVECTOR X = XMVector2Reflect(i, n);
    XMStoreFloat2(&result, X);
}

inline FVector2 FVector2::Reflect(const FVector2& ivec, const FVector2& nvec) noexcept
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat2(&ivec);
    XMVECTOR n = XMLoadFloat2(&nvec);
    XMVECTOR X = XMVector2Reflect(i, n);

    FVector2 result;
    XMStoreFloat2(&result, X);
    return result;
}

inline void FVector2::Refract(const FVector2& ivec, const FVector2& nvec, float refractionIndex, FVector2& result) noexcept
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat2(&ivec);
    XMVECTOR n = XMLoadFloat2(&nvec);
    XMVECTOR X = XMVector2Refract(i, n, refractionIndex);
    XMStoreFloat2(&result, X);
}

inline FVector2 FVector2::Refract(const FVector2& ivec, const FVector2& nvec, float refractionIndex) noexcept
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat2(&ivec);
    XMVECTOR n = XMLoadFloat2(&nvec);
    XMVECTOR X = XMVector2Refract(i, n, refractionIndex);

    FVector2 result;
    XMStoreFloat2(&result, X);
    return result;
}

inline void FVector2::FTransform(const FVector2& v, const FQuat& quat, FVector2& result) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(&v);
    XMVECTOR q = XMLoadFloat4(&quat);
    XMVECTOR X = XMVector3Rotate(v1, q);
    XMStoreFloat2(&result, X);
}

inline FVector2 FVector2::FTransform(const FVector2& v, const FQuat& quat) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(&v);
    XMVECTOR q = XMLoadFloat4(&quat);
    XMVECTOR X = XMVector3Rotate(v1, q);

    FVector2 result;
    XMStoreFloat2(&result, X);
    return result;
}

inline void FVector2::FTransform(const FVector2& v, const FMatrix& m, FVector2& result) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(&v);
    XMMATRIX M = XMLoadFloat4x4(&m);
    XMVECTOR X = XMVector2TransformCoord(v1, M);
    XMStoreFloat2(&result, X);
}

inline FVector2 FVector2::FTransform(const FVector2& v, const FMatrix& m) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(&v);
    XMMATRIX M = XMLoadFloat4x4(&m);
    XMVECTOR X = XMVector2TransformCoord(v1, M);

    FVector2 result;
    XMStoreFloat2(&result, X);
    return result;
}

_Use_decl_annotations_
inline void FVector2::FTransform(const FVector2* varray, size_t count, const FMatrix& m, FVector2* resultArray) noexcept
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4(&m);
    XMVector2TransformCoordStream(resultArray, sizeof(XMFLOAT2), varray, sizeof(XMFLOAT2), count, M);
}

inline void FVector2::FTransform(const FVector2& v, const FMatrix& m, FVector4& result) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(&v);
    XMMATRIX M = XMLoadFloat4x4(&m);
    XMVECTOR X = XMVector2Transform(v1, M);
    XMStoreFloat4(&result, X);
}

_Use_decl_annotations_
inline void FVector2::FTransform(const FVector2* varray, size_t count, const FMatrix& m, FVector4* resultArray) noexcept
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4(&m);
    XMVector2TransformStream(resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT2), count, M);
}

inline void FVector2::TransformNormal(const FVector2& v, const FMatrix& m, FVector2& result) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(&v);
    XMMATRIX M = XMLoadFloat4x4(&m);
    XMVECTOR X = XMVector2TransformNormal(v1, M);
    XMStoreFloat2(&result, X);
}

inline FVector2 FVector2::TransformNormal(const FVector2& v, const FMatrix& m) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(&v);
    XMMATRIX M = XMLoadFloat4x4(&m);
    XMVECTOR X = XMVector2TransformNormal(v1, M);

    FVector2 result;
    XMStoreFloat2(&result, X);
    return result;
}

_Use_decl_annotations_
inline void FVector2::TransformNormal(const FVector2* varray, size_t count, const FMatrix& m, FVector2* resultArray) noexcept
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4(&m);
    XMVector2TransformNormalStream(resultArray, sizeof(XMFLOAT2), varray, sizeof(XMFLOAT2), count, M);
}



// =======================================================================
// FVector3 Implementation
// =======================================================================


//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

inline bool FVector3::operator == (const FVector3& V) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(this);
    XMVECTOR v2 = XMLoadFloat3(&V);
    return XMVector3Equal(v1, v2);
}

inline bool FVector3::operator != (const FVector3& V) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(this);
    XMVECTOR v2 = XMLoadFloat3(&V);
    return XMVector3NotEqual(v1, v2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

inline FVector3& FVector3::operator+= (const FVector3& V) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(this);
    XMVECTOR v2 = XMLoadFloat3(&V);
    XMVECTOR X = XMVectorAdd(v1, v2);
    XMStoreFloat3(this, X);
    return *this;
}

inline FVector3& FVector3::operator-= (const FVector3& V) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(this);
    XMVECTOR v2 = XMLoadFloat3(&V);
    XMVECTOR X = XMVectorSubtract(v1, v2);
    XMStoreFloat3(this, X);
    return *this;
}

inline FVector3& FVector3::operator*= (const FVector3& V) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(this);
    XMVECTOR v2 = XMLoadFloat3(&V);
    XMVECTOR X = XMVectorMultiply(v1, v2);
    XMStoreFloat3(this, X);
    return *this;
}

inline FVector3& FVector3::operator*= (float S) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(this);
    XMVECTOR X = XMVectorScale(v1, S);
    XMStoreFloat3(this, X);
    return *this;
}

inline FVector3& FVector3::operator/= (float S) noexcept
{
    using namespace DirectX;
    assert(S != 0.0f);
    XMVECTOR v1 = XMLoadFloat3(this);
    XMVECTOR X = XMVectorScale(v1, 1.f / S);
    XMStoreFloat3(this, X);
    return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

inline FVector3 FVector3::operator- () const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(this);
    XMVECTOR X = XMVectorNegate(v1);
    FVector3 R;
    XMStoreFloat3(&R, X);
    return R;
}


//------------------------------------------------------------------------------
// Utility
//------------------------------------------------------------------------------

inline String FVector3::ToString() const
{
    std::stringstream Stream;
    Stream << "(" << x << ", " << y << ", " << z << ")";
    return Stream.str();
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

inline FVector3 operator+ (const FVector3& V1, const FVector3& V2) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(&V1);
    XMVECTOR v2 = XMLoadFloat3(&V2);
    XMVECTOR X = XMVectorAdd(v1, v2);
    FVector3 R;
    XMStoreFloat3(&R, X);
    return R;
}

inline FVector3 operator- (const FVector3& V1, const FVector3& V2) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(&V1);
    XMVECTOR v2 = XMLoadFloat3(&V2);
    XMVECTOR X = XMVectorSubtract(v1, v2);
    FVector3 R;
    XMStoreFloat3(&R, X);
    return R;
}

inline FVector3 operator* (const FVector3& V1, const FVector3& V2) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(&V1);
    XMVECTOR v2 = XMLoadFloat3(&V2);
    XMVECTOR X = XMVectorMultiply(v1, v2);
    FVector3 R;
    XMStoreFloat3(&R, X);
    return R;
}

inline FVector3 operator* (const FVector3& V, float S) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(&V);
    XMVECTOR X = XMVectorScale(v1, S);
    FVector3 R;
    XMStoreFloat3(&R, X);
    return R;
}

inline FVector3 operator/ (const FVector3& V1, const FVector3& V2) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(&V1);
    XMVECTOR v2 = XMLoadFloat3(&V2);
    XMVECTOR X = XMVectorDivide(v1, v2);
    FVector3 R;
    XMStoreFloat3(&R, X);
    return R;
}

inline FVector3 operator/ (const FVector3& V, float S) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(&V);
    XMVECTOR X = XMVectorScale(v1, 1.f / S);
    FVector3 R;
    XMStoreFloat3(&R, X);
    return R;
}

inline FVector3 operator* (float S, const FVector3& V) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(&V);
    XMVECTOR X = XMVectorScale(v1, S);
    FVector3 R;
    XMStoreFloat3(&R, X);
    return R;
}

//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

inline bool FVector3::InBounds(const FVector3& Bounds) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(this);
    XMVECTOR v2 = XMLoadFloat3(&Bounds);
    return XMVector3InBounds(v1, v2);
}

inline float FVector3::Length() const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(this);
    XMVECTOR X = XMVector3Length(v1);
    return XMVectorGetX(X);
}

inline float FVector3::LengthSquared() const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(this);
    XMVECTOR X = XMVector3LengthSq(v1);
    return XMVectorGetX(X);
}

inline float FVector3::Dot(const FVector3& V) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(this);
    XMVECTOR v2 = XMLoadFloat3(&V);
    XMVECTOR X = XMVector3Dot(v1, v2);
    return XMVectorGetX(X);
}

inline void FVector3::Cross(const FVector3& V, FVector3& result) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(this);
    XMVECTOR v2 = XMLoadFloat3(&V);
    XMVECTOR R = XMVector3Cross(v1, v2);
    XMStoreFloat3(&result, R);
}

inline FVector3 FVector3::Cross(const FVector3& V) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(this);
    XMVECTOR v2 = XMLoadFloat3(&V);
    XMVECTOR R = XMVector3Cross(v1, v2);

    FVector3 result;
    XMStoreFloat3(&result, R);
    return result;
}

inline void FVector3::Normalize() noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(this);
    XMVECTOR X = XMVector3Normalize(v1);
    XMStoreFloat3(this, X);
}

inline void FVector3::Normalize(FVector3& result) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(this);
    XMVECTOR X = XMVector3Normalize(v1);
    XMStoreFloat3(&result, X);
}

inline void FVector3::Clamp(const FVector3& vmin, const FVector3& vmax) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(this);
    XMVECTOR v2 = XMLoadFloat3(&vmin);
    XMVECTOR v3 = XMLoadFloat3(&vmax);
    XMVECTOR X = XMVectorClamp(v1, v2, v3);
    XMStoreFloat3(this, X);
}

inline void FVector3::Clamp(const FVector3& vmin, const FVector3& vmax, FVector3& result) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(this);
    XMVECTOR v2 = XMLoadFloat3(&vmin);
    XMVECTOR v3 = XMLoadFloat3(&vmax);
    XMVECTOR X = XMVectorClamp(v1, v2, v3);
    XMStoreFloat3(&result, X);
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

inline float FVector3::Distance(const FVector3& v1, const FVector3& v2) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat3(&v1);
    XMVECTOR x2 = XMLoadFloat3(&v2);
    XMVECTOR V = XMVectorSubtract(x2, x1);
    XMVECTOR X = XMVector3Length(V);
    return XMVectorGetX(X);
}

inline float FVector3::DistanceSquared(const FVector3& v1, const FVector3& v2) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat3(&v1);
    XMVECTOR x2 = XMLoadFloat3(&v2);
    XMVECTOR V = XMVectorSubtract(x2, x1);
    XMVECTOR X = XMVector3LengthSq(V);
    return XMVectorGetX(X);
}

inline void FVector3::Min(const FVector3& v1, const FVector3& v2, FVector3& result) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat3(&v1);
    XMVECTOR x2 = XMLoadFloat3(&v2);
    XMVECTOR X = XMVectorMin(x1, x2);
    XMStoreFloat3(&result, X);
}

inline FVector3 FVector3::Min(const FVector3& v1, const FVector3& v2) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat3(&v1);
    XMVECTOR x2 = XMLoadFloat3(&v2);
    XMVECTOR X = XMVectorMin(x1, x2);

    FVector3 result;
    XMStoreFloat3(&result, X);
    return result;
}

inline void FVector3::Max(const FVector3& v1, const FVector3& v2, FVector3& result) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat3(&v1);
    XMVECTOR x2 = XMLoadFloat3(&v2);
    XMVECTOR X = XMVectorMax(x1, x2);
    XMStoreFloat3(&result, X);
}

inline FVector3 FVector3::Max(const FVector3& v1, const FVector3& v2) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat3(&v1);
    XMVECTOR x2 = XMLoadFloat3(&v2);
    XMVECTOR X = XMVectorMax(x1, x2);

    FVector3 result;
    XMStoreFloat3(&result, X);
    return result;
}

inline void FVector3::Lerp(const FVector3& v1, const FVector3& v2, float t, FVector3& result) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat3(&v1);
    XMVECTOR x2 = XMLoadFloat3(&v2);
    XMVECTOR X = XMVectorLerp(x1, x2, t);
    XMStoreFloat3(&result, X);
}

inline FVector3 FVector3::Lerp(const FVector3& v1, const FVector3& v2, float t) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat3(&v1);
    XMVECTOR x2 = XMLoadFloat3(&v2);
    XMVECTOR X = XMVectorLerp(x1, x2, t);

    FVector3 result;
    XMStoreFloat3(&result, X);
    return result;
}

inline void FVector3::SmoothStep(const FVector3& v1, const FVector3& v2, float t, FVector3& result) noexcept
{
    using namespace DirectX;
    t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
    t = t * t * (3.f - 2.f * t);
    XMVECTOR x1 = XMLoadFloat3(&v1);
    XMVECTOR x2 = XMLoadFloat3(&v2);
    XMVECTOR X = XMVectorLerp(x1, x2, t);
    XMStoreFloat3(&result, X);
}

inline FVector3 FVector3::SmoothStep(const FVector3& v1, const FVector3& v2, float t) noexcept
{
    using namespace DirectX;
    t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
    t = t * t * (3.f - 2.f * t);
    XMVECTOR x1 = XMLoadFloat3(&v1);
    XMVECTOR x2 = XMLoadFloat3(&v2);
    XMVECTOR X = XMVectorLerp(x1, x2, t);

    FVector3 result;
    XMStoreFloat3(&result, X);
    return result;
}

inline void FVector3::Barycentric(const FVector3& v1, const FVector3& v2, const FVector3& v3, float f, float g, FVector3& result) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat3(&v1);
    XMVECTOR x2 = XMLoadFloat3(&v2);
    XMVECTOR x3 = XMLoadFloat3(&v3);
    XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);
    XMStoreFloat3(&result, X);
}

inline FVector3 FVector3::Barycentric(const FVector3& v1, const FVector3& v2, const FVector3& v3, float f, float g) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat3(&v1);
    XMVECTOR x2 = XMLoadFloat3(&v2);
    XMVECTOR x3 = XMLoadFloat3(&v3);
    XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);

    FVector3 result;
    XMStoreFloat3(&result, X);
    return result;
}

inline void FVector3::CatmullRom(const FVector3& v1, const FVector3& v2, const FVector3& v3, const FVector3& v4, float t, FVector3& result) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat3(&v1);
    XMVECTOR x2 = XMLoadFloat3(&v2);
    XMVECTOR x3 = XMLoadFloat3(&v3);
    XMVECTOR x4 = XMLoadFloat3(&v4);
    XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);
    XMStoreFloat3(&result, X);
}

inline FVector3 FVector3::CatmullRom(const FVector3& v1, const FVector3& v2, const FVector3& v3, const FVector3& v4, float t) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat3(&v1);
    XMVECTOR x2 = XMLoadFloat3(&v2);
    XMVECTOR x3 = XMLoadFloat3(&v3);
    XMVECTOR x4 = XMLoadFloat3(&v4);
    XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);

    FVector3 result;
    XMStoreFloat3(&result, X);
    return result;
}

inline void FVector3::Hermite(const FVector3& v1, const FVector3& t1, const FVector3& v2, const FVector3& t2, float t, FVector3& result) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat3(&v1);
    XMVECTOR x2 = XMLoadFloat3(&t1);
    XMVECTOR x3 = XMLoadFloat3(&v2);
    XMVECTOR x4 = XMLoadFloat3(&t2);
    XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);
    XMStoreFloat3(&result, X);
}

inline FVector3 FVector3::Hermite(const FVector3& v1, const FVector3& t1, const FVector3& v2, const FVector3& t2, float t) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat3(&v1);
    XMVECTOR x2 = XMLoadFloat3(&t1);
    XMVECTOR x3 = XMLoadFloat3(&v2);
    XMVECTOR x4 = XMLoadFloat3(&t2);
    XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);

    FVector3 result;
    XMStoreFloat3(&result, X);
    return result;
}

inline void FVector3::Reflect(const FVector3& ivec, const FVector3& nvec, FVector3& result) noexcept
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat3(&ivec);
    XMVECTOR n = XMLoadFloat3(&nvec);
    XMVECTOR X = XMVector3Reflect(i, n);
    XMStoreFloat3(&result, X);
}

inline FVector3 FVector3::Reflect(const FVector3& ivec, const FVector3& nvec) noexcept
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat3(&ivec);
    XMVECTOR n = XMLoadFloat3(&nvec);
    XMVECTOR X = XMVector3Reflect(i, n);

    FVector3 result;
    XMStoreFloat3(&result, X);
    return result;
}

inline void FVector3::Refract(const FVector3& ivec, const FVector3& nvec, float refractionIndex, FVector3& result) noexcept
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat3(&ivec);
    XMVECTOR n = XMLoadFloat3(&nvec);
    XMVECTOR X = XMVector3Refract(i, n, refractionIndex);
    XMStoreFloat3(&result, X);
}

inline FVector3 FVector3::Refract(const FVector3& ivec, const FVector3& nvec, float refractionIndex) noexcept
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat3(&ivec);
    XMVECTOR n = XMLoadFloat3(&nvec);
    XMVECTOR X = XMVector3Refract(i, n, refractionIndex);

    FVector3 result;
    XMStoreFloat3(&result, X);
    return result;
}

inline void FVector3::FTransform(const FVector3& v, const FQuat& quat, FVector3& result) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(&v);
    XMVECTOR q = XMLoadFloat4(&quat);
    XMVECTOR X = XMVector3Rotate(v1, q);
    XMStoreFloat3(&result, X);
}

inline FVector3 FVector3::FTransform(const FVector3& v, const FQuat& quat) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(&v);
    XMVECTOR q = XMLoadFloat4(&quat);
    XMVECTOR X = XMVector3Rotate(v1, q);

    FVector3 result;
    XMStoreFloat3(&result, X);
    return result;
}

inline void FVector3::FTransform(const FVector3& v, const FMatrix& m, FVector3& result) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(&v);
    XMMATRIX M = XMLoadFloat4x4(&m);
    XMVECTOR X = XMVector3TransformCoord(v1, M);
    XMStoreFloat3(&result, X);
}

inline FVector3 FVector3::FTransform(const FVector3& v, const FMatrix& m) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(&v);
    XMMATRIX M = XMLoadFloat4x4(&m);
    XMVECTOR X = XMVector3TransformCoord(v1, M);

    FVector3 result;
    XMStoreFloat3(&result, X);
    return result;
}

_Use_decl_annotations_
inline void FVector3::FTransform(const FVector3* varray, size_t count, const FMatrix& m, FVector3* resultArray) noexcept
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4(&m);
    XMVector3TransformCoordStream(resultArray, sizeof(XMFLOAT3), varray, sizeof(XMFLOAT3), count, M);
}

inline void FVector3::FTransform(const FVector3& v, const FMatrix& m, FVector4& result) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(&v);
    XMMATRIX M = XMLoadFloat4x4(&m);
    XMVECTOR X = XMVector3Transform(v1, M);
    XMStoreFloat4(&result, X);
}

_Use_decl_annotations_
inline void FVector3::FTransform(const FVector3* varray, size_t count, const FMatrix& m, FVector4* resultArray) noexcept
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4(&m);
    XMVector3TransformStream(resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT3), count, M);
}

inline void FVector3::TransformNormal(const FVector3& v, const FMatrix& m, FVector3& result) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(&v);
    XMMATRIX M = XMLoadFloat4x4(&m);
    XMVECTOR X = XMVector3TransformNormal(v1, M);
    XMStoreFloat3(&result, X);
}

inline FVector3 FVector3::TransformNormal(const FVector3& v, const FMatrix& m) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(&v);
    XMMATRIX M = XMLoadFloat4x4(&m);
    XMVECTOR X = XMVector3TransformNormal(v1, M);

    FVector3 result;
    XMStoreFloat3(&result, X);
    return result;
}

_Use_decl_annotations_
inline void FVector3::TransformNormal(const FVector3* varray, size_t count, const FMatrix& m, FVector3* resultArray) noexcept
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4(&m);
    XMVector3TransformNormalStream(resultArray, sizeof(XMFLOAT3), varray, sizeof(XMFLOAT3), count, M);
}


// =======================================================================
// FVector4 Implementation
// =======================================================================


//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

inline bool FVector4::operator == (const FVector4& V) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(this);
    XMVECTOR v2 = XMLoadFloat4(&V);
    return XMVector4Equal(v1, v2);
}

inline bool FVector4::operator != (const FVector4& V) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(this);
    XMVECTOR v2 = XMLoadFloat4(&V);
    return XMVector4NotEqual(v1, v2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

inline FVector4& FVector4::operator+= (const FVector4& V) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(this);
    XMVECTOR v2 = XMLoadFloat4(&V);
    XMVECTOR X = XMVectorAdd(v1, v2);
    XMStoreFloat4(this, X);
    return *this;
}

inline FVector4& FVector4::operator-= (const FVector4& V) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(this);
    XMVECTOR v2 = XMLoadFloat4(&V);
    XMVECTOR X = XMVectorSubtract(v1, v2);
    XMStoreFloat4(this, X);
    return *this;
}

inline FVector4& FVector4::operator*= (const FVector4& V) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(this);
    XMVECTOR v2 = XMLoadFloat4(&V);
    XMVECTOR X = XMVectorMultiply(v1, v2);
    XMStoreFloat4(this, X);
    return *this;
}

inline FVector4& FVector4::operator*= (float S) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(this);
    XMVECTOR X = XMVectorScale(v1, S);
    XMStoreFloat4(this, X);
    return *this;
}

inline FVector4& FVector4::operator/= (float S) noexcept
{
    using namespace DirectX;
    assert(S != 0.0f);
    XMVECTOR v1 = XMLoadFloat4(this);
    XMVECTOR X = XMVectorScale(v1, 1.f / S);
    XMStoreFloat4(this, X);
    return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

inline FVector4 FVector4::operator- () const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(this);
    XMVECTOR X = XMVectorNegate(v1);
    FVector4 R;
    XMStoreFloat4(&R, X);
    return R;
}

//------------------------------------------------------------------------------
// Utility
//------------------------------------------------------------------------------

inline String FVector4::ToString() const
{
    std::stringstream Stream;
    Stream << "(" << x << ", " << y << ", " << z << ", " << w << ")";
    return Stream.str();
}


//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

inline FVector4 operator+ (const FVector4& V1, const FVector4& V2) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(&V1);
    XMVECTOR v2 = XMLoadFloat4(&V2);
    XMVECTOR X = XMVectorAdd(v1, v2);
    FVector4 R;
    XMStoreFloat4(&R, X);
    return R;
}

inline FVector4 operator- (const FVector4& V1, const FVector4& V2) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(&V1);
    XMVECTOR v2 = XMLoadFloat4(&V2);
    XMVECTOR X = XMVectorSubtract(v1, v2);
    FVector4 R;
    XMStoreFloat4(&R, X);
    return R;
}

inline FVector4 operator* (const FVector4& V1, const FVector4& V2) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(&V1);
    XMVECTOR v2 = XMLoadFloat4(&V2);
    XMVECTOR X = XMVectorMultiply(v1, v2);
    FVector4 R;
    XMStoreFloat4(&R, X);
    return R;
}

inline FVector4 operator* (const FVector4& V, float S) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(&V);
    XMVECTOR X = XMVectorScale(v1, S);
    FVector4 R;
    XMStoreFloat4(&R, X);
    return R;
}

inline FVector4 operator/ (const FVector4& V1, const FVector4& V2) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(&V1);
    XMVECTOR v2 = XMLoadFloat4(&V2);
    XMVECTOR X = XMVectorDivide(v1, v2);
    FVector4 R;
    XMStoreFloat4(&R, X);
    return R;
}

inline FVector4 operator/ (const FVector4& V, float S) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(&V);
    XMVECTOR X = XMVectorScale(v1, 1.f / S);
    FVector4 R;
    XMStoreFloat4(&R, X);
    return R;
}

inline FVector4 operator* (float S, const FVector4& V) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(&V);
    XMVECTOR X = XMVectorScale(v1, S);
    FVector4 R;
    XMStoreFloat4(&R, X);
    return R;
}

//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

inline bool FVector4::InBounds(const FVector4& Bounds) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(this);
    XMVECTOR v2 = XMLoadFloat4(&Bounds);
    return XMVector4InBounds(v1, v2);
}

inline float FVector4::Length() const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(this);
    XMVECTOR X = XMVector4Length(v1);
    return XMVectorGetX(X);
}

inline float FVector4::LengthSquared() const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(this);
    XMVECTOR X = XMVector4LengthSq(v1);
    return XMVectorGetX(X);
}

inline float FVector4::Dot(const FVector4& V) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(this);
    XMVECTOR v2 = XMLoadFloat4(&V);
    XMVECTOR X = XMVector4Dot(v1, v2);
    return XMVectorGetX(X);
}

inline void FVector4::Cross(const FVector4& v1, const FVector4& v2, FVector4& result) const noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(this);
    XMVECTOR x2 = XMLoadFloat4(&v1);
    XMVECTOR x3 = XMLoadFloat4(&v2);
    XMVECTOR R = XMVector4Cross(x1, x2, x3);
    XMStoreFloat4(&result, R);
}

inline FVector4 FVector4::Cross(const FVector4& v1, const FVector4& v2) const noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(this);
    XMVECTOR x2 = XMLoadFloat4(&v1);
    XMVECTOR x3 = XMLoadFloat4(&v2);
    XMVECTOR R = XMVector4Cross(x1, x2, x3);

    FVector4 result;
    XMStoreFloat4(&result, R);
    return result;
}

inline void FVector4::Normalize() noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(this);
    XMVECTOR X = XMVector4Normalize(v1);
    XMStoreFloat4(this, X);
}

inline void FVector4::Normalize(FVector4& result) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(this);
    XMVECTOR X = XMVector4Normalize(v1);
    XMStoreFloat4(&result, X);
}

inline void FVector4::Clamp(const FVector4& vmin, const FVector4& vmax) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(this);
    XMVECTOR v2 = XMLoadFloat4(&vmin);
    XMVECTOR v3 = XMLoadFloat4(&vmax);
    XMVECTOR X = XMVectorClamp(v1, v2, v3);
    XMStoreFloat4(this, X);
}

inline void FVector4::Clamp(const FVector4& vmin, const FVector4& vmax, FVector4& result) const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(this);
    XMVECTOR v2 = XMLoadFloat4(&vmin);
    XMVECTOR v3 = XMLoadFloat4(&vmax);
    XMVECTOR X = XMVectorClamp(v1, v2, v3);
    XMStoreFloat4(&result, X);
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

inline float FVector4::Distance(const FVector4& v1, const FVector4& v2) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(&v1);
    XMVECTOR x2 = XMLoadFloat4(&v2);
    XMVECTOR V = XMVectorSubtract(x2, x1);
    XMVECTOR X = XMVector4Length(V);
    return XMVectorGetX(X);
}

inline float FVector4::DistanceSquared(const FVector4& v1, const FVector4& v2) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(&v1);
    XMVECTOR x2 = XMLoadFloat4(&v2);
    XMVECTOR V = XMVectorSubtract(x2, x1);
    XMVECTOR X = XMVector4LengthSq(V);
    return XMVectorGetX(X);
}

inline void FVector4::Min(const FVector4& v1, const FVector4& v2, FVector4& result) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(&v1);
    XMVECTOR x2 = XMLoadFloat4(&v2);
    XMVECTOR X = XMVectorMin(x1, x2);
    XMStoreFloat4(&result, X);
}

inline FVector4 FVector4::Min(const FVector4& v1, const FVector4& v2) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(&v1);
    XMVECTOR x2 = XMLoadFloat4(&v2);
    XMVECTOR X = XMVectorMin(x1, x2);

    FVector4 result;
    XMStoreFloat4(&result, X);
    return result;
}

inline void FVector4::Max(const FVector4& v1, const FVector4& v2, FVector4& result) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(&v1);
    XMVECTOR x2 = XMLoadFloat4(&v2);
    XMVECTOR X = XMVectorMax(x1, x2);
    XMStoreFloat4(&result, X);
}

inline FVector4 FVector4::Max(const FVector4& v1, const FVector4& v2) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(&v1);
    XMVECTOR x2 = XMLoadFloat4(&v2);
    XMVECTOR X = XMVectorMax(x1, x2);

    FVector4 result;
    XMStoreFloat4(&result, X);
    return result;
}

inline void FVector4::Lerp(const FVector4& v1, const FVector4& v2, float t, FVector4& result) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(&v1);
    XMVECTOR x2 = XMLoadFloat4(&v2);
    XMVECTOR X = XMVectorLerp(x1, x2, t);
    XMStoreFloat4(&result, X);
}

inline FVector4 FVector4::Lerp(const FVector4& v1, const FVector4& v2, float t) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(&v1);
    XMVECTOR x2 = XMLoadFloat4(&v2);
    XMVECTOR X = XMVectorLerp(x1, x2, t);

    FVector4 result;
    XMStoreFloat4(&result, X);
    return result;
}

inline void FVector4::SmoothStep(const FVector4& v1, const FVector4& v2, float t, FVector4& result) noexcept
{
    using namespace DirectX;
    t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
    t = t * t * (3.f - 2.f * t);
    XMVECTOR x1 = XMLoadFloat4(&v1);
    XMVECTOR x2 = XMLoadFloat4(&v2);
    XMVECTOR X = XMVectorLerp(x1, x2, t);
    XMStoreFloat4(&result, X);
}

inline FVector4 FVector4::SmoothStep(const FVector4& v1, const FVector4& v2, float t) noexcept
{
    using namespace DirectX;
    t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
    t = t * t * (3.f - 2.f * t);
    XMVECTOR x1 = XMLoadFloat4(&v1);
    XMVECTOR x2 = XMLoadFloat4(&v2);
    XMVECTOR X = XMVectorLerp(x1, x2, t);

    FVector4 result;
    XMStoreFloat4(&result, X);
    return result;
}

inline void FVector4::Barycentric(const FVector4& v1, const FVector4& v2, const FVector4& v3, float f, float g, FVector4& result) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(&v1);
    XMVECTOR x2 = XMLoadFloat4(&v2);
    XMVECTOR x3 = XMLoadFloat4(&v3);
    XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);
    XMStoreFloat4(&result, X);
}

inline FVector4 FVector4::Barycentric(const FVector4& v1, const FVector4& v2, const FVector4& v3, float f, float g) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(&v1);
    XMVECTOR x2 = XMLoadFloat4(&v2);
    XMVECTOR x3 = XMLoadFloat4(&v3);
    XMVECTOR X = XMVectorBaryCentric(x1, x2, x3, f, g);

    FVector4 result;
    XMStoreFloat4(&result, X);
    return result;
}

inline void FVector4::CatmullRom(const FVector4& v1, const FVector4& v2, const FVector4& v3, const FVector4& v4, float t, FVector4& result) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(&v1);
    XMVECTOR x2 = XMLoadFloat4(&v2);
    XMVECTOR x3 = XMLoadFloat4(&v3);
    XMVECTOR x4 = XMLoadFloat4(&v4);
    XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);
    XMStoreFloat4(&result, X);
}

inline FVector4 FVector4::CatmullRom(const FVector4& v1, const FVector4& v2, const FVector4& v3, const FVector4& v4, float t) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(&v1);
    XMVECTOR x2 = XMLoadFloat4(&v2);
    XMVECTOR x3 = XMLoadFloat4(&v3);
    XMVECTOR x4 = XMLoadFloat4(&v4);
    XMVECTOR X = XMVectorCatmullRom(x1, x2, x3, x4, t);

    FVector4 result;
    XMStoreFloat4(&result, X);
    return result;
}

inline void FVector4::Hermite(const FVector4& v1, const FVector4& t1, const FVector4& v2, const FVector4& t2, float t, FVector4& result) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(&v1);
    XMVECTOR x2 = XMLoadFloat4(&t1);
    XMVECTOR x3 = XMLoadFloat4(&v2);
    XMVECTOR x4 = XMLoadFloat4(&t2);
    XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);
    XMStoreFloat4(&result, X);
}

inline FVector4 FVector4::Hermite(const FVector4& v1, const FVector4& t1, const FVector4& v2, const FVector4& t2, float t) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(&v1);
    XMVECTOR x2 = XMLoadFloat4(&t1);
    XMVECTOR x3 = XMLoadFloat4(&v2);
    XMVECTOR x4 = XMLoadFloat4(&t2);
    XMVECTOR X = XMVectorHermite(x1, x2, x3, x4, t);

    FVector4 result;
    XMStoreFloat4(&result, X);
    return result;
}

inline void FVector4::Reflect(const FVector4& ivec, const FVector4& nvec, FVector4& result) noexcept
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat4(&ivec);
    XMVECTOR n = XMLoadFloat4(&nvec);
    XMVECTOR X = XMVector4Reflect(i, n);
    XMStoreFloat4(&result, X);
}

inline FVector4 FVector4::Reflect(const FVector4& ivec, const FVector4& nvec) noexcept
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat4(&ivec);
    XMVECTOR n = XMLoadFloat4(&nvec);
    XMVECTOR X = XMVector4Reflect(i, n);

    FVector4 result;
    XMStoreFloat4(&result, X);
    return result;
}

inline void FVector4::Refract(const FVector4& ivec, const FVector4& nvec, float refractionIndex, FVector4& result) noexcept
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat4(&ivec);
    XMVECTOR n = XMLoadFloat4(&nvec);
    XMVECTOR X = XMVector4Refract(i, n, refractionIndex);
    XMStoreFloat4(&result, X);
}

inline FVector4 FVector4::Refract(const FVector4& ivec, const FVector4& nvec, float refractionIndex) noexcept
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat4(&ivec);
    XMVECTOR n = XMLoadFloat4(&nvec);
    XMVECTOR X = XMVector4Refract(i, n, refractionIndex);

    FVector4 result;
    XMStoreFloat4(&result, X);
    return result;
}

inline void FVector4::FTransform(const FVector2& v, const FQuat& quat, FVector4& result) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(&v);
    XMVECTOR q = XMLoadFloat4(&quat);
    XMVECTOR X = XMVector3Rotate(v1, q);
    X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f
    XMStoreFloat4(&result, X);
}

inline FVector4 FVector4::FTransform(const FVector2& v, const FQuat& quat) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2(&v);
    XMVECTOR q = XMLoadFloat4(&quat);
    XMVECTOR X = XMVector3Rotate(v1, q);
    X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f

    FVector4 result;
    XMStoreFloat4(&result, X);
    return result;
}

inline void FVector4::FTransform(const FVector3& v, const FQuat& quat, FVector4& result) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(&v);
    XMVECTOR q = XMLoadFloat4(&quat);
    XMVECTOR X = XMVector3Rotate(v1, q);
    X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f
    XMStoreFloat4(&result, X);
}

inline FVector4 FVector4::FTransform(const FVector3& v, const FQuat& quat) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3(&v);
    XMVECTOR q = XMLoadFloat4(&quat);
    XMVECTOR X = XMVector3Rotate(v1, q);
    X = XMVectorSelect(g_XMIdentityR3, X, g_XMSelect1110); // result.w = 1.f

    FVector4 result;
    XMStoreFloat4(&result, X);
    return result;
}

inline void FVector4::FTransform(const FVector4& v, const FQuat& quat, FVector4& result) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(&v);
    XMVECTOR q = XMLoadFloat4(&quat);
    XMVECTOR X = XMVector3Rotate(v1, q);
    X = XMVectorSelect(v1, X, g_XMSelect1110); // result.w = v.w
    XMStoreFloat4(&result, X);
}

inline FVector4 FVector4::FTransform(const FVector4& v, const FQuat& quat) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(&v);
    XMVECTOR q = XMLoadFloat4(&quat);
    XMVECTOR X = XMVector3Rotate(v1, q);
    X = XMVectorSelect(v1, X, g_XMSelect1110); // result.w = v.w

    FVector4 result;
    XMStoreFloat4(&result, X);
    return result;
}

inline void FVector4::FTransform(const FVector4& v, const FMatrix& m, FVector4& result) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(&v);
    XMMATRIX M = XMLoadFloat4x4(&m);
    XMVECTOR X = XMVector4Transform(v1, M);
    XMStoreFloat4(&result, X);
}

inline FVector4 FVector4::FTransform(const FVector4& v, const FMatrix& m) noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(&v);
    XMMATRIX M = XMLoadFloat4x4(&m);
    XMVECTOR X = XMVector4Transform(v1, M);

    FVector4 result;
    XMStoreFloat4(&result, X);
    return result;
}

_Use_decl_annotations_
inline void FVector4::FTransform(const FVector4* varray, size_t count, const FMatrix& m, FVector4* resultArray) noexcept
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4(&m);
    XMVector4TransformStream(resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT4), count, M);
}



// =======================================================================
// FMatrix Implementation
// =======================================================================


//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

inline bool FMatrix::operator == (const FMatrix& M) const noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

    XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
    XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
    XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
    XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

    return (XMVector4Equal(x1, y1)
        && XMVector4Equal(x2, y2)
        && XMVector4Equal(x3, y3)
        && XMVector4Equal(x4, y4)) != 0;
}

inline bool FMatrix::operator != (const FMatrix& M) const noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

    XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
    XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
    XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
    XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

    return (XMVector4NotEqual(x1, y1)
        || XMVector4NotEqual(x2, y2)
        || XMVector4NotEqual(x3, y3)
        || XMVector4NotEqual(x4, y4)) != 0;
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

inline FMatrix::FMatrix(const XMFLOAT3X3& M) noexcept
{
    _11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
    _21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
    _31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
    _41 = 0.f;   _42 = 0.f;   _43 = 0.f;   _44 = 1.f;
}

inline FMatrix::FMatrix(const XMFLOAT4X3& M) noexcept
{
    _11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
    _21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
    _31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
    _41 = M._41; _42 = M._42; _43 = M._43; _44 = 1.f;
}

inline FMatrix& FMatrix::operator= (const XMFLOAT3X3& M) noexcept
{
    _11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
    _21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
    _31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
    _41 = 0.f;   _42 = 0.f;   _43 = 0.f;   _44 = 1.f;
    return *this;
}

inline FMatrix& FMatrix::operator= (const XMFLOAT4X3& M) noexcept
{
    _11 = M._11; _12 = M._12; _13 = M._13; _14 = 0.f;
    _21 = M._21; _22 = M._22; _23 = M._23; _24 = 0.f;
    _31 = M._31; _32 = M._32; _33 = M._33; _34 = 0.f;
    _41 = M._41; _42 = M._42; _43 = M._43; _44 = 1.f;
    return *this;
}

inline FMatrix& FMatrix::operator+= (const FMatrix& M) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

    XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
    XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
    XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
    XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

    x1 = XMVectorAdd(x1, y1);
    x2 = XMVectorAdd(x2, y2);
    x3 = XMVectorAdd(x3, y3);
    x4 = XMVectorAdd(x4, y4);

    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
    return *this;
}

inline FMatrix& FMatrix::operator-= (const FMatrix& M) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

    XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
    XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
    XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
    XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

    x1 = XMVectorSubtract(x1, y1);
    x2 = XMVectorSubtract(x2, y2);
    x3 = XMVectorSubtract(x3, y3);
    x4 = XMVectorSubtract(x4, y4);

    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
    return *this;
}

inline FMatrix& FMatrix::operator*= (const FMatrix& M) noexcept
{
    using namespace DirectX;
    XMMATRIX M1 = XMLoadFloat4x4(this);
    XMMATRIX M2 = XMLoadFloat4x4(&M);
    XMMATRIX X = XMMatrixMultiply(M1, M2);
    XMStoreFloat4x4(this, X);
    return *this;
}

inline FMatrix& FMatrix::operator*= (float S) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

    x1 = XMVectorScale(x1, S);
    x2 = XMVectorScale(x2, S);
    x3 = XMVectorScale(x3, S);
    x4 = XMVectorScale(x4, S);

    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
    return *this;
}

inline FMatrix& FMatrix::operator/= (float S) noexcept
{
    using namespace DirectX;
    assert(S != 0.f);
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

    float rs = 1.f / S;

    x1 = XMVectorScale(x1, rs);
    x2 = XMVectorScale(x2, rs);
    x3 = XMVectorScale(x3, rs);
    x4 = XMVectorScale(x4, rs);

    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
    return *this;
}

inline FMatrix& FMatrix::operator/= (const FMatrix& M) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

    XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
    XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
    XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
    XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

    x1 = XMVectorDivide(x1, y1);
    x2 = XMVectorDivide(x2, y2);
    x3 = XMVectorDivide(x3, y3);
    x4 = XMVectorDivide(x4, y4);

    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&_41), x4);
    return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

inline FMatrix FMatrix::operator- () const noexcept
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_11));
    XMVECTOR v2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_21));
    XMVECTOR v3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_31));
    XMVECTOR v4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&_41));

    v1 = XMVectorNegate(v1);
    v2 = XMVectorNegate(v2);
    v3 = XMVectorNegate(v3);
    v4 = XMVectorNegate(v4);

    FMatrix R;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), v1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), v2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), v3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), v4);
    return R;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

inline FMatrix operator+ (const FMatrix& M1, const FMatrix& M2) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

    XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
    XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
    XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
    XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

    x1 = XMVectorAdd(x1, y1);
    x2 = XMVectorAdd(x2, y2);
    x3 = XMVectorAdd(x3, y3);
    x4 = XMVectorAdd(x4, y4);

    FMatrix R;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
    return R;
}

inline FMatrix operator- (const FMatrix& M1, const FMatrix& M2) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

    XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
    XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
    XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
    XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

    x1 = XMVectorSubtract(x1, y1);
    x2 = XMVectorSubtract(x2, y2);
    x3 = XMVectorSubtract(x3, y3);
    x4 = XMVectorSubtract(x4, y4);

    FMatrix R;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
    return R;
}

inline FMatrix operator* (const FMatrix& M1, const FMatrix& M2) noexcept
{
    using namespace DirectX;
    XMMATRIX m1 = XMLoadFloat4x4(&M1);
    XMMATRIX m2 = XMLoadFloat4x4(&M2);
    XMMATRIX X = XMMatrixMultiply(m1, m2);

    FMatrix R;
    XMStoreFloat4x4(&R, X);
    return R;
}

inline FMatrix operator* (const FMatrix& M, float S) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

    x1 = XMVectorScale(x1, S);
    x2 = XMVectorScale(x2, S);
    x3 = XMVectorScale(x3, S);
    x4 = XMVectorScale(x4, S);

    FMatrix R;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
    return R;
}

inline FMatrix operator/ (const FMatrix& M, float S) noexcept
{
    using namespace DirectX;
    assert(S != 0.f);

    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

    float rs = 1.f / S;

    x1 = XMVectorScale(x1, rs);
    x2 = XMVectorScale(x2, rs);
    x3 = XMVectorScale(x3, rs);
    x4 = XMVectorScale(x4, rs);

    FMatrix R;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
    return R;
}

inline FMatrix operator/ (const FMatrix& M1, const FMatrix& M2) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

    XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
    XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
    XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
    XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

    x1 = XMVectorDivide(x1, y1);
    x2 = XMVectorDivide(x2, y2);
    x3 = XMVectorDivide(x3, y3);
    x4 = XMVectorDivide(x4, y4);

    FMatrix R;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
    return R;
}

inline FMatrix operator* (float S, const FMatrix& M) noexcept
{
    using namespace DirectX;

    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M._41));

    x1 = XMVectorScale(x1, S);
    x2 = XMVectorScale(x2, S);
    x3 = XMVectorScale(x3, S);
    x4 = XMVectorScale(x4, S);

    FMatrix R;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&R._41), x4);
    return R;
}

//------------------------------------------------------------------------------
// FMatrix operations
//------------------------------------------------------------------------------

inline bool FMatrix::Decompose(FVector3& scale, FQuat& rotation, FVector3& translation) noexcept
{
    using namespace DirectX;

    XMVECTOR s, r, t;

    if (!XMMatrixDecompose(&s, &r, &t, *this))
        return false;

    XMStoreFloat3(&scale, s);
    XMStoreFloat4(&rotation, r);
    XMStoreFloat3(&translation, t);

    return true;
}

inline FMatrix FMatrix::Transpose() const noexcept
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4(this);
    FMatrix R;
    XMStoreFloat4x4(&R, XMMatrixTranspose(M));
    return R;
}

inline void FMatrix::Transpose(FMatrix& result) const noexcept
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4(this);
    XMStoreFloat4x4(&result, XMMatrixTranspose(M));
}

inline FMatrix FMatrix::Invert() const noexcept
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4(this);
    FMatrix R;
    XMVECTOR det;
    XMStoreFloat4x4(&R, XMMatrixInverse(&det, M));
    return R;
}

inline void FMatrix::Invert(FMatrix& result) const noexcept
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4(this);
    XMVECTOR det;
    XMStoreFloat4x4(&result, XMMatrixInverse(&det, M));
}

inline float FMatrix::Determinant() const noexcept
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4(this);
    return XMVectorGetX(XMMatrixDeterminant(M));
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

_Use_decl_annotations_
inline FMatrix FMatrix::CreateBillboard(
    const FVector3& object,
    const FVector3& cameraPosition,
    const FVector3& cameraUp,
    const FVector3* cameraForward) noexcept
{
    using namespace DirectX;
    XMVECTOR O = XMLoadFloat3(&object);
    XMVECTOR C = XMLoadFloat3(&cameraPosition);
    XMVECTOR Z = XMVectorSubtract(O, C);

    XMVECTOR N = XMVector3LengthSq(Z);
    if (XMVector3Less(N, g_XMEpsilon))
    {
        if (cameraForward)
        {
            XMVECTOR F = XMLoadFloat3(cameraForward);
            Z = XMVectorNegate(F);
        }
        else
            Z = g_XMNegIdentityR2;
    }
    else
    {
        Z = XMVector3Normalize(Z);
    }

    XMVECTOR up = XMLoadFloat3(&cameraUp);
    XMVECTOR X = XMVector3Cross(up, Z);
    X = XMVector3Normalize(X);

    XMVECTOR Y = XMVector3Cross(Z, X);

    XMMATRIX M;
    M.r[0] = X;
    M.r[1] = Y;
    M.r[2] = Z;
    M.r[3] = XMVectorSetW(O, 1.f);

    FMatrix R;
    XMStoreFloat4x4(&R, M);
    return R;
}

_Use_decl_annotations_
inline FMatrix FMatrix::CreateConstrainedBillboard(
    const FVector3& object,
    const FVector3& cameraPosition,
    const FVector3& rotateAxis,
    const FVector3* cameraForward,
    const FVector3* objectForward) noexcept
{
    using namespace DirectX;

    static const XMVECTORF32 s_minAngle = { { { 0.99825467075f, 0.99825467075f, 0.99825467075f, 0.99825467075f } } }; // 1.0 - XMConvertToRadians( 0.1f );

    XMVECTOR O = XMLoadFloat3(&object);
    XMVECTOR C = XMLoadFloat3(&cameraPosition);
    XMVECTOR faceDir = XMVectorSubtract(O, C);

    XMVECTOR N = XMVector3LengthSq(faceDir);
    if (XMVector3Less(N, g_XMEpsilon))
    {
        if (cameraForward)
        {
            XMVECTOR F = XMLoadFloat3(cameraForward);
            faceDir = XMVectorNegate(F);
        }
        else
            faceDir = g_XMNegIdentityR2;
    }
    else
    {
        faceDir = XMVector3Normalize(faceDir);
    }

    XMVECTOR Y = XMLoadFloat3(&rotateAxis);
    XMVECTOR X, Z;

    XMVECTOR dot = XMVectorAbs(XMVector3Dot(Y, faceDir));
    if (XMVector3Greater(dot, s_minAngle))
    {
        if (objectForward)
        {
            Z = XMLoadFloat3(objectForward);
            dot = XMVectorAbs(XMVector3Dot(Y, Z));
            if (XMVector3Greater(dot, s_minAngle))
            {
                dot = XMVectorAbs(XMVector3Dot(Y, g_XMNegIdentityR2));
                Z = (XMVector3Greater(dot, s_minAngle)) ? g_XMIdentityR0 : g_XMNegIdentityR2;
            }
        }
        else
        {
            dot = XMVectorAbs(XMVector3Dot(Y, g_XMNegIdentityR2));
            Z = (XMVector3Greater(dot, s_minAngle)) ? g_XMIdentityR0 : g_XMNegIdentityR2;
        }

        X = XMVector3Cross(Y, Z);
        X = XMVector3Normalize(X);

        Z = XMVector3Cross(X, Y);
        Z = XMVector3Normalize(Z);
    }
    else
    {
        X = XMVector3Cross(Y, faceDir);
        X = XMVector3Normalize(X);

        Z = XMVector3Cross(X, Y);
        Z = XMVector3Normalize(Z);
    }

    XMMATRIX M;
    M.r[0] = X;
    M.r[1] = Y;
    M.r[2] = Z;
    M.r[3] = XMVectorSetW(O, 1.f);

    FMatrix R;
    XMStoreFloat4x4(&R, M);
    return R;
}

inline FMatrix FMatrix::CreateTranslation(const FVector3& position) noexcept
{
    using namespace DirectX;
    FMatrix R;
    XMStoreFloat4x4(&R, XMMatrixTranslation(position.x, position.y, position.z));
    return R;
}

inline FMatrix FMatrix::CreateTranslation(float x, float y, float z) noexcept
{
    using namespace DirectX;
    FMatrix R;
    XMStoreFloat4x4(&R, XMMatrixTranslation(x, y, z));
    return R;
}

inline FMatrix FMatrix::CreateScale(const FVector3& scales) noexcept
{
    using namespace DirectX;
    FMatrix R;
    XMStoreFloat4x4(&R, XMMatrixScaling(scales.x, scales.y, scales.z));
    return R;
}

inline FMatrix FMatrix::CreateScale(float xs, float ys, float zs) noexcept
{
    using namespace DirectX;
    FMatrix R;
    XMStoreFloat4x4(&R, XMMatrixScaling(xs, ys, zs));
    return R;
}

inline FMatrix FMatrix::CreateScale(float scale) noexcept
{
    using namespace DirectX;
    FMatrix R;
    XMStoreFloat4x4(&R, XMMatrixScaling(scale, scale, scale));
    return R;
}

inline FMatrix FMatrix::CreateRotationX(float radians) noexcept
{
    using namespace DirectX;
    FMatrix R;
    XMStoreFloat4x4(&R, XMMatrixRotationX(radians));
    return R;
}

inline FMatrix FMatrix::CreateRotationY(float radians) noexcept
{
    using namespace DirectX;
    FMatrix R;
    XMStoreFloat4x4(&R, XMMatrixRotationY(radians));
    return R;
}

inline FMatrix FMatrix::CreateRotationZ(float radians) noexcept
{
    using namespace DirectX;
    FMatrix R;
    XMStoreFloat4x4(&R, XMMatrixRotationZ(radians));
    return R;
}

inline FMatrix FMatrix::CreateFromAxisAngle(const FVector3& axis, float angle) noexcept
{
    using namespace DirectX;
    FMatrix R;
    XMVECTOR a = XMLoadFloat3(&axis);
    XMStoreFloat4x4(&R, XMMatrixRotationAxis(a, angle));
    return R;
}

inline FMatrix FMatrix::CreatePerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept
{
    using namespace DirectX;
    FMatrix R;
    XMStoreFloat4x4(&R, XMMatrixPerspectiveFovRH(fov, aspectRatio, nearPlane, farPlane));
    return R;
}

inline FMatrix FMatrix::CreatePerspective(float width, float height, float nearPlane, float farPlane) noexcept
{
    using namespace DirectX;
    FMatrix R;
    XMStoreFloat4x4(&R, XMMatrixPerspectiveRH(width, height, nearPlane, farPlane));
    return R;
}

inline FMatrix FMatrix::CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane) noexcept
{
    using namespace DirectX;
    FMatrix R;
    XMStoreFloat4x4(&R, XMMatrixPerspectiveOffCenterRH(left, right, bottom, top, nearPlane, farPlane));
    return R;
}

inline FMatrix FMatrix::CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane) noexcept
{
    using namespace DirectX;
    FMatrix R;
    XMStoreFloat4x4(&R, XMMatrixOrthographicRH(width, height, zNearPlane, zFarPlane));
    return R;
}

inline FMatrix FMatrix::CreateOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane) noexcept
{
    using namespace DirectX;
    FMatrix R;
    XMStoreFloat4x4(&R, XMMatrixOrthographicOffCenterRH(left, right, bottom, top, zNearPlane, zFarPlane));
    return R;
}

inline FMatrix FMatrix::CreateLookAt(const FVector3& eye, const FVector3& target, const FVector3& up) noexcept
{
    using namespace DirectX;
    FMatrix R;
    XMVECTOR eyev = XMLoadFloat3(&eye);
    XMVECTOR targetv = XMLoadFloat3(&target);
    XMVECTOR upv = XMLoadFloat3(&up);
    XMStoreFloat4x4(&R, XMMatrixLookAtRH(eyev, targetv, upv));
    return R;
}

inline FMatrix FMatrix::CreateWorld(const FVector3& position, const FVector3& forward, const FVector3& up) noexcept
{
    using namespace DirectX;
    XMVECTOR zaxis = XMVector3Normalize(XMVectorNegate(XMLoadFloat3(&forward)));
    XMVECTOR yaxis = XMLoadFloat3(&up);
    XMVECTOR xaxis = XMVector3Normalize(XMVector3Cross(yaxis, zaxis));
    yaxis = XMVector3Cross(zaxis, xaxis);

    FMatrix R;
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._11), xaxis);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._21), yaxis);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&R._31), zaxis);
    R._14 = R._24 = R._34 = 0.f;
    R._41 = position.x; R._42 = position.y; R._43 = position.z;
    R._44 = 1.f;
    return R;
}

inline FMatrix FMatrix::CreateFromQuaternion(const FQuat& rotation) noexcept
{
    using namespace DirectX;
    FMatrix R;
    XMVECTOR quatv = XMLoadFloat4(&rotation);
    XMStoreFloat4x4(&R, XMMatrixRotationQuaternion(quatv));
    return R;
}

inline FMatrix FMatrix::CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept
{
    using namespace DirectX;
    FMatrix R;
    XMStoreFloat4x4(&R, XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
    return R;
}

inline FMatrix FMatrix::CreateShadow(const FVector3& lightDir, const Plane& plane) noexcept
{
    using namespace DirectX;
    FMatrix R;
    XMVECTOR light = XMLoadFloat3(&lightDir);
    XMVECTOR planev = XMLoadFloat4(&plane);
    XMStoreFloat4x4(&R, XMMatrixShadow(planev, light));
    return R;
}

inline FMatrix FMatrix::CreateReflection(const Plane& plane) noexcept
{
    using namespace DirectX;
    FMatrix R;
    XMVECTOR planev = XMLoadFloat4(&plane);
    XMStoreFloat4x4(&R, XMMatrixReflect(planev));
    return R;
}

inline void FMatrix::Lerp(const FMatrix& M1, const FMatrix& M2, float t, FMatrix& result) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

    XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
    XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
    XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
    XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

    x1 = XMVectorLerp(x1, y1, t);
    x2 = XMVectorLerp(x2, y2, t);
    x3 = XMVectorLerp(x3, y3, t);
    x4 = XMVectorLerp(x4, y4, t);

    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._41), x4);
}

inline FMatrix FMatrix::Lerp(const FMatrix& M1, const FMatrix& M2, float t) noexcept
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._11));
    XMVECTOR x2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._21));
    XMVECTOR x3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._31));
    XMVECTOR x4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M1._41));

    XMVECTOR y1 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._11));
    XMVECTOR y2 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._21));
    XMVECTOR y3 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._31));
    XMVECTOR y4 = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&M2._41));

    x1 = XMVectorLerp(x1, y1, t);
    x2 = XMVectorLerp(x2, y2, t);
    x3 = XMVectorLerp(x3, y3, t);
    x4 = XMVectorLerp(x4, y4, t);

    FMatrix result;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._11), x1);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._21), x2);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._31), x3);
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&result._41), x4);
    return result;
}

inline void FMatrix::FTransform(const FMatrix& M, const FQuat& rotation, FMatrix& result) noexcept
{
    using namespace DirectX;
    XMVECTOR quatv = XMLoadFloat4(&rotation);

    XMMATRIX M0 = XMLoadFloat4x4(&M);
    XMMATRIX M1 = XMMatrixRotationQuaternion(quatv);

    XMStoreFloat4x4(&result, XMMatrixMultiply(M0, M1));
}

inline FMatrix FMatrix::FTransform(const FMatrix& M, const FQuat& rotation) noexcept
{
    using namespace DirectX;
    XMVECTOR quatv = XMLoadFloat4(&rotation);

    XMMATRIX M0 = XMLoadFloat4x4(&M);
    XMMATRIX M1 = XMMatrixRotationQuaternion(quatv);

    FMatrix result;
    XMStoreFloat4x4(&result, XMMatrixMultiply(M0, M1));
    return result;
}



// =======================================================================
// FQuat Implementation
// =======================================================================


//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

inline bool FQuat::operator == (const FQuat& q) const noexcept
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4(this);
    XMVECTOR q2 = XMLoadFloat4(&q);
    return XMQuaternionEqual(q1, q2);
}

inline bool FQuat::operator != (const FQuat& q) const noexcept
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4(this);
    XMVECTOR q2 = XMLoadFloat4(&q);
    return XMQuaternionNotEqual(q1, q2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

inline FQuat& FQuat::operator+= (const FQuat& q) noexcept
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4(this);
    XMVECTOR q2 = XMLoadFloat4(&q);
    XMStoreFloat4(this, XMVectorAdd(q1, q2));
    return *this;
}

inline FQuat& FQuat::operator-= (const FQuat& q) noexcept
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4(this);
    XMVECTOR q2 = XMLoadFloat4(&q);
    XMStoreFloat4(this, XMVectorSubtract(q1, q2));
    return *this;
}

inline FQuat& FQuat::operator*= (const FQuat& q) noexcept
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4(this);
    XMVECTOR q2 = XMLoadFloat4(&q);
    XMStoreFloat4(this, XMQuaternionMultiply(q1, q2));
    return *this;
}

inline FQuat& FQuat::operator*= (float S) noexcept
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4(this);
    XMStoreFloat4(this, XMVectorScale(q, S));
    return *this;
}

inline FQuat& FQuat::operator/= (const FQuat& q) noexcept
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4(this);
    XMVECTOR q2 = XMLoadFloat4(&q);
    q2 = XMQuaternionInverse(q2);
    XMStoreFloat4(this, XMQuaternionMultiply(q1, q2));
    return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

inline FQuat FQuat::operator- () const noexcept
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4(this);

    FQuat R;
    XMStoreFloat4(&R, XMVectorNegate(q));
    return R;
}

//------------------------------------------------------------------------------
// Utility
//------------------------------------------------------------------------------

inline String FQuat::ToString() const
{
    std::stringstream Stream;
    Stream << "(" << x << ", " << y << ", " << z << ", " << w << ")";
    return Stream.str();
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

inline FQuat operator+ (const FQuat& Q1, const FQuat& Q2) noexcept
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4(&Q1);
    XMVECTOR q2 = XMLoadFloat4(&Q2);

    FQuat R;
    XMStoreFloat4(&R, XMVectorAdd(q1, q2));
    return R;
}

inline FQuat operator- (const FQuat& Q1, const FQuat& Q2) noexcept
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4(&Q1);
    XMVECTOR q2 = XMLoadFloat4(&Q2);

    FQuat R;
    XMStoreFloat4(&R, XMVectorSubtract(q1, q2));
    return R;
}

inline FQuat operator* (const FQuat& Q1, const FQuat& Q2) noexcept
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4(&Q1);
    XMVECTOR q2 = XMLoadFloat4(&Q2);

    FQuat R;
    XMStoreFloat4(&R, XMQuaternionMultiply(q1, q2));
    return R;
}

inline FQuat operator* (const FQuat& Q, float S) noexcept
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4(&Q);

    FQuat R;
    XMStoreFloat4(&R, XMVectorScale(q, S));
    return R;
}

inline FQuat operator/ (const FQuat& Q1, const FQuat& Q2) noexcept
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4(&Q1);
    XMVECTOR q2 = XMLoadFloat4(&Q2);
    q2 = XMQuaternionInverse(q2);

    FQuat R;
    XMStoreFloat4(&R, XMQuaternionMultiply(q1, q2));
    return R;
}

inline FQuat operator* (float S, const FQuat& Q) noexcept
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4(&Q);

    FQuat R;
    XMStoreFloat4(&R, XMVectorScale(q1, S));
    return R;
}

//------------------------------------------------------------------------------
// FQuat operations
//------------------------------------------------------------------------------

inline float FQuat::Length() const noexcept
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4(this);
    return XMVectorGetX(XMQuaternionLength(q));
}

inline float FQuat::LengthSquared() const noexcept
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4(this);
    return XMVectorGetX(XMQuaternionLengthSq(q));
}

inline void FQuat::Normalize() noexcept
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4(this);
    XMStoreFloat4(this, XMQuaternionNormalize(q));
}

inline void FQuat::Normalize(FQuat& result) const noexcept
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4(this);
    XMStoreFloat4(&result, XMQuaternionNormalize(q));
}

inline void FQuat::Conjugate() noexcept
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4(this);
    XMStoreFloat4(this, XMQuaternionConjugate(q));
}

inline void FQuat::Conjugate(FQuat& result) const noexcept
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4(this);
    XMStoreFloat4(&result, XMQuaternionConjugate(q));
}

inline void FQuat::Inverse(FQuat& result) const noexcept
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4(this);
    XMStoreFloat4(&result, XMQuaternionInverse(q));
}

inline float FQuat::Dot(const FQuat& q) const noexcept
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4(this);
    XMVECTOR q2 = XMLoadFloat4(&q);
    return XMVectorGetX(XMQuaternionDot(q1, q2));
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

inline FQuat FQuat::CreateFromAxisAngle(const FVector3& axis, float angle) noexcept
{
    using namespace DirectX;
    XMVECTOR a = XMLoadFloat3(&axis);

    FQuat R;
    XMStoreFloat4(&R, XMQuaternionRotationAxis(a, angle));
    return R;
}

inline FQuat FQuat::CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept
{
    using namespace DirectX;
    FQuat R;
    XMStoreFloat4(&R, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
    return R;
}

inline FQuat FQuat::CreateFromRotationMatrix(const FMatrix& M) noexcept
{
    using namespace DirectX;
    XMMATRIX M0 = XMLoadFloat4x4(&M);

    FQuat R;
    XMStoreFloat4(&R, XMQuaternionRotationMatrix(M0));
    return R;
}

inline void FQuat::Lerp(const FQuat& q1, const FQuat& q2, float t, FQuat& result) noexcept
{
    using namespace DirectX;
    XMVECTOR Q0 = XMLoadFloat4(&q1);
    XMVECTOR Q1 = XMLoadFloat4(&q2);

    XMVECTOR dot = XMVector4Dot(Q0, Q1);

    XMVECTOR R;
    if (XMVector4GreaterOrEqual(dot, XMVectorZero()))
    {
        R = XMVectorLerp(Q0, Q1, t);
    }
    else
    {
        XMVECTOR tv = XMVectorReplicate(t);
        XMVECTOR t1v = XMVectorReplicate(1.f - t);
        XMVECTOR X0 = XMVectorMultiply(Q0, t1v);
        XMVECTOR X1 = XMVectorMultiply(Q1, tv);
        R = XMVectorSubtract(X0, X1);
    }

    XMStoreFloat4(&result, XMQuaternionNormalize(R));
}

inline FQuat FQuat::Lerp(const FQuat& q1, const FQuat& q2, float t) noexcept
{
    using namespace DirectX;
    XMVECTOR Q0 = XMLoadFloat4(&q1);
    XMVECTOR Q1 = XMLoadFloat4(&q2);

    XMVECTOR dot = XMVector4Dot(Q0, Q1);

    XMVECTOR R;
    if (XMVector4GreaterOrEqual(dot, XMVectorZero()))
    {
        R = XMVectorLerp(Q0, Q1, t);
    }
    else
    {
        XMVECTOR tv = XMVectorReplicate(t);
        XMVECTOR t1v = XMVectorReplicate(1.f - t);
        XMVECTOR X0 = XMVectorMultiply(Q0, t1v);
        XMVECTOR X1 = XMVectorMultiply(Q1, tv);
        R = XMVectorSubtract(X0, X1);
    }

    FQuat result;
    XMStoreFloat4(&result, XMQuaternionNormalize(R));
    return result;
}

inline void FQuat::Slerp(const FQuat& q1, const FQuat& q2, float t, FQuat& result) noexcept
{
    using namespace DirectX;
    XMVECTOR Q0 = XMLoadFloat4(&q1);
    XMVECTOR Q1 = XMLoadFloat4(&q2);
    XMStoreFloat4(&result, XMQuaternionSlerp(Q0, Q1, t));
}

inline FQuat FQuat::Slerp(const FQuat& q1, const FQuat& q2, float t) noexcept
{
    using namespace DirectX;
    XMVECTOR Q0 = XMLoadFloat4(&q1);
    XMVECTOR Q1 = XMLoadFloat4(&q2);

    FQuat result;
    XMStoreFloat4(&result, XMQuaternionSlerp(Q0, Q1, t));
    return result;
}

inline void FQuat::Concatenate(const FQuat& q1, const FQuat& q2, FQuat& result) noexcept
{
    using namespace DirectX;
    XMVECTOR Q0 = XMLoadFloat4(&q1);
    XMVECTOR Q1 = XMLoadFloat4(&q2);
    XMStoreFloat4(&result, XMQuaternionMultiply(Q1, Q0));
}

inline FQuat FQuat::Concatenate(const FQuat& q1, const FQuat& q2) noexcept
{
    using namespace DirectX;
    XMVECTOR Q0 = XMLoadFloat4(&q1);
    XMVECTOR Q1 = XMLoadFloat4(&q2);

    FQuat result;
    XMStoreFloat4(&result, XMQuaternionMultiply(Q1, Q0));
    return result;
}



// =======================================================================
// Plane Implementation
// =======================================================================


inline Plane::Plane(const FVector3& point1, const FVector3& point2, const FVector3& point3) noexcept
{
    using namespace DirectX;
    XMVECTOR P0 = XMLoadFloat3(&point1);
    XMVECTOR P1 = XMLoadFloat3(&point2);
    XMVECTOR P2 = XMLoadFloat3(&point3);
    XMStoreFloat4(this, XMPlaneFromPoints(P0, P1, P2));
}

inline Plane::Plane(const FVector3& point, const FVector3& normal) noexcept
{
    using namespace DirectX;
    XMVECTOR P = XMLoadFloat3(&point);
    XMVECTOR N = XMLoadFloat3(&normal);
    XMStoreFloat4(this, XMPlaneFromPointNormal(P, N));
}

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

inline bool Plane::operator == (const Plane& p) const noexcept
{
    using namespace DirectX;
    XMVECTOR p1 = XMLoadFloat4(this);
    XMVECTOR p2 = XMLoadFloat4(&p);
    return XMPlaneEqual(p1, p2);
}

inline bool Plane::operator != (const Plane& p) const noexcept
{
    using namespace DirectX;
    XMVECTOR p1 = XMLoadFloat4(this);
    XMVECTOR p2 = XMLoadFloat4(&p);
    return XMPlaneNotEqual(p1, p2);
}

//------------------------------------------------------------------------------
// Plane operations
//------------------------------------------------------------------------------

inline void Plane::Normalize() noexcept
{
    using namespace DirectX;
    XMVECTOR p = XMLoadFloat4(this);
    XMStoreFloat4(this, XMPlaneNormalize(p));
}

inline void Plane::Normalize(Plane& result) const noexcept
{
    using namespace DirectX;
    XMVECTOR p = XMLoadFloat4(this);
    XMStoreFloat4(&result, XMPlaneNormalize(p));
}

inline float Plane::Dot(const FVector4& v) const noexcept
{
    using namespace DirectX;
    XMVECTOR p = XMLoadFloat4(this);
    XMVECTOR v0 = XMLoadFloat4(&v);
    return XMVectorGetX(XMPlaneDot(p, v0));
}

inline float Plane::DotCoordinate(const FVector3& position) const noexcept
{
    using namespace DirectX;
    XMVECTOR p = XMLoadFloat4(this);
    XMVECTOR v0 = XMLoadFloat3(&position);
    return XMVectorGetX(XMPlaneDotCoord(p, v0));
}

inline float Plane::DotNormal(const FVector3& normal) const noexcept
{
    using namespace DirectX;
    XMVECTOR p = XMLoadFloat4(this);
    XMVECTOR n0 = XMLoadFloat3(&normal);
    return XMVectorGetX(XMPlaneDotNormal(p, n0));
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

inline void Plane::FTransform(const Plane& plane, const FMatrix& M, Plane& result) noexcept
{
    using namespace DirectX;
    XMVECTOR p = XMLoadFloat4(&plane);
    XMMATRIX m0 = XMLoadFloat4x4(&M);
    XMStoreFloat4(&result, XMPlaneTransform(p, m0));
}

inline Plane Plane::FTransform(const Plane& plane, const FMatrix& M) noexcept
{
    using namespace DirectX;
    XMVECTOR p = XMLoadFloat4(&plane);
    XMMATRIX m0 = XMLoadFloat4x4(&M);

    Plane result;
    XMStoreFloat4(&result, XMPlaneTransform(p, m0));
    return result;
}

inline void Plane::FTransform(const Plane& plane, const FQuat& rotation, Plane& result) noexcept
{
    using namespace DirectX;
    XMVECTOR p = XMLoadFloat4(&plane);
    XMVECTOR q = XMLoadFloat4(&rotation);
    XMVECTOR X = XMVector3Rotate(p, q);
    X = XMVectorSelect(p, X, g_XMSelect1110); // result.d = plane.d
    XMStoreFloat4(&result, X);
}

inline Plane Plane::FTransform(const Plane& plane, const FQuat& rotation) noexcept
{
    using namespace DirectX;
    XMVECTOR p = XMLoadFloat4(&plane);
    XMVECTOR q = XMLoadFloat4(&rotation);
    XMVECTOR X = XMVector3Rotate(p, q);
    X = XMVectorSelect(p, X, g_XMSelect1110); // result.d = plane.d

    Plane result;
    XMStoreFloat4(&result, X);
    return result;
}

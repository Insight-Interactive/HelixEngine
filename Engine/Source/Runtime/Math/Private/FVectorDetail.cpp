#include "MathPCH.h"

#include "MathCore.h"


// ---------------------
//	FVector 2
// ---------------------

/* static */ const FAngles FAngles::Zero = { 0.f, 0.f, 0.f };


// ---------------------
//	FVector 2
// ---------------------

/* static */ const FVector2 FVector2::One = {1.f,  1.f};
/* static */ const FVector2 FVector2::Zero	= {  0.f,  0.f  };
											    
/* static */ const FVector2 FVector2::UnitX	= {  1.f,  0.f  };
/* static */ const FVector2 FVector2::UnitY	= {  0.f,  1.f  };
											    
/* static */ const FVector2 FVector2::Up		= {  0.f,  1.f  };
/* static */ const FVector2 FVector2::Down	= {  0.f, -1.f  };
/* static */ const FVector2 FVector2::Left	= { -1.f,  0.f  };
/* static */ const FVector2 FVector2::Right	= {  1.f,  0.f  };


// ---------------------
//	FVector 3
// ---------------------

/* static */ const FVector3 FVector3::One = { 1.f,  1.f,  1.f };
/* static */ const FVector3 FVector3::Zero = { 0.f,  0.f,  0.f };

/* static */ const FVector3 FVector3::UnitX = { 1.f,  0.f,  0.f };
/* static */ const FVector3 FVector3::UnitY = { 0.f,  1.f,  0.f };
/* static */ const FVector3 FVector3::UnitZ = { 0.f,  0.f,  1.f };

/* static */ const FVector3 FVector3::Up = { 0.f,  1.f,  0.f };
/* static */ const FVector3 FVector3::Down = { 0.f, -1.f,  0.f };
/* static */ const FVector3 FVector3::Left = { -1.f,  0.f,  0.f };
/* static */ const FVector3 FVector3::Right = { 1.f,  0.f,  0.f };
/* static */ const FVector3 FVector3::Forward = { 0.f,  0.f,  1.f };
/* static */ const FVector3 FVector3::Backward = { 0.f,  0.f, -1.f };


// ---------------------
//	FVector 4
// ---------------------

/* static */ const FVector4 FVector4::Zero = { 0.f, 0.f, 0.f, 0.f };
/* static */ const FVector4 FVector4::One = { 1.f, 1.f, 1.f, 1.f };
/* static */ const FVector4 FVector4::UnitX = { 1.f, 0.f, 0.f, 0.f };
/* static */ const FVector4 FVector4::UnitY = { 0.f, 1.f, 0.f, 0.f };
/* static */ const FVector4 FVector4::UnitZ = { 0.f, 0.f, 1.f, 0.f };
/* static */ const FVector4 FVector4::UnitW = { 0.f, 0.f, 0.f, 1.f };


// ---------------------
//	FMatrix
// ---------------------

/* static */ const FMatrix FMatrix::Identity = { 1.f, 0.f, 0.f, 0.f,
                                  0.f, 1.f, 0.f, 0.f,
                                  0.f, 0.f, 1.f, 0.f,
                                  0.f, 0.f, 0.f, 1.f };


// ---------------------
//	Quaternion
// ---------------------

/* static */ const FQuat FQuat::Identity = { 0.f, 0.f, 0.f, 1.f };

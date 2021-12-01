#include "CorePCH.h"

#include "GUID.h"

#if HE_WINDOWS
/*static*/ const FGUID FGUID::Invalid( uint32(-1) );
/*static*/ const FGUID FGUID::Zero( uint32(0) );

#endif
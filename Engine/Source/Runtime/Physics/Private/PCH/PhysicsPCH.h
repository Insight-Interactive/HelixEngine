#pragma once

#include <map>
#include <queue>
#include <vector>
#include <thread>
#include <string>
#include <unordered_map>
#include <sstream>
#include <iostream>

#include "PhysicsFwd.h"
#include "CoreFwd.h"
#include "Logger.h"
#include "StringHelper.h"

#if HE_WINDOWS
#	include "Windows.h"

#endif // HE_WINDOWS

#include "MathCore.h"

// PhysX
#include "PxPhysicsAPI.h"
#include "extensions/PxSimpleFactory.h"

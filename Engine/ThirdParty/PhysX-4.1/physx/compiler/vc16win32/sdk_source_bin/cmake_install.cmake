# Install script for directory: C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/compiler/cmake

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PhysX")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/source/foundation/include/windows" TYPE FILE FILES
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/windows/PsWindowsAoS.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/windows/PsWindowsFPU.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/windows/PsWindowsInclude.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/windows/PsWindowsInlineAoS.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/windows/PsWindowsIntrinsics.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/windows/PsWindowsTrigConstants.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/windows/PxWindowsIntrinsics.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/windows" TYPE FILE FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/windows/PxWindowsIntrinsics.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/unix/PxUnixIntrinsics.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/unix" TYPE FILE FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/unix/PxUnixIntrinsics.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysXFoundation_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysXFoundation_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysXFoundation_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysXFoundation_32.pdb")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxFoundation.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/foundation" TYPE FILE FILES
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/foundation/PxAssert.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/foundation/PxFoundationConfig.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/foundation/PxMathUtils.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/source/foundation/include" TYPE FILE FILES
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/Ps.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsAlignedMalloc.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsAlloca.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsAllocator.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsAoS.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsArray.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsAtomic.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsBasicTemplates.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsBitUtils.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsBroadcast.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsCpu.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsFoundation.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsFPU.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsHash.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsHashInternals.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsHashMap.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsHashSet.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsInlineAllocator.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsInlineAoS.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsInlineArray.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsIntrinsics.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsMathUtils.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsMutex.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsPool.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsSList.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsSocket.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsSort.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsSortInternals.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsString.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsSync.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsTempAllocator.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsThread.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsTime.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsUserAllocated.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsUtilities.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsVecMath.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsVecMathAoSScalar.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsVecMathAoSScalarInline.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsVecMathSSE.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsVecMathUtilities.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsVecQuat.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsVecTransform.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/Px.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxAllocatorCallback.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxProfiler.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxSharedAssert.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxBitAndData.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxBounds3.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxErrorCallback.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxErrors.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxFlags.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxIntrinsics.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxIO.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxMat33.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxMat44.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxMath.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxMemory.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxPlane.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxPreprocessor.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxQuat.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxSimpleTypes.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxStrideIterator.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxTransform.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxUnionCast.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxVec2.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxVec3.h;C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation/PxVec4.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/install/vc15win32/PxShared/include/foundation" TYPE FILE FILES
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/Px.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxAllocatorCallback.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxProfiler.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxSharedAssert.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxBitAndData.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxBounds3.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxErrorCallback.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxErrors.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxFlags.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxIntrinsics.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxIO.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxMat33.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxMat44.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxMath.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxMemory.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxPlane.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxPreprocessor.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxQuat.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxSimpleTypes.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxStrideIterator.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxTransform.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxUnionCast.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxVec2.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxVec3.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxVec4.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/gpu" TYPE FILE FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/gpu/PxGpu.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cudamanager" TYPE FILE FILES
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/cudamanager/PxCudaContextManager.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/cudamanager/PxCudaMemoryManager.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/common/windows" TYPE FILE FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/common/windows/PxWindowsDelayLoadHook.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysX_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysX_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysX_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysX_32.pdb")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxActor.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxAggregate.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxArticulationReducedCoordinate.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxArticulationBase.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxArticulation.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxArticulationJoint.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxArticulationJointReducedCoordinate.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxArticulationLink.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxBatchQuery.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxBatchQueryDesc.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxBroadPhase.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxClient.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxConstraint.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxConstraintDesc.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxContact.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxContactModifyCallback.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxDeletionListener.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxFiltering.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxForceMode.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxImmediateMode.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxLockedData.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxMaterial.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxPhysics.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxPhysicsAPI.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxPhysicsSerialization.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxPhysicsVersion.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxPhysXConfig.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxPruningStructure.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxQueryFiltering.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxQueryReport.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxRigidActor.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxRigidBody.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxRigidDynamic.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxRigidStatic.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxScene.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxSceneDesc.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxSceneLock.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxShape.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxSimulationEventCallback.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxSimulationStatistics.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxVisualizationParameter.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/common" TYPE FILE FILES
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/common/PxBase.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/common/PxCollection.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/common/PxCoreUtilityTypes.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/common/PxMetaData.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/common/PxMetaDataFlags.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/common/PxPhysicsInsertionCallback.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/common/PxPhysXCommonConfig.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/common/PxRenderBuffer.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/common/PxSerialFramework.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/common/PxSerializer.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/common/PxStringTable.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/common/PxTolerancesScale.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/common/PxTypeInfo.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/common/PxProfileZone.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pvd" TYPE FILE FILES
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/pvd/PxPvdSceneClient.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/pvd/PxPvd.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/pvd/PxPvdTransport.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/collision" TYPE FILE FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/collision/PxCollisionDefs.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/solver" TYPE FILE FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/solver/PxSolverDefs.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/PxConfig.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysXCharacterKinematic_static_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysXCharacterKinematic_static_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysXCharacterKinematic_static_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysXCharacterKinematic_static_32.pdb")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/characterkinematic" TYPE FILE FILES
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/characterkinematic/PxBoxController.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/characterkinematic/PxCapsuleController.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/characterkinematic/PxController.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/characterkinematic/PxControllerBehavior.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/characterkinematic/PxControllerManager.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/characterkinematic/PxControllerObstacles.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/characterkinematic/PxExtended.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysXCommon_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysXCommon_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysXCommon_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysXCommon_32.pdb")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/geometry" TYPE FILE FILES
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxBoxGeometry.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxCapsuleGeometry.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxConvexMesh.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxConvexMeshGeometry.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxGeometry.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxGeometryHelpers.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxGeometryQuery.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxHeightField.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxHeightFieldDesc.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxHeightFieldFlag.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxHeightFieldGeometry.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxHeightFieldSample.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxMeshQuery.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxMeshScale.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxPlaneGeometry.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxSimpleTriangleMesh.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxSphereGeometry.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxTriangle.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxTriangleMesh.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxTriangleMeshGeometry.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geometry/PxBVHStructure.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/geomutils" TYPE FILE FILES
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geomutils/GuContactBuffer.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/geomutils/GuContactPoint.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysXCooking_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysXCooking_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysXCooking_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysXCooking_32.pdb")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cooking" TYPE FILE FILES
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/cooking/PxBVH33MidphaseDesc.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/cooking/PxBVH34MidphaseDesc.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/cooking/Pxc.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/cooking/PxConvexMeshDesc.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/cooking/PxCooking.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/cooking/PxMidphaseDesc.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/cooking/PxTriangleMeshDesc.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/cooking/PxBVHStructureDesc.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysXExtensions_static_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysXExtensions_static_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysXExtensions_static_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysXExtensions_static_32.pdb")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/extensions" TYPE FILE FILES
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxBinaryConverter.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxBroadPhaseExt.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxCollectionExt.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxConstraintExt.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxContactJoint.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxConvexMeshExt.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxD6Joint.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxD6JointCreate.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxDefaultAllocator.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxDefaultCpuDispatcher.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxDefaultErrorCallback.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxDefaultSimulationFilterShader.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxDefaultStreams.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxDistanceJoint.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxContactJoint.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxExtensionsAPI.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxFixedJoint.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxJoint.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxJointLimit.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxMassProperties.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxPrismaticJoint.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxRaycastCCD.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxRepXSerializer.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxRepXSimpleType.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxRevoluteJoint.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxRigidActorExt.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxRigidBodyExt.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxSceneQueryExt.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxSerialization.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxShapeExt.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxSimpleFactory.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxSmoothNormals.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxSphericalJoint.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxStringTableExt.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/extensions/PxTriangleMeshExt.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/filebuf" TYPE FILE FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/filebuf/PxFileBuf.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysXVehicle_static_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysXVehicle_static_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysXVehicle_static_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysXVehicle_static_32.pdb")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/vehicle" TYPE FILE FILES
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleComponents.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleDrive.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleDrive4W.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleDriveNW.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleDriveTank.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleNoDrive.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleSDK.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleShaders.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleTireFriction.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleUpdate.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleUtil.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleUtilControl.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleUtilSetup.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleUtilTelemetry.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleWheels.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/source/fastxml/include" TYPE FILE FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/source/fastxml/include/PsFastXml.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysXPvdSDK_static_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysXPvdSDK_static_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysXPvdSDK_static_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysXPvdSDK_static_32.pdb")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysXTask_static_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysXTask_static_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysXTask_static_32.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE FILE OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysXTask_static_32.pdb")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/task" TYPE FILE FILES
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/task/PxCpuDispatcher.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/task/PxTask.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/task/PxTaskDefine.h"
    "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/include/task/PxTaskManager.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysXFoundation_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysXFoundation_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysXFoundation_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysXFoundation_32.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE SHARED_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysXFoundation_32.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE SHARED_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysXFoundation_32.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE SHARED_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysXFoundation_32.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE SHARED_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysXFoundation_32.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysX_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysX_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysX_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysX_32.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE SHARED_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysX_32.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE SHARED_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysX_32.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE SHARED_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysX_32.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE SHARED_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysX_32.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysXCharacterKinematic_static_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysXCharacterKinematic_static_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysXCharacterKinematic_static_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysXCharacterKinematic_static_32.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysXPvdSDK_static_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysXPvdSDK_static_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysXPvdSDK_static_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysXPvdSDK_static_32.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysXCommon_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysXCommon_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysXCommon_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysXCommon_32.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE SHARED_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysXCommon_32.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE SHARED_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysXCommon_32.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE SHARED_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysXCommon_32.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE SHARED_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysXCommon_32.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysXCooking_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysXCooking_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysXCooking_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysXCooking_32.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE SHARED_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysXCooking_32.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE SHARED_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysXCooking_32.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE SHARED_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysXCooking_32.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE SHARED_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysXCooking_32.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysXExtensions_static_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysXExtensions_static_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysXExtensions_static_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysXExtensions_static_32.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysXVehicle_static_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysXVehicle_static_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysXVehicle_static_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysXVehicle_static_32.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/debug" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/debug/PhysXTask_static_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/checked" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/checked/PhysXTask_static_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/profile" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/profile/PhysXTask_static_32.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_32.vc142.mt/release" TYPE STATIC_LIBRARY FILES "C:/Dev/InsightInteractive/HelixEngine/Engine/ThirdParty/PhysX-4.1/physx/bin/win.x86_32.vc142.mt/release/PhysXTask_static_32.lib")
  endif()
endif()


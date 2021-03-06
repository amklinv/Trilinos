
# @HEADER
#  ************************************************************************
#
#                    Trios: Trilinos I/O Support
#                  Copyright 2011 Sandia Corporation
#
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are
#  met:
#
#  1. Redistributions of source code must retain the above copyright
#  notice, this list of conditions and the following disclaimer.
#
#  2. Redistributions in binary form must reproduce the above copyright
#  notice, this list of conditions and the following disclaimer in the
#  documentation and/or other materials provided with the distribution.
#
#  3. Neither the name of the Corporation nor the names of the
#  contributors may be used to endorse or promote products derived from
#  this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
#  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
#  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Questions? Contact Ron A. Oldfield (raoldfi@sandia.gov)
#
#  *************************************************************************
# @HEADER


INCLUDE(TriosProcessXDR)

SET(NNTI_HEADERS "")
SET(NNTI_NONINSTALLHEADERS "")
SET(NNTI_SOURCES "")


############# COMMON ##########################

TRIBITS_SUBPACKAGE(nnti)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


############# NNTI ##########################

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_POSIX_C_SOURCE=200112L")

TriosProcessXDR(${CMAKE_CURRENT_SOURCE_DIR}/Trios_nnti_xdr.x)

APPEND_SET(NNTI_NOINSTHEADERS
  nnti_gni.h
  nnti_ib.h
  nnti_dcmf.h
  nnti_mpi.h
  nnti_internal.h
  nnti_ptls.h
  nnti_utils.h
  buffer_queue.h
)

APPEND_SET(NNTI_HEADERS
  ${CMAKE_CURRENT_BINARY_DIR}/Trios_nnti_xdr.h
  Trios_nnti_fprint_types.h
  Trios_nnti.h
)

APPEND_SET(NNTI_SOURCES
  ${CMAKE_CURRENT_BINARY_DIR}/Trios_nnti_xdr.c
  fprint_types.cpp
  nnti.c
  nnti_utils.c
  buffer_queue.cpp
)

SET(TRIOS_SUPPORTED_NETWORK_FOUND 0)
IF (${PACKAGE_NAME}_ENABLE_Portals OR ${PACKAGE_NAME}_ENABLE_CrayPortals)
  APPEND_SET(NNTI_SOURCES nnti_ptls.cpp)
  SET(TRIOS_SUPPORTED_NETWORK_FOUND 1)
ENDIF ()
IF (${PACKAGE_NAME}_ENABLE_InfiniBand)
  APPEND_SET(NNTI_SOURCES nnti_ib.cpp)
  SET(TRIOS_SUPPORTED_NETWORK_FOUND 1)
ENDIF ()
IF (${PACKAGE_NAME}_ENABLE_Gemini)
  APPEND_SET(NNTI_SOURCES nnti_gni.cpp)
  SET(TRIOS_SUPPORTED_NETWORK_FOUND 1)
ENDIF ()
IF (${PARENT_PACKAGE_NAME}_ENABLE_BGPDCMF)
    APPEND_SET(NNTI_SOURCES nnti_dcmf.cpp)
    SET(TRIOS_SUPPORTED_NETWORK_FOUND 1)
ENDIF ()
IF (${PARENT_PACKAGE_NAME}_ENABLE_BGQPAMI)
    APPEND_SET(NNTI_SOURCES nnti_pami.cpp)
    SET(TRIOS_SUPPORTED_NETWORK_FOUND 1)
ENDIF ()
IF (${PARENT_PACKAGE_NAME}_ENABLE_MPI)
  APPEND_SET(NNTI_SOURCES nnti_mpi.cpp)
  SET(TRIOS_SUPPORTED_NETWORK_FOUND 1)
ENDIF ()
IF (NOT TRIOS_SUPPORTED_NETWORK_FOUND)
   message(FATAL "Did not find a supported network protocol. ")
ENDIF ()

############# libtrios_nnti.a ##########################

TRIBITS_ADD_LIBRARY(
   trios_nnti
   HEADERS ${NNTI_HEADERS}
   NOINSTALLHEADERS ${NNTI_NOINSTALLHEADERS}
   SOURCES ${NNTI_SOURCES}
   DEPLIBS ${DEPLIBS}
)


TRIBITS_SUBPACKAGE_POSTPROCESS()

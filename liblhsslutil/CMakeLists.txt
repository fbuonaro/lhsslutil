cmake_minimum_required( VERSION 3.10 )

project( liblhsslutil )

# general flags for building successfully
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

# for debugging
set( LLHC_CXX_FLAGS_DBG "${LLHC_CXX_FLAGS_DBG} -Wall -Wextra -W" )
set( LLHC_CXX_FLAGS_DBG "${LLHC_CXX_FLAGS_DBG} -DDEBUG" )
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${LLHC_CXX_FLAGS_DBG}" )
set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${LLHC_CXX_FLAGS_DBG}" )

##############################################################
# library
##############################################################

set( LIB_SRC_DIR "src" )
set( LIB_INC_DIR "include" )

# pull in gtest
find_package( GTest REQUIRED )
if( NOT GTEST_FOUND )
    message( FATAL_ERROR "gtest could not be found" )
endif( NOT GTEST_FOUND )

# pull in crypto
find_package( OpenSSL 1.0 REQUIRED )
if( NOT OPENSSL_FOUND )
    message( FATAL_ERROR "openssl/crypto could not be found" )
endif( NOT OPENSSL_FOUND )


add_library( lhsslutil SHARED
    "${LIB_SRC_DIR}/base64.cxx"
    "${LIB_SRC_DIR}/hashwrapper.cxx"
    "${LIB_SRC_DIR}/hexutils.cxx"
    "${LIB_SRC_DIR}/opensslhash.cxx"
    "${LIB_SRC_DIR}/opensslprocesscontext.cxx"
    "${LIB_SRC_DIR}/opensslrandomnessgenerator.cxx"
    "${LIB_SRC_DIR}/randomnessgenerator.cxx" )

target_link_libraries( lhsslutil
    PUBLIC
        "${OPENSSL_CRYPTO_LIBRARY}" )

target_include_directories( lhsslutil
    PUBLIC
        "${OPENSSL_INCLUDE_DIR}"
        "${LIB_INC_DIR}/lhsslutil"
    PRIVATE
        "${LIB_INC_DIR}/lhsslutil_impl" )

##############################################################
# unit tests
##############################################################

set( TST_SRC_DIR "./test" )

enable_testing()

add_executable( testlhsslutil
    "${TST_SRC_DIR}/testbase64.cxx"
    "${TST_SRC_DIR}/testhashwrapper.cxx"
    "${TST_SRC_DIR}/testopensslprocesscontext.cxx"
    "${TST_SRC_DIR}/testrandomness.cxx" )

target_link_libraries( testlhsslutil
    PRIVATE
        "${GTEST_BOTH_LIBRARIES}"
        lhsslutil )

target_include_directories( testlhsslutil
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}"
        "${GTEST_INCLUDE_DIRS}"
        "${OPENSSL_INCLUDE_DIR}"
        "${LIB_INC_DIR}/lhsslutil"
        "${LIB_INC_DIR}/lhsslutil_impl" )

include( GoogleTest )
gtest_add_tests( TARGET testlhsslutil )

##############################################################
# installation
##############################################################

# library
install( TARGETS lhsslutil
         LIBRARY
            DESTINATION lib
            COMPONENT lhsslutil )

# include directory
install( DIRECTORY "${LIB_INC_DIR}/lhsslutil/lhsslutil"
         DESTINATION include
         COMPONENT lhsslutil
         FILES_MATCHING PATTERN "*.h" )

ADD_CUSTOM_TARGET( install-lhsslutil
                   ${CMAKE_COMMAND}
                   -D "CMAKE_INSTALL_COMPONENT=lhsslutil"
                   -P "cmake_install.cmake" )

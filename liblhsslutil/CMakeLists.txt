cmake_minimum_required( VERSION 3.10 )

project( liblhsslutil 
         DESCRIPTION "lhsslutil library"
         HOMEPAGE_URL "https://github.com/fbuonaro/lhsslutil/liblhsslutil"
         LANGUAGES CXX
         VERSION 1.0.0 )

##############################################################
# general
##############################################################

include( GNUInstallDirs )

set( CMAKE_CXX_STANDARD 11 )

add_compile_options( -Wall -Wextra -pedantic -Werror )

##############################################################
# library
##############################################################

# install directories
set( LH_INSTALL_BINDIR "${CMAKE_INSTALL_BINDIR}" )
set( LH_INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}/liblhsslutil_${PROJECT_VERSION}" )
set( LH_INSTALL_INCDIR "${CMAKE_INSTALL_INCLUDEDIR}/lhsslutil_${PROJECT_VERSION}" )

# pull in crypto
find_package( OpenSSL 1.0 REQUIRED )
if( NOT OPENSSL_FOUND )
    message( FATAL_ERROR "openssl/crypto could not be found" )
endif( NOT OPENSSL_FOUND )

# library name
set( LH_LIB_NAME lhsslutil )

# source files
set( LH_LIB_SRC_FILES 
    "src/base64.cxx"
    "src/hashwrapper.cxx"
    "src/hexutils.cxx"
    "src/opensslhash.cxx"
    "src/opensslprocesscontext.cxx"
    "src/opensslrandomnessgenerator.cxx"
    "src/randomnessgenerator.cxx" )
# library dependencies
set( LH_LIB_PUBLIC_LINKLIBS "${OPENSSL_CRYPTO_LIBRARY}" )
set( LH_LIB_PRIVATE_LINKLIBS )
# header dependencies
set( LH_LIB_PUBLIC_INCLUDES 
    "${OPENSSL_INCLUDE_DIR}"
     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/lhsslutil>        # build
     $<INSTALL_INTERFACE:${LH_INSTALL_INCDIR}> )                                # export
set( LH_LIB_PRIVATE_INCLUDES 
     "${CMAKE_CURRENT_SOURCE_DIR}/include/lhsslutil_impl"
     "${CMAKE_CURRENT_BINARY_DIR}" )

lh_add_library()

##############################################################
# unit tests
##############################################################

# pull in gtest
find_package( GTest REQUIRED )
if( NOT GTEST_FOUND )
    message( FATAL_ERROR "gtest could not be found" )
endif( NOT GTEST_FOUND )

add_executable( testlhsslutil
                "test/testbase64.cxx"
                "test/testhashwrapper.cxx"
                "test/testopensslprocesscontext.cxx"
                "test/testrandomness.cxx" )

target_link_libraries( testlhsslutil
                       PRIVATE
                           "${GTEST_BOTH_LIBRARIES}"
                           lhsslutil )

target_include_directories( testlhsslutil
                            PRIVATE
                                "${LH_LIB_PRIVATE_INCLUDES}"
                                "${CMAKE_CURRENT_BINARY_DIR}"
                                "${GTEST_INCLUDE_DIRS}" )

include( GoogleTest )
gtest_add_tests( TARGET testlhsslutil )

##############################################################
# installation
##############################################################

# name of component to install
set(LH_COMPONENT_NAME liblhsslutil )
# directories to install
set(LH_LIB_PUBLIC_INCLUDE_DIRS "include/lhsslutil/lhsslutil")

lh_add_install_library()

##############################################################
# pkg-config
##############################################################
set( PC_INCDIR "${LH_INSTALL_INCDIR}")
set( PC_LIBDIR "${LH_INSTALL_LIBDIR}")
set( PC_PUBREQS )
set( PC_PRIVREQS )
set( PC_CFLAGS )
set( PC_PUBLIBS "-llhsslutil" )
set( PC_PRIVLIBS )
set( PC_INSTALL_DIR "${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig" )
set( PC_INSTALL_FILENAME "${PROJECT_NAME}-${CMAKE_PROJECT_VERSION_MAJOR}.pc" )
set( PC_INSTALL_FILE "${PC_INSTALL_DIR}/${PC_INSTALL_FILENAME}" )
set( PC_RELOCATE_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/rpm_relocate_pc.sh" )

lh_add_pkgconfig()

##############################################################
# cmake packaging
##############################################################

lh_add_install_cmake_config()

##############################################################
# rpm packaging
##############################################################
 
set( CPACK_RPM_LIBLHSSLUTIL_PACKAGE_REQUIRES "openssl-devel >= 1.0.2" PARENT_SCOPE)

# to relocate the pc file prefix to /somewhere if 'rpm --install --prefix /somewhere'
set( CPACK_RPM_LIBLHSSLUTIL_PACKAGE_REQUIRES_POST "sed" PARENT_SCOPE) 
set( CPACK_RPM_LIBLHSSLUTIL_POST_INSTALL_SCRIPT_FILE "${PC_RELOCATE_SCRIPT}" PARENT_SCOPE)

set(LH_PACKAGE_DESC "liblhsslutil libraries package")

lh_add_cpack_component()

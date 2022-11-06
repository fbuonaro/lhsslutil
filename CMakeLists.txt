cmake_minimum_required( VERSION 3.10 )

project( lhsslutil 
         DESCRIPTION "lhsslutil library"
         HOMEPAGE_URL "https://github.com/fbuonaro/lhsslutil"
         LANGUAGES CXX
         VERSION 1.0.0 )


enable_testing()

add_subdirectory( liblhsslutil )

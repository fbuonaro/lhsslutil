#ifndef __LHSSLUTIL_HEXUTILS__
#define __LHSSLUTIL_HEXUTILS__

#include <string>

namespace LHSSLUtilImplNS
{

    unsigned char ByteForHexCharacter( char hex_char );

    // assume that bytes_in is length bytes long
    std::string BytesToHex( const unsigned char* bytes_in,
                            unsigned int length );

    void FillHexFromBytes( const unsigned char* bytes_in,
                           unsigned int length,
                           std::string& hex_out );

    void HexToBytes( unsigned char* bytes_out, const std::string& hex_in );
}

#endif

#ifndef __LHSSLUTIL_BASE64__
#define __LHSSLUTIL_BASE64__

#include <string>
#include <vector>

namespace LHSSLUtilNS
{
    // does not add padding
    int ConvertB64UrlStrToB64( const std::string& b64UrlStr, std::string& b64StrOut, int& paddingLengthOut );

    int DecodeB64UrlStr(
        const std::string& b64UrlEncodedStr,
        std::vector< unsigned char >& decodedStrOut );

    int DecodeB64Str(
        const std::string& b64EncodedStr,
        std::vector< unsigned char >& decodedStrOut,
        bool includeNewLines );

    int EncodeStrToB64( const std::string& str, std::string& b64EncodedStr, bool includeNewLines = false );
}

#endif
#include <gtest/gtest.h>
#include <lhsslutil/base64.h>

namespace TestLHSSLUtilNS
{
    namespace
    {
        int decodeB64( const std::string& in, std::string& out, bool includeNewLines = false )
        {
            int rc = 0;
            std::vector< unsigned char > vec;

            rc = LHSSLUtilNS::DecodeB64Str( in, vec, includeNewLines );
            if ( ( rc == 0 ) && vec.size() )
            {
                out.assign( reinterpret_cast<char*>( vec.data() ), vec.size() );
            }

            return rc;
        }
    }

    TEST( TestLHSSLUtil, TestEncodeStrToB641 )
    {
        int rc = 0;
        std::string inputToEncode;
        std::string encodedOutput;

        inputToEncode = "";
        rc = LHSSLUtilNS::EncodeStrToB64( inputToEncode, encodedOutput );
        ASSERT_EQ( 0, rc ) << "rc=" << rc << " for inputToEncode=[" << inputToEncode << "]";

        std::string decodedInput;
        rc = decodeB64( encodedOutput, decodedInput );
        ASSERT_EQ( 0, rc ) << "rc=" << rc << " for encodedOutput=[" << encodedOutput << "]";
        ASSERT_EQ( inputToEncode, decodedInput );
    }

    TEST( TestLHSSLUtil, TestEncodeStrToB642 )
    {
        int rc = 0;
        std::string inputToEncode;
        std::string encodedOutput;

        inputToEncode = "a";
        rc = LHSSLUtilNS::EncodeStrToB64( inputToEncode, encodedOutput );
        ASSERT_EQ( 0, rc ) << "rc=" << rc << " for inputToEncode=[" << inputToEncode << "]";

        std::string decodedInput;
        rc = decodeB64( encodedOutput, decodedInput );
        ASSERT_EQ( 0, rc ) << "rc=" << rc << " for encodedOutput=[" << encodedOutput << "]";
        ASSERT_EQ( inputToEncode, decodedInput );
    }

    TEST( TestLHSSLUtil, TestEncodeStrToB643 )
    {
        int rc = 0;
        std::string inputToEncode;
        std::string encodedOutput;

        inputToEncode = "abc";
        rc = LHSSLUtilNS::EncodeStrToB64( inputToEncode, encodedOutput );
        ASSERT_EQ( 0, rc ) << "rc=" << rc << " for inputToEncode=[" << inputToEncode << "]";

        std::string decodedInput;
        rc = decodeB64( encodedOutput, decodedInput );
        ASSERT_EQ( 0, rc ) << "rc=" << rc << " for encodedOutput=[" << encodedOutput << "]";
        ASSERT_EQ( inputToEncode, decodedInput );
    }

    TEST( TestLHSSLUtil, TestEncodeStrToB644 )
    {
        int rc = 0;
        std::string inputToEncode;
        std::string encodedOutput;

        inputToEncode = "abcd";
        rc = LHSSLUtilNS::EncodeStrToB64( inputToEncode, encodedOutput );
        ASSERT_EQ( 0, rc ) << "rc=" << rc << " for inputToEncode=[" << inputToEncode << "]";

        std::string decodedInput;
        rc = decodeB64( encodedOutput, decodedInput );
        ASSERT_EQ( 0, rc ) << "rc=" << rc << " for encodedOutput=[" << encodedOutput << "]";
        ASSERT_EQ( inputToEncode, decodedInput );
    }

    TEST( TestLHSSLUtil, TestEncodeStrToB645 )
    {
        int rc = 0;
        std::string inputToEncode;
        std::string encodedOutput;

        inputToEncode = "abcabcabcabcabcabc";
        rc = LHSSLUtilNS::EncodeStrToB64( inputToEncode, encodedOutput, true );
        ASSERT_EQ( 0, rc ) << "rc=" << rc << " for inputToEncode=[" << inputToEncode << "]";

        std::string decodedInput;
        rc = decodeB64( encodedOutput, decodedInput, true );
        ASSERT_EQ( 0, rc ) << "rc=" << rc << " for encodedOutput=[" << encodedOutput << "]";
        ASSERT_EQ( inputToEncode, decodedInput );
    }

    TEST( TestLHSSLUtil, TestEncodeStrToB646 )
    {
        int rc = 0;
        std::string inputToEncode;
        std::string encodedOutput;

        inputToEncode = "abcabcabcabcabcabcA";
        rc = LHSSLUtilNS::EncodeStrToB64( inputToEncode, encodedOutput, true );
        ASSERT_EQ( 0, rc ) << "rc=" << rc << " for inputToEncode=[" << inputToEncode << "]";

        std::string decodedInput;
        rc = decodeB64( encodedOutput, decodedInput, true );
        ASSERT_EQ( 0, rc ) << "rc=" << rc << " for encodedOutput=[" << encodedOutput << "]";
        ASSERT_EQ( inputToEncode, decodedInput );
    }

    TEST( TestLHSSLUtil, TestEncodeStrToB647 )
    {
        int rc = 0;
        std::string inputToEncode;
        std::string encodedOutput;

        inputToEncode = "abcabcabcabcabcabcAB";
        rc = LHSSLUtilNS::EncodeStrToB64( inputToEncode, encodedOutput, true );
        ASSERT_EQ( 0, rc ) << "rc=" << rc << " for inputToEncode=[" << inputToEncode << "]";

        std::string decodedInput;
        rc = decodeB64( encodedOutput, decodedInput, true );
        ASSERT_EQ( 0, rc ) << "rc=" << rc << " for encodedOutput=[" << encodedOutput << "]";
        ASSERT_EQ( inputToEncode, decodedInput );
    }
}
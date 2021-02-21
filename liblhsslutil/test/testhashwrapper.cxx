#include <gtest/gtest.h>

#include <memory>
#include <stdio.h>
#include <string>

#include <lhsslutil/hashwrapper.h>
#include <lhsslutil/opensslhash.h>

#define TEST_STRING "1234567890abcdefghij"
#define TEST_HASH_1 98B7401940A4725F8B200E9FFD8E7F6B63E211CA5C87E7C41E28B4C4D93AF1AD93A60777FB6065B01F6DA7CF50EB4B48E2F532E4B4118754375B3D60EABB2A1D
#define TEST_HASH_1_BYTES 0x98,0xB7,0x40,0x19,0x40,0xA4,0x72,0x5F,0x8B,0x20,0x0E,0x9F,0xFD,0x8E,0x7F,0x6B,0x63,0xE2,0x11,0xCA,0x5C,0x87, 0xE7, 0xC4, 0x1E,0x28,0xB4,0xC4,0xD9,0x3A,0xF1,0xAD,0x93,0xA6,0x07,0x77,0xFB,0x60,0x65,0xB0,0x1F,0x6D,0xA7,0xCF,0x50,0xEB,0x4B,0x48,0xE2,0xF5,0x32,0xE4,0xB4,0x11,0x87,0x54,0x37,0x5B,0x3D,0x60,0xEA,0xBB,0x2A,0x1D
#define XSTR( y ) #y
#define STR( x ) XSTR( x )

namespace LHSSLUtilTestNS
{
    class HashWrapperTest : public ::testing::Test
    {
        protected:
            virtual void SetUp()
            {   
                hash0 = std::unique_ptr<LHSSLUtilNS::OpenSSLHash>( new LHSSLUtilNS::OpenSSLHash() );
            }

            virtual void TearDown()
            {

            }

            std::unique_ptr<LHSSLUtilNS::OpenSSLHash> hash0;
            LHSSLUtilNS::OpenSSLHash hash1;
            LHSSLUtilNS::OpenSSLHash hash2;
            LHSSLUtilNS::OpenSSLHash hash3;
            LHSSLUtilNS::OpenSSLHash hash4;
            LHSSLUtilNS::OpenSSLHash hash5;
            LHSSLUtilNS::OpenSSLHash hash6;
            LHSSLUtilNS::OpenSSLHash hash7;
            LHSSLUtilNS::OpenSSLHash hash8;
    };

    TEST_F( HashWrapperTest, TestBasic1 ) 
    {
        const char* chars = "1234567890";

        ASSERT_EQ( NULL, hash1.GetHashValue() ) << "Hash 1 is not empty going into test";

        hash1.UpdateHash( reinterpret_cast<const void*>( chars ), 10 );
        hash1.FinalizeHash();
        ASSERT_TRUE( NULL != hash1.GetHashValue() ) << "Hash 1 is empty after Finalizing";

        hash1.RestartHash();
        ASSERT_EQ( NULL, hash1.GetHashValue() ) << "Hash 1 is not empty after Restarting";
    }

    TEST_F( HashWrapperTest, TestBasic2 ) 
    {   
        const char* alpha_chars = "1234567890";
        const char* numer_chars = "abcdefghij";
        std::unique_ptr<LHSSLUtilNS::HashWrapper> abstract_hash = nullptr;

        ASSERT_EQ( NULL, hash2.GetHashValue() ) << "Hash 2 is not empty going into test";

        hash2.UpdateHash( reinterpret_cast<const void*>( alpha_chars ), 10 );
        hash2.UpdateHash( reinterpret_cast<const void*>( numer_chars ), 10 );
           
        ASSERT_TRUE( NULL != hash2.GetFinalHashValue() )
            << "Hash 2 is empty after GetFinalHashValue";

        abstract_hash = std::unique_ptr<LHSSLUtilNS::HashWrapper>(
                            static_cast<LHSSLUtilNS::HashWrapper*>(
                                new LHSSLUtilNS::OpenSSLHash( hash2 ) ) );

        ASSERT_TRUE( abstract_hash->GetHashValue() != NULL )
            << "abstract_hash is empty after copy constructor";

        ASSERT_EQ( 0,
                   memcmp(
                    abstract_hash->GetHashValue(),
                    hash2.GetHashValue(),
                    hash2.GetHashLength() ) )
            << "abstract_hash copy is not the same as Hash 2";
    }

    // make sure that hashing works by verifying that the small updates are equivalent to big updates
    TEST_F( HashWrapperTest, TestBasic3 ) 
    {   
        const char* alpha_chars = "1234567890";
        const char* numer_chars = "abcdefghij";
        const char* alphanumer_chars = "1234567890abcdefghij";

        ASSERT_EQ( NULL, hash3.GetHashValue() ) << "Hash 3 is not empty going into test";
        ASSERT_EQ( NULL, hash4.GetHashValue() ) << "Hash 4 is not empty going into test";

        hash3.UpdateHash( reinterpret_cast<const void*>( alpha_chars ), 10 );
        hash3.UpdateHash( reinterpret_cast<const void*>( numer_chars ), 10 );
        hash4.UpdateHash( reinterpret_cast<const void*>( alphanumer_chars ), 20 );
           
        ASSERT_TRUE( NULL != hash3.GetFinalHashValue() ) << "Hash 3 is empty after GetFinalHashValue";
        ASSERT_TRUE( NULL != hash4.GetFinalHashValue() ) << "Hash 4 is empty after GetFinalHashValue";

        ASSERT_TRUE( hash3 == hash4 ) << "Hash 3 is not the same as Hash 4";
        ASSERT_TRUE( hash3.HexString() ==
                     std::string( STR( TEST_HASH_1 ) ) ) << "Hash 3<" << hash3.HexString()
                                                         << "> is not the same as TEST_HASH_1<"
                                                         << std::string( STR( TEST_HASH_1 ) ) << ">";
    }

    // tests the ability to create valid hashes from base 16 strings or raw bytes and ensures that the resulting
    // hash is equivalent to the hash of the preimage
    // Also tests the move constructor and move assignmnet
    TEST_F( HashWrapperTest, TestBasic4 ) 
    {   
        unsigned char bytes[] = {TEST_HASH_1_BYTES};

        ASSERT_EQ( NULL, hash5.GetHashValue() ) << "Hash 5 is not empty going into test";
        ASSERT_EQ( NULL, hash6.GetHashValue() ) << "Hash 6 is not empty going into test";
        ASSERT_EQ( NULL, hash7.GetHashValue() ) << "Hash 7 is not empty going into test";
        ASSERT_EQ( NULL, hash8.GetHashValue() ) << "Hash 7 is not empty going into test";

        hash5 = LHSSLUtilNS::OpenSSLHash( std::string( STR( TEST_HASH_1 ) ) );
        hash8 = LHSSLUtilNS::OpenSSLHash();
        hash8.FromHexString( std::string( STR( TEST_HASH_1 ) ) );
        hash6.FromBytes( bytes, 64 );
        hash7.UpdateHash( reinterpret_cast<const void*>( TEST_STRING ), 20 );
        hash7.FinalizeHash();
           
        ASSERT_TRUE( NULL != hash5.GetHashValue() ) << "Hash 5 is empty after GetFinalHashValue";
        ASSERT_TRUE( NULL != hash6.GetHashValue() ) << "Hash 6 is empty after GetFinalHashValue";
        ASSERT_TRUE( NULL != hash7.GetHashValue() ) << "Hash 7 is empty after GetFinalHashValue";
        ASSERT_TRUE( NULL != hash8.GetHashValue() ) << "Hash 8 is empty after GetFinalHashValue";

        ASSERT_TRUE( hash8 == hash5 ) << "Hash 5 is not the same as Hash 7";
        ASSERT_TRUE( hash8 == hash6 ) << "Hash 8 is not the same as Hash 6";
        ASSERT_TRUE( hash6 == hash7 ) << "Hash 6<" << hash6.HexString()
                                      << "> is not the same as Hash 7<"
                                      << hash7.HexString() << ">";
    }

    TEST_F( HashWrapperTest, TestBasic5 )
    {   
        unsigned char bytes[64] = {0};
        hash6.FromBytes( bytes, 64 );

        ASSERT_TRUE( !( hash6 == hash5 ) ) << "Hash 6 ( empty hashed ) is different from Hash 5 ( unhashed )";
        ASSERT_TRUE( hash7 == hash5 ) << "Expected two unhashed hashes to be equal but they were not!"; 

        hash5.FinalizeHash();
        hash7.FinalizeHash();
        ASSERT_TRUE( hash5 == hash7 ) << "Two dummy hashes are not equal!" ;
    }

    TEST_F( HashWrapperTest, TestBasic6 )
    {
        LHSSLUtilNS::OpenSSLHash copied_hash = hash1;
        LHSSLUtilNS::OpenSSLHash copied_hash2;
        std::string copied_hash2_str;
        std::string hash7_str;

        ASSERT_TRUE( copied_hash == hash1 )
            << "Copiend Empty hash not equal to empty hash it was just copied from";
        ASSERT_TRUE( copied_hash == copied_hash2 )
            << "Empty hash copied from another empty hash not equal to default hash";


        hash7.UpdateHash( reinterpret_cast<const void*>( TEST_STRING ), 20 );
        hash7.FinalizeHash();
        copied_hash2 = hash7;
        ASSERT_TRUE( copied_hash2 == hash7 )
            << "Non-empty copied hash not equal to the hash it was just copied from";

        copied_hash2_str = copied_hash2.HexString();
        hash7_str = hash7.HexString();

        ASSERT_TRUE( copied_hash2_str == hash7_str ) << "String equivalents are not equal!";
    }
}

#include <memory>
#include <stdio.h>
#include <string>

#include <gtest/gtest.h>

#include <lhsslutil/opensslprocesscontext.h>
#include <lhsslutil/opensslrandomnessgenerator.h>


namespace LHSSLUtilTestNS
{
    class TestRandomnessGenerator : public ::testing::Test
    {
        protected:
            virtual void SetUp()
            {   
                maxLength = 512;
                randGen = 
                    std::unique_ptr<LHSSLUtilNS::OpenSSLRandomnessGenerator>(
                        new LHSSLUtilNS::OpenSSLRandomnessGenerator( maxLength ) );
            }

            virtual void TearDown()
            {
            }

            std::unique_ptr<LHSSLUtilNS::OpenSSLRandomnessGenerator> randGen;
            unsigned int maxLength;
    };

    TEST_F( TestRandomnessGenerator, TestBasic01 ) 
    {
        std::string out;

        LHSSLUtilNS::OpenSSLProcessContext::ProcessInitialize( LHSSLUtilNS::OpenSSLModule::Randomness );

        ASSERT_TRUE( randGen->MaxLength() == maxLength )
            << "Randomness has incorrect max length";
        ASSERT_TRUE( randGen->GenerateRandomnessAsHexStringInto( maxLength+1, out ) != 0 )
            << "GenerateRandomnessAsHexStringInto succeeded!";
        ASSERT_TRUE( out.size() == 0 ) << "Randomness larger than max size generated!";

        LHSSLUtilNS::OpenSSLProcessContext::ProcessCleanup();
    }

    TEST_F( TestRandomnessGenerator, TestBasic02 ) 
    {
        std::string curr;
        std::string prev;

        LHSSLUtilNS::OpenSSLProcessContext::ProcessInitialize( LHSSLUtilNS::OpenSSLModule::Randomness );
        
        ASSERT_EQ( 0,
                   randGen->GenerateRandomnessAsHexStringInto( maxLength, prev ) )
            << "GenerateRandomnessAsHexStringInto failed";
        ASSERT_EQ( maxLength*2, prev.size() )
            << "Size of randmoness not as expected " << prev.size() << " vs " << maxLength * 2;

        for( unsigned int i = 0; i < 2*maxLength; ++i )
        {
            bool different = true;

            ASSERT_EQ( 0, randGen->GenerateRandomnessAsHexStringInto( maxLength, curr ) )
                << "GenerateRandomnessAsHexStringInto failed";
            ASSERT_EQ( maxLength*2, curr.size() )
                << "Size of randmoness not as expected "
                << prev.size() << " vs " << maxLength * 2;
            
            different = ( prev != curr );

            ASSERT_TRUE( different ) << "Same randomness generated twice in a row " << i;

            if( !different )
                break;

            prev = curr;
        }

        LHSSLUtilNS::OpenSSLProcessContext::ProcessCleanup();
    }
        
    TEST_F( TestRandomnessGenerator, TestBasic03 ) 
    {
        std::vector< unsigned char > prevBuffer;
        std::vector< unsigned char > currBuffer;

        LHSSLUtilNS::OpenSSLProcessContext::ProcessInitialize( LHSSLUtilNS::OpenSSLModule::Randomness );

        prevBuffer.resize( maxLength, '\0' );
        currBuffer.resize( maxLength, '\0' );

        ASSERT_EQ( 0, randGen->GenerateRandomness( maxLength, &prevBuffer[ 0 ] ) )
            << "GenerateRandomness failed";
        LHSSLUtilNS::OpenSSLRandomnessGenerator::ReseedFrom(
            maxLength,
            &prevBuffer[ 0 ] );
        

        for( unsigned int i = 0; i < 2*maxLength; ++i )
        {
            bool different = true;

            ASSERT_EQ( 0, randGen->GenerateRandomness( maxLength, &currBuffer[ 0 ] ) )
                << "GenerateRandomness failed";
            
            different = ( prevBuffer != currBuffer );

            ASSERT_TRUE( different ) << "Same randomness generated twice in a row " << i;

            if( !different )
                break;

            prevBuffer = currBuffer;
        }

        LHSSLUtilNS::OpenSSLProcessContext::ProcessCleanup();
    }
}

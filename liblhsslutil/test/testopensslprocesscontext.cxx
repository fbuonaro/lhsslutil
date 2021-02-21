#include <memory>
#include <stdio.h>
#include <string>

#include <gtest/gtest.h>

#include <lhsslutil/opensslprocesscontext.h>

namespace LHSSLUtilTestNS
{
    class TestProcessContext : public ::testing::Test
    {
        protected:
            virtual void SetUp()
            {   
            }

            virtual void TearDown()
            {
            }
    };

    TEST_F( TestProcessContext, TestBasic00 )
    {
        bool throwDoubleInit = false;
        bool throwDoubleClean = false;

        
        try
        {
            LHSSLUtilNS::OpenSSLProcessContext::ProcessInitialize(
                LHSSLUtilNS::OpenSSLModule::Randomness );
        }
        catch( std::exception e )
        {
            ASSERT_TRUE( false )
                << "received exception on first initialize< " << e.what() << " >";
        }

        try
        {
            LHSSLUtilNS::OpenSSLProcessContext::ProcessInitialize(
                LHSSLUtilNS::OpenSSLModule::Randomness );
        }
        catch( ... )
        {
            throwDoubleInit = true;
        }

        ASSERT_TRUE( throwDoubleInit );

        try
        {
            LHSSLUtilNS::OpenSSLProcessContext::ProcessCleanup();
        }
        catch( std::exception e )
        {
            ASSERT_TRUE( false )
                << "received exception on first cleanup< " << e.what() << " >";
        }

        try
        {
            LHSSLUtilNS::OpenSSLProcessContext::ProcessCleanup();
        }
        catch( ... )
        {
            throwDoubleClean = true;
        }

        ASSERT_TRUE( !throwDoubleClean );

        try
        {
            LHSSLUtilNS::OpenSSLProcessContext::ProcessInitialize(
                LHSSLUtilNS::OpenSSLModule::Randomness );
        }
        catch( std::exception e )
        {
            ASSERT_TRUE( false )
                << "received exception on second initialize< " << e.what() << " >";
        }

        try
        {
            LHSSLUtilNS::OpenSSLProcessContext::ProcessCleanup();
        }
        catch( std::exception e )
        {
            ASSERT_TRUE( false )
                << "received exception on second cleanup< " << e.what() << " >";
        }
    }
}

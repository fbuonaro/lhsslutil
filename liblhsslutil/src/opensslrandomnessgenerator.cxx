#include <cstddef>

#include <openssl/err.h>
#include <openssl/rand.h>

#include <lhsslutil/opensslrandomnessgenerator.h>
#include <lhsslutil_impl/hexutils.h>

namespace LHSSLUtilNS
{
    bool OpenSSLRandomnessGenerator::processInitialized = false;

    void OpenSSLRandomnessGenerator::ProcessInitialize()
    {
        if( !processInitialized )
        {
            RAND_poll();
            processInitialized = true;
        }
    }

    void OpenSSLRandomnessGenerator::ProcessCleanup()
    {
        if( processInitialized )
        {
            RAND_cleanup();
            processInitialized = false;
        }
    }

    const char* OpenSSLStaleRandomness::what() const throw()
    {
        return "OpenSSLRandomnessGenerator instance in need of reseeding";
    }

    OpenSSLRandomnessGenerator::OpenSSLRandomnessGenerator( unsigned int _maxLength )
    :   RandomnessGenerator( _maxLength )
    ,   buffer()
    ,   bufferStart( nullptr )
    {
        buffer.resize( maxLength, '\0' );
        bufferStart = &buffer[ 0 ];
    }

    OpenSSLRandomnessGenerator::~OpenSSLRandomnessGenerator()
    {
    }

    int OpenSSLRandomnessGenerator::GenerateRandomness( unsigned int ulength,
                                                        unsigned char* ubuffer )
    {
        int ret = 0;

        if( !( generatorWorking() ) )
            throw OpenSSLStaleRandomness();
        else if( ulength > maxLength )
            ret = 1;
        else
        {
            ret = RAND_bytes( ubuffer, ulength );
            if( ret == 0 )
                ret = ERR_get_error();
            else
                ret = 0;
        }

        return ret;
    }

    //length is number of random bytes, string will be twice that size
    std::string OpenSSLRandomnessGenerator::GenerateRandomnessAsHexString( unsigned int length )
    {
        std::string out;
        int ret = 0;

        if( !( generatorWorking() ) )
            throw OpenSSLStaleRandomness();

        if( length > maxLength )
            return out;

        ret = RAND_bytes( bufferStart, length );
        if( ret == 1 )
        {
            LHSSLUtilImplNS::FillHexFromBytes( bufferStart, length, out );
            ret = 0;
        }
        else
            ret = ERR_get_error();

        return out;
    }

    int OpenSSLRandomnessGenerator::GenerateRandomnessAsHexStringInto( unsigned int length,
                                                                       std::string& out )
    {
        int ret = 0;

        if( !( generatorWorking() ) )
            throw OpenSSLStaleRandomness();
        else if( length > maxLength )
            ret = 1;
        else
        {
            ret = RAND_bytes( bufferStart, length );
            if( ret == 1 )
            {
                LHSSLUtilImplNS::FillHexFromBytes( bufferStart, length, out );
                ret = 0;
            }
            else
                ret = ERR_get_error();
        }

        return ret;
    }

    int OpenSSLRandomnessGenerator::ReseedFrom( unsigned int reseedLength,
                                                const void* reseedBuffer )
    {
        RAND_seed( reseedBuffer, reseedLength );
        return 0;
    }

    bool OpenSSLRandomnessGenerator::generatorWorking()
    {
        return ( RAND_status() == 1 );
    }
}

#ifndef __LHSSLUTIL_OPENSSLRANDOMNESS_H__
#define __LHSSLUTIL_OPENSSLRANDOMNESS_H__

#include <stdexcept>
#include <vector>

#include <lhsslutil/randomnessgenerator.h>

namespace LHSSLUtilNS
{
    struct OpenSSLStaleRandomness : public std::exception
    {
        virtual const char* what() const throw();
    };

    class OpenSSLRandomnessGenerator : public RandomnessGenerator
    {
        public:
            OpenSSLRandomnessGenerator( unsigned int maxLength );
            ~OpenSSLRandomnessGenerator();

            int GenerateRandomness( unsigned int length, unsigned char* buffer );
            std::string GenerateRandomnessAsHexString( unsigned int length );
            int GenerateRandomnessAsHexStringInto( unsigned int length, std::string& out );

            // Static Singleton API - not thread safe
            static int ReseedFrom( unsigned int reseedLength, const void* reseedBuffer );
            static void ProcessInitialize();
            static void ProcessCleanup();

        private:
            bool generatorWorking();

            std::vector< unsigned char > buffer;
            unsigned char* bufferStart;

            static bool processInitialized;

    };
}

#endif

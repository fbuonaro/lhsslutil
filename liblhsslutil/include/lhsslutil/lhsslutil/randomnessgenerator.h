#ifndef __LHSSLUTIL_RANDOMNESS_H__
#define __LHSSLUTIL_RANDOMNESS_H__

#include <string>

namespace LHSSLUtilNS
{
    class RandomnessGenerator
    {
        public:
            RandomnessGenerator( unsigned int maxLength );
            virtual ~RandomnessGenerator();

            virtual int GenerateRandomness( unsigned int length, unsigned char* buffer ) = 0;
            virtual std::string GenerateRandomnessAsHexString( unsigned int length ) = 0;
            virtual int GenerateRandomnessAsHexStringInto( unsigned int length, std::string& out ) = 0;

            inline unsigned int MaxLength()
            {
                return maxLength;
            };

        protected:
            unsigned int maxLength;
    };
}

#endif

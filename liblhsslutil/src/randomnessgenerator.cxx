#include <stdexcept>

#include <lhsslutil/randomnessgenerator.h>

namespace LHSSLUtilNS
{
    RandomnessGenerator::RandomnessGenerator( unsigned int _maxLength )
    :   maxLength( _maxLength )
    {
        if( maxLength <= 0 )
        {
            throw std::runtime_error( "maxLength cannot be less than or equal to 0" );
        }
    }

    RandomnessGenerator::~RandomnessGenerator()
    {
    }
}

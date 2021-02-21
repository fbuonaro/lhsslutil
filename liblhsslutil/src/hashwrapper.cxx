#include <lhsslutil/hashwrapper.h>
#include <cstring>

namespace
{
    const char hex_digits[] = { '0', '1', '2', '3',
                                '4', '5', '6', '7',
                                '8', '9', 'A', 'B',
                                'C', 'D', 'E', 'F' };
}

namespace LHSSLUtilNS
{
    HashWrapper::HashWrapper()
    :   length( 0 ) 
    {
    }

    HashWrapper::HashWrapper( unsigned int _length )
    :   length( _length ) 
    {
    }

    HashWrapper::~HashWrapper()
    {
    }

    unsigned int HashWrapper::GetHashLength() const
    {
        return length;
    }

    //The rules are as follows:
    //  something which is not hashed is always less than somehting that is hashed
    //  something with a smaller hash length is less than something with a larger hash length
    //  something whcih is hashed and has the same hash length as another hashed hash is less than
    //      the other hash only if memcmp( hash, another_hash ) < 0
    bool HashWrapper::operator ==( const HashWrapper& r ) const
    {
        
        return      ( this == &r )
                ||  (   GetHashValue() == r.GetHashValue() )
                ||  (       ( length == r.GetHashLength() ) 
                        &&  GetHashValue()
                        &&  r.GetHashValue() 
                        &&  ( memcmp( GetHashValue() , r.GetHashValue(), length ) == 0 ) );
    }

    //Something not hashed is less than something that is hashed
    bool HashWrapper::operator<( const HashWrapper& r ) const
    {   
        return      ( !GetHashValue() && r.GetHashValue() )
                ||  ( length < r.GetHashLength() )
                ||  (       GetHashValue()
                        &&  r.GetHashValue()
                        &&  ( length == r.GetHashLength() ) 
                        &&  ( memcmp( GetHashValue() , r.GetHashValue(), r.GetHashLength() ) < 0 ) );
    }

    bool HashWrapper::operator >( const HashWrapper& r ) const
    {
        return      ( GetHashValue() && !r.GetHashValue() )
                ||  ( length > r.GetHashLength() )
                ||  (       GetHashValue()
                        &&  r.GetHashValue()
                        &&  ( length == r.GetHashLength() )
                        &&  ( memcmp( GetHashValue() , r.GetHashValue(), r.GetHashLength() ) > 0 ) );
     }
}

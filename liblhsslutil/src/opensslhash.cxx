#include <lhsslutil/opensslhash.h>
#include <lhsslutil_impl/hexutils.h>
#include <string.h>

#define CHUNCK_SIZE 1024

namespace LHSSLUtilNS
{
    void OpenSSLHash::hardRestartHash()
    {
        if( ! hash_provider )
        {
            hash_provider = default_provider();
        }

        EVP_MD_CTX_init( &hash_context );
        EVP_DigestInit_ex( &hash_context, hash_provider, NULL );
        length = EVP_MD_size( hash_provider );
        hashed = false;
    }

    void OpenSSLHash::FromBytes( const unsigned char* bytes_in, unsigned int _length )
    {
        hardRestartHash();

        if( _length <= EVP_MAX_MD_SIZE ) 
        {   length = _length;
            memcpy( hash_value, bytes_in, _length );
            LHSSLUtilImplNS::FillHexFromBytes( hash_value, length, hex_string );
            hashed = true;
        }
    }

    void OpenSSLHash::FromHexString( const std::string& hex_in ) 
    {
        hardRestartHash();

        if( ( hex_in.length()>>1 ) <= EVP_MAX_MD_SIZE )
        {   length = hex_in.length()>>1;
            LHSSLUtilImplNS::HexToBytes( hash_value, hex_in ); // length should already be set
            hex_string = hex_in;
            hashed = true;
        }
    }

    OpenSSLHash::OpenSSLHash( std::istream& input_byte_stream ) 
    :   HashWrapper::HashWrapper( 0 )
    ,   hash_provider( nullptr )
    {
        char chunk[CHUNCK_SIZE] = { 0 };
        std::istream::pos_type size = 0;
        std::istream::pos_type bytes_read = 0;

        hardRestartHash();

        if( input_byte_stream.good() ) 
        {
            input_byte_stream.seekg( 0, input_byte_stream.end );
            size = input_byte_stream.tellg();
            input_byte_stream.seekg( 0, input_byte_stream.beg );

            // read the first chunk
            do 
            {
                input_byte_stream.read( chunk, CHUNCK_SIZE );
                bytes_read = input_byte_stream.gcount();
                UpdateHash( chunk, bytes_read );
                size = size - bytes_read; 
            } while ( size > 0 );
        }

        FinalizeHash();
    }


    OpenSSLHash::OpenSSLHash( const unsigned char* bytes_in, unsigned int _length ) 
    :   HashWrapper::HashWrapper( _length )
    ,   hash_provider( nullptr )
    {   FromBytes( bytes_in, _length );
    }

    OpenSSLHash::OpenSSLHash( const std::string& hex_in ) 
    :   HashWrapper::HashWrapper( hex_in.length()>>1 ) 
    ,   hash_provider( nullptr )
    {
        FromHexString( hex_in );
    }

    OpenSSLHash::OpenSSLHash( const EVP_MD* _hash_provider ) 
    :   hash_provider( _hash_provider )
    {
        hardRestartHash();
    }

    OpenSSLHash::~OpenSSLHash()
    {
        EVP_MD_CTX_cleanup( &hash_context );
    }

    OpenSSLHash::OpenSSLHash( const OpenSSLHash& lother ) 
    :   HashWrapper( lother.length ) 
    {
        EVP_MD_CTX_copy( &hash_context, &lother.hash_context );
        hashed = lother.hashed;
        memcpy( hash_value, lother.hash_value, length );
        hash_provider = lother.hash_provider;
        hex_string = lother.hex_string;
    }

    OpenSSLHash::OpenSSLHash()
    :   hash_provider( nullptr )
    {
        hardRestartHash();
    }

    OpenSSLHash::OpenSSLHash( OpenSSLHash&& rother ) 
    {
        length = rother.length;

        hash_context = rother.hash_context;

        //TODO - not sure if this is safe considering that the destructor of other will
        //       eventually call EVP_MD_CTX_cleanup on its own hash_context
        memset( &rother.hash_context,0, sizeof( EVP_MD_CTX ) );

        hashed = rother.hashed;

        memcpy( hash_value, rother.hash_value, length );

        hash_provider = rother.hash_provider;

        hex_string = rother.hex_string;
    }

    OpenSSLHash& OpenSSLHash::operator=( OpenSSLHash other ) 
    {
        other.swap( *this );
        return *this;
    }

    void OpenSSLHash::swap( OpenSSLHash& b ) 
    {
        EVP_MD_CTX tempC;
        const EVP_MD* temp_provider;

        EVP_MD_CTX_copy( &tempC, &hash_context );
        EVP_MD_CTX_copy( &hash_context, &b.hash_context );
        EVP_MD_CTX_copy( &b.hash_context, &tempC );

        temp_provider = hash_provider;
        hash_provider = b.hash_provider;
        b.hash_provider = temp_provider;
         
        std::swap<unsigned int>( length, b.length );
        std::swap<bool>( hashed, b.hashed );
        std::swap<unsigned char, EVP_MAX_MD_SIZE>( hash_value, b.hash_value );
        std::swap<std::string>( hex_string, b.hex_string );

        EVP_MD_CTX_cleanup( &tempC );
    }

    int OpenSSLHash::ReadFinalHashValue( void* bytes )
    {
        if( ! hashed )
        {
            FinalizeHash();
        }

        memcpy( bytes, reinterpret_cast<const void*>( hash_value ), length );

        return length;
    }

    int OpenSSLHash::ReadHashValue( void* bytes ) const
    {
        int length_written = 0;

        if( hashed )
        {
            length_written = length;
            memcpy( bytes, reinterpret_cast<const void*>( hash_value ), length_written );
        }

        return length_written;
    }



    const unsigned char* OpenSSLHash::GetFinalHashValue()
    {
        if( ! hashed )
        {
            FinalizeHash();
        }

        return hash_value;
    }

    const unsigned char* OpenSSLHash::GetHashValue() const
    {
        const unsigned char* out = NULL;

        if( hashed )
        {
            out = hash_value;
        }

        return out;
    }

    void OpenSSLHash::UpdateHash( const void* bytes, int len_bytes )
    {
        EVP_DigestUpdate( &hash_context, bytes, len_bytes );

        hashed = false;
    }

    int OpenSSLHash::FinalizeHash()
    {
        EVP_DigestFinal_ex( &hash_context, hash_value, &length );

        LHSSLUtilImplNS::FillHexFromBytes( hash_value, length, hex_string );

        hashed = true;

        return 0;
    }

    void OpenSSLHash::RestartHash()
    {
        hardRestartHash();
    }

    const EVP_MD* OpenSSLHash::default_provider() const
    {
        return EVP_sha512();
    }


    const std::string& OpenSSLHash::HexString() const
    {
        return hex_string;
    }
}

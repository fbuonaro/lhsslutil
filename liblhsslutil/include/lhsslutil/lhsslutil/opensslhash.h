#ifndef __LHSSLUTIL_OPENSSLHASH__
#define __LHSSLUTIL_OPENSSLHASH__

#include <istream>
#include <openssl/evp.h>

#include <lhsslutil/hashwrapper.h>

namespace LHSSLUtilNS
{
    class OpenSSLHash : public HashWrapper
    {
        public:
            OpenSSLHash();
            ~OpenSSLHash();

            OpenSSLHash( std::istream& input_byte_stream );  
            OpenSSLHash( const unsigned char* bytes_in, unsigned int _length );
            OpenSSLHash( const std::string& hex_in );
            OpenSSLHash( const EVP_MD* hash_provider );
            OpenSSLHash( const OpenSSLHash& lother );
            OpenSSLHash( OpenSSLHash&& rother );
            OpenSSLHash& operator=( OpenSSLHash other );

            void swap( OpenSSLHash& b );

            int ReadFinalHashValue( void* out_bytes );
            const unsigned char* GetFinalHashValue();

            int ReadHashValue( void* out_bytes ) const;
            const unsigned char* GetHashValue() const;

            void UpdateHash( const void* bytes, int len_bytes );
            int FinalizeHash();
            void RestartHash();

            const std::string& HexString() const;

            void FromHexString( const std::string& hex_in );
            void FromBytes( const unsigned char*, unsigned int _length );
     
        private:
            void hardRestartHash();
            const EVP_MD* default_provider() const;

            bool hashed;
            const EVP_MD* hash_provider;
            unsigned char hash_value[ EVP_MAX_MD_SIZE ];
            EVP_MD_CTX hash_context;
            std::string hex_string;
    };
}

#endif

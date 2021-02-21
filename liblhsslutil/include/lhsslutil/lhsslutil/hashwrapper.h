#ifndef __LHSSLUTIL_HASHWRAPPER__
#define __LHSSLUTIL_HASHWRAPPER__

#include <string>

namespace LHSSLUtilNS
{
    // assume that bytes_in is length bytes long
    inline std::string BytesToHex( const unsigned char* bytes_in,
                                   unsigned int length );

    inline void FillHexFromBytes( const unsigned char* bytes_in,
                                  unsigned int length,
                                  std::string& hex_out );

    class HashWrapper
    {
        public:
            HashWrapper();
            HashWrapper( unsigned int _length );
            virtual ~HashWrapper();

            unsigned int GetHashLength() const;

            virtual bool operator==(const HashWrapper&) const;
            virtual bool operator<(const HashWrapper&) const;
            virtual bool operator>(const HashWrapper&) const;

            virtual int ReadFinalHashValue(void* out_bytes) = 0;
            virtual const unsigned char* GetFinalHashValue() = 0;

            virtual int ReadHashValue(void* out_bytes) const = 0;
            virtual const unsigned char* GetHashValue() const = 0;

            virtual void UpdateHash(const void* bytes, int len_bytes) = 0;
            virtual int FinalizeHash() = 0;
            virtual void RestartHash() = 0;

            virtual const std::string& HexString() const = 0;

        protected:
            unsigned int length;
    };
}

#endif

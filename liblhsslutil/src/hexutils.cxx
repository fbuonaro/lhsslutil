#include <lhsslutil_impl/hexutils.h>
#include <cstring>

namespace LHSSLUtilImplNS
{
namespace
{
    const char hex_digits[] = { '0', '1', '2', '3',
                                '4', '5', '6', '7',
                                '8', '9', 'A', 'B',
                                'C', 'D', 'E', 'F' };
}


    // assume that bytes_in is length bytes long
    std::string BytesToHex( const unsigned char* bytes_in,
                            unsigned int length )
    {
        unsigned int byte_index = 0;
        unsigned char curr_byte = 0;
        std::string hex_out;

        hex_out.reserve( length<<1 );

        while( byte_index < length )
        {
            curr_byte = bytes_in[byte_index++];
            hex_out.push_back( hex_digits[ curr_byte>>4 ] );
            hex_out.push_back( hex_digits[ curr_byte&0xF ] );
        }

        return hex_out;
    }

    // assume that bytes_in is length bytes long
    void FillHexFromBytes( const unsigned char* bytes_in,
                           unsigned int length,
                           std::string& hex_out )
    {
        unsigned int byte_index = 0;
        unsigned char curr_byte = 0;

        hex_out.clear();
        hex_out.reserve( length<<1 );

        while( byte_index < length )
        {
            curr_byte = bytes_in[ byte_index++ ];
            hex_out.push_back( hex_digits[curr_byte>>4] );
            hex_out.push_back( hex_digits[curr_byte&0xF] );
        }
    }

    unsigned char ByteForHexCharacter( char hex_char )
    {
        unsigned char out = 0;

        switch( hex_char )
        {
            default:
            case '0':
            {
                out = 0;
                break;
            }
            case '1':
            {
                out = 1;
                break;
            }
            case '2':
            {
                out = 2;
                break;
            }
            case '3':
            {
                out = 3;
                break;
            }
            case '4':
            {
                out = 4;
                break;
            }
            case '5':
            {
                out = 5;
                break;
            }
            case '6':
            {
                out = 6;
                break;
            }
            case '7':
            {
                out = 7;
                break;
            }
            case '8':
            {
                out = 8;
                break;
            }
            case '9':
            {
                out = 9;
                break;
            }
            case 'A':
            {
                out = 10;
                break;
            }
            case 'B':
            {
                out = 11;
                break;
            }
            case 'C':
            {
                out = 12;
                break;
            }
            case 'D':
            {
                out = 13;
                break;
            }
            case 'E':
            {
                out = 14;
                break;
            }
            case 'F':
            {
                out = 15;
                break;
            }
        }

        return out;
    }

    void HexToBytes( unsigned char* bytes_out, const std::string& hex_in )
    {
        unsigned int byte_index = 0;

        std::string::const_iterator end = hex_in.cend();
        std::string::const_iterator it_upper = hex_in.cbegin();
        std::string::const_iterator it_lower = hex_in.cbegin();
        ++it_lower;

        while( it_upper < end )
        {
            bytes_out[byte_index++] =   ( ByteForHexCharacter( *( it_upper ) ) << 4 ) 
                                      | ( ByteForHexCharacter( *( it_lower ) ) );
            it_lower += 2;
            it_upper += 2;
        }
    }

}

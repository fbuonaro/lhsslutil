#ifndef __LHSSLUTIL_OPENSSLPROCESSCONTEXT_H__
#define __LHSSLUTIL_OPENSSLPROCESSCONTEXT_H__

#include <mutex>
#include <stdexcept>
#include <thread>
#include <unordered_map>
#include <vector>

// #define OPENSSL_THEAD_DEBUG

namespace LHSSLUtilNS
{
    enum class OpenSSLModule : std::int8_t
    {
        None          =   1<<0,
        Randomness    =   1<<1
    };

    typedef OpenSSLModule OpenSSLModules;

    inline OpenSSLModule operator&( OpenSSLModule a, OpenSSLModule b )
    {
        return static_cast< OpenSSLModule >(
               static_cast< std::int8_t >( a ) & static_cast< std::int8_t >( b ) );
    }

    inline OpenSSLModule operator&=( OpenSSLModule& a, OpenSSLModule& b )
    {
        a = static_cast< OpenSSLModule >(
            static_cast< std::int8_t >( a ) & static_cast< std::int8_t >( b ) );

        return a;
    }

    inline OpenSSLModule operator|( OpenSSLModule a, OpenSSLModule b )
    {
        return static_cast< OpenSSLModule >(
               static_cast< std::int8_t >( a ) | static_cast< std::int8_t >( b ) );
    }

    inline OpenSSLModule operator|=( OpenSSLModule& a, OpenSSLModule& b )
    {
        a = static_cast< OpenSSLModule >(
            static_cast< std::int8_t >( a ) | static_cast< std::int8_t >( b ) );

        return a;
    }

    struct OpenSSLProcessContextAlreadyInitialized : public std::exception
    {
        virtual const char* what() const throw();
    };

    struct OpenSSLProcessContextMultiThreadingDisabled : public std::exception
    {
        virtual const char* what() const throw();
    };

    class OpenSSLProcessContext
    {
        public:
            // Static Singleton API - not thread safe
            static void ProcessToggleLock( int lock_mode, int lock_number );
            static unsigned long ProcessCurrentThreadIdentifier();
            static void ProcessInitialize( OpenSSLModules modules );
            static void ProcessCleanup();

        private:
            // CTOR/DTOR
            OpenSSLProcessContext();
            ~OpenSSLProcessContext();

            OpenSSLProcessContext( const OpenSSLProcessContext& other ) = delete;
            OpenSSLProcessContext( OpenSSLProcessContext&& other ) = delete;
            OpenSSLProcessContext& operator=( const OpenSSLProcessContext& other ) = delete;
            OpenSSLProcessContext& operator=( OpenSSLProcessContext&& other ) = delete;

            // Methods
            void Initialize( OpenSSLModules modules );
            void Cleanup();
            unsigned long CurrentThreadIdentifier();
            void ToggleLock( int lock_mode, int lock_number );

            // Data
            OpenSSLModules modules;
            std::unordered_map< std::thread::id, unsigned long > thread_ids;
            std::mutex thread_ids_mutex;
            unsigned long last_thread_id;
            std::mutex* openssl_mutexes;

            // Static Data
            static OpenSSLProcessContext processInstance;
            static bool processInitialized;
    };
}

#endif

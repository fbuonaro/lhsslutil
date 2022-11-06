#include <iostream>

#define OPENSLL_THREAD_DEFINES
#include <openssl/opensslconf.h>
#include <openssl/crypto.h>

#include <lhsslutil/opensslprocesscontext.h>
#include <lhsslutil/opensslrandomnessgenerator.h>


namespace LHSSLUtilNS
{
    //OPENSSL CALLBACKS
    static void openssl_locking_callback(
        int lock_mode, int lock_number, const char* file, int line )
    {
#ifdef OPENSSL_THEAD_DEBUG
        std::cout << "OpenSSLProcess::openssl_locking_callback: locking " << file << " at line " << line << std::endl;
#endif
        (void)file;
        (void)line;
        OpenSSLProcessContext::ProcessToggleLock( lock_mode, lock_number );
    }

    static void openssl_threadid_callback( CRYPTO_THREADID* openssl_threadid )
    {
        unsigned long thread_id = OpenSSLProcessContext::ProcessCurrentThreadIdentifier();
        CRYPTO_THREADID_set_numeric( openssl_threadid, thread_id );
    }


    //STATIC API
    bool OpenSSLProcessContext::processInitialized = false;
    OpenSSLProcessContext OpenSSLProcessContext::processInstance;

    void OpenSSLProcessContext::ProcessInitialize( OpenSSLModules modules )
    {
#ifdef OPENSSL_THREADS
        if ( !OpenSSLProcessContext::processInitialized )
        {
            OpenSSLProcessContext::processInstance.Initialize( modules );
            OpenSSLProcessContext::processInitialized = true;
        }
        else
            throw OpenSSLProcessContextAlreadyInitialized();
#else
        throw OpenSSLProcessContextMultiThreadingDisabled();
#endif
    }

    void OpenSSLProcessContext::ProcessCleanup()
    {
#ifdef OPENSSL_THREADS
        if ( OpenSSLProcessContext::processInitialized )
        {
            OpenSSLProcessContext::processInstance.Cleanup();
            processInitialized = false;
        }
#else
        throw OpenSSLProcessContextMultiThreadingDisabled();
#endif
    }

    void OpenSSLProcessContext::ProcessToggleLock( int lock_mode, int lock_number )
    {
        if ( OpenSSLProcessContext::processInitialized )
            OpenSSLProcessContext::processInstance.ToggleLock( lock_mode, lock_number );
    }

    unsigned long OpenSSLProcessContext::ProcessCurrentThreadIdentifier()
    {
        unsigned long ret = 0;
        if ( OpenSSLProcessContext::processInitialized )
            ret = OpenSSLProcessContext::processInstance.CurrentThreadIdentifier();
        return ret;
    }

    //EXCEPTIONS
    const char* OpenSSLProcessContextMultiThreadingDisabled::what() const throw( )
    {
        return "OpenSSLProcessContext not enabled";
    }

    const char* OpenSSLProcessContextAlreadyInitialized::what() const throw( )
    {
        return "OpenSSLProcessContext already initialized";
    }

    //PROTECTED CTOR/DTOR, methdods
    OpenSSLProcessContext::OpenSSLProcessContext()
        : modules( OpenSSLModule::None )
        , thread_ids()
        , thread_ids_mutex()
        , last_thread_id( 0 )
        , openssl_mutexes( nullptr )
    {
    }

    OpenSSLProcessContext::~OpenSSLProcessContext()
    {
        try
        {
            Cleanup();
        }
        catch ( std::exception e )
        {
            std::cerr << "OpenSSLProcessContext::~OpenSSLProcessContext exception["
                << e.what() << "]" << std::endl;
        }
        catch ( ... )
        {
            std::cerr << "OpenSSLProcessContext::~OpenSSLProcessContext unknown exception"
                << std::endl;
        }
    }

    void OpenSSLProcessContext::Initialize( OpenSSLModules _modules )
    {
        Cleanup();

        modules = _modules;

        openssl_mutexes = new std::mutex[ CRYPTO_num_locks() ];

        CRYPTO_THREADID_set_callback( openssl_threadid_callback );

        CRYPTO_set_locking_callback( openssl_locking_callback );

        if ( ( modules & OpenSSLModule::Randomness ) != OpenSSLModule::None )
        {
            OpenSSLRandomnessGenerator::ProcessInitialize();
        }
    }

    void OpenSSLProcessContext::Cleanup()
    {

        if ( ( modules & OpenSSLModule::Randomness ) != OpenSSLModule::None )
        {
            OpenSSLRandomnessGenerator::ProcessCleanup();
        }

        CRYPTO_set_locking_callback( NULL );

        CRYPTO_THREADID_set_callback( NULL );

        if ( openssl_mutexes != nullptr )
        {
            for ( int i = 0; i < CRYPTO_num_locks(); ++i )
            {
                openssl_mutexes[ i ].unlock();
            }

            delete[] openssl_mutexes;
            openssl_mutexes = nullptr;
        }

        modules = OpenSSLModule::None;
    }

    unsigned long OpenSSLProcessContext::CurrentThreadIdentifier()
    {
        unsigned long ret = 0;
        std::thread::id this_thread_id( std::this_thread::get_id() );
        std::unordered_map< std::thread::id, unsigned long >::const_iterator cit =
            thread_ids.find( this_thread_id );
        std::unordered_map< std::thread::id, unsigned long >::const_iterator cend =
            thread_ids.cend();

        if ( cit != cend )
        {
            ret = cit->second;
        }
        else
        {
            std::lock_guard< std::mutex > thread_ids_lock( thread_ids_mutex );

            ret = last_thread_id + 1;
            thread_ids.insert(
                std::pair< std::thread::id, unsigned long >( this_thread_id, ret ) );
            last_thread_id = ret;
        }

        return ret;
    }

    //lock whatever is at lock_number
    void OpenSSLProcessContext::ToggleLock( int lock_mode, int lock_number )
    {
        if ( lock_mode & CRYPTO_LOCK )
        {
            openssl_mutexes[ lock_number ].lock();
        }
        else
        {
            openssl_mutexes[ lock_number ].unlock();
        }
    }

}

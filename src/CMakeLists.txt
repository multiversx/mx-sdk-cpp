add_subdirectory(utils)

file(GLOB src
        transaction/transaction.cpp
        transaction/signer.cpp
        account/account.cpp
        account/address.cpp
        filehandler/ifile.cpp
        filehandler/pemreader.cpp
        wrappers/jsonwrapper.h
        wrappers/httpwrapper.h
        wrappers/cryptosignwrapper.h wrappers/cryptosignwrapper.cpp
        proxy/proxyprovider.cpp
        )

include_directories(utils)
include_directories(wrappers)

add_library(src ${src})

target_link_libraries(src PUBLIC utils)
target_link_libraries(src PUBLIC external)
target_link_libraries(src LINK_PUBLIC ${LIBSODIUM_LIBRARY})

target_link_libraries(src PUBLIC
        $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:OpenSSL::SSL>
        $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:OpenSSL::Crypto>)

target_compile_definitions(src PUBLIC
        $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>
        )
	


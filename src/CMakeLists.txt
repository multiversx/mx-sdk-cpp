add_subdirectory(utils)

file(GLOB src
        transaction/transaction.cpp
        transaction/signer.cpp
        transaction/messagesigner.cpp
        transaction/scarguments.cpp
        transaction/esdt.cpp
        internal/biguint.cpp
        account/account.cpp
        account/address.cpp
        filehandler/ifile.cpp
        filehandler/pemreader.cpp
        filehandler/keyfilereader.cpp
        wrappers/jsonwrapper.h
        wrappers/httpwrapper.h
        wrappers/cryptosignwrapper.h wrappers/cryptosignwrapper.cpp
        provider/apiresponse.h
        provider/proxyprovider.cpp
        provider/data/data_transaction.cpp
        provider/data/networkconfig.cpp
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
	


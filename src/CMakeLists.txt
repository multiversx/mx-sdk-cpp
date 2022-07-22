add_subdirectory(utils)

file(GLOB src
        transaction/transaction.cpp
        transaction/signer.cpp
        transaction/messagesigner.cpp
        transaction/esdt.cpp
        transaction/gas_estimator.cpp
        transaction/token_payment.cpp
        transaction/payload_builder.cpp
        transaction/itransaction_builder.cpp
        transaction/transaction_builders.cpp
        transaction/transaction_factory.cpp
        smartcontracts/sc_arguments.cpp
        smartcontracts/contract_call.cpp
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

option(BUILD_SHARED_LIBS ON)
add_library(src SHARED ${src})

target_link_libraries(src PUBLIC utils)
target_link_libraries(src PUBLIC external)
target_link_libraries(src LINK_PUBLIC ${LIBSODIUM_LIBRARY})

target_link_libraries(src PUBLIC
        $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:OpenSSL::SSL>
        $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:OpenSSL::Crypto>)

target_compile_definitions(src PUBLIC
        $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>
        )

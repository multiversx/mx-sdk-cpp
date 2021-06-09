add_subdirectory(utils)

file(GLOB src
        transaction/transaction.cpp
        transaction/signer.cpp
        account/account.cpp
        account/address.cpp
        filehandler/ifile.cpp
        filehandler/pemreader.cpp
        wrappers/jsonwrapper.h
        wrappers/cryptosignwrapper.h wrappers/cryptosignwrapper.cpp)

include_directories(utils)
include_directories(wrappers)

add_library(src ${src})

target_link_libraries(src PUBLIC utils)
target_link_libraries(src PUBLIC external)
	


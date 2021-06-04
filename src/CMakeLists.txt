add_subdirectory(utils)

file(GLOB src
        transaction/transaction.cpp
        transaction/signer.cpp
        account/account.cpp
        account/address.cpp
        filehandler/ifilehandler.cpp
        filehandler/pemhandler.cpp
        wrappers/cryptosignwrapper.h wrappers/cryptosignwrapper.cpp)

include_directories(utils)
include_directories(wrappers)

add_library(src ${src})

target_link_libraries(src PUBLIC utils)
target_link_libraries(src PUBLIC external)
	


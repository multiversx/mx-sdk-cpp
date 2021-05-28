add_subdirectory(utils)

file(GLOB src
	internal.h
	transaction/transaction.cpp
	transaction/signer.cpp
	account/account.cpp
	account/address.cpp)

include_directories(utils)

add_library(src ${src})


include_directories(../../include)
	
target_link_libraries(src PUBLIC utils)
target_link_libraries(src PUBLIC external)
	


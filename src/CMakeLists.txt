add_subdirectory(utils)

file(GLOB src
	transaction/transaction.cpp
	transaction/signer.cpp
	account/account.cpp
	account/address.cpp)

include_directories(utils)

add_library(src ${src})
	
target_link_libraries(src PUBLIC utils)
target_link_libraries(src PUBLIC external)
	


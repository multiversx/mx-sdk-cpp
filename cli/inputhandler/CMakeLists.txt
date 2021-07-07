file(GLOB inputhandlers
		options.h
        arghandler.h arghandler.cpp
        jsonfile.h jsonfile.cpp
        wrappers/iwrapper.h
        wrappers/transaction_input_wrapper.h
        wrappers/pem_input_wrapper.h)

add_library(inputhandler ${inputhandlers})

target_link_libraries(inputhandler PUBLIC utils)
	

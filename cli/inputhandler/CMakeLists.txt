file(GLOB inputhandlers
        arghandler.h arghandler.cpp
        jsonhandler.h jsonhandler.cpp
        wrappers/iwrapper.h
        wrappers/transaction_input_wrapper.h
        wrappers/pem_input_wrapper.h)

add_library(inputhandler ${inputhandlers})

target_link_libraries(inputhandler PUBLIC utils)
	

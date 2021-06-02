file(GLOB inputhandlers
        arghandler.h arghandler.cpp
        ifilehandler.h ifilehandler.cpp
        jsonhandler.h jsonhandler.cpp
        pemhandler.h pemhandler.cpp
        wrappers/iwrapper.h
        wrappers/transaction_input_wrapper.h
        wrappers/pem_input_wrapper.h)

add_library(inputhandler ${inputhandlers})

target_link_libraries(inputhandler PUBLIC utils)
	

file(GLOB inputhandlers
        arghandler.h arghandler.cpp
        ifilehandler.h ifilehandler.cpp
        jsonhandler.h jsonhandler.cpp
        pemhandler.h pemhandler.cpp
        wrappers/iwrapper.h
        wrappers/jsonhandler_input_wrapper.h
        wrappers/pemhandler_input_wrapper.h)

add_library(inputhandler ${inputhandlers})

target_link_libraries(inputhandler PUBLIC utils)
	

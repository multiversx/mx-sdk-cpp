#include_directories(${PROJECT_SOURCE_DIR}/src/utils)

file(GLOB inputhandlers
	arghandler.h arghandler.cpp
	ifilehandler.h ifilehandler.cpp
	jsonhandler.h jsonhandler.cpp
	pemhandler.h pemhandler.cpp
	wrappers/iwrapper.h
	wrappers/jsonhandler_input_wrapper.h
	wrappers/pemhandler_input_wrapper.h)

add_library(inputhandler ${inputhandlers})

#include_directories(../../src/utils)
	
target_link_libraries(inputhandler PUBLIC utils)
	

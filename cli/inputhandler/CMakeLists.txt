file(GLOB inputhandlers
	options.h options.cpp
	options_esdt.h options_esdt.cpp
	arghandler.h arghandler.cpp
	wrappers/iwrapper.h
	wrappers/transaction_input_wrapper.h
	)

add_library(inputhandler ${inputhandlers})

target_link_libraries(inputhandler PUBLIC src)

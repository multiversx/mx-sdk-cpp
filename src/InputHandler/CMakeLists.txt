file(GLOB inputhandlers
	ArgHandler/arghandler.h ArgHandler/arghandler.cpp
	IFileHandler/ifilehandler.h IFileHandler/ifilehandler.cpp
	InputWrappers/iwrapper.h
	InputWrappers/jsonhandler_input_wrapper.h
	InputWrappers/pemhandler_input_wrapper.h
	JsonHandler/jsonhandler.h JsonHandler/jsonhandler.cpp
	PemHandler/pemhandler.h PemHandler/pemhandler.cpp
	address.h address.cpp
	account.h account.cpp
		signer.h signer.cpp
		transaction.h transaction.cpp
)

add_library(inputhandler ${inputhandlers})

include_directories(../Utils)
	
target_link_libraries(inputhandler PUBLIC utils)
	

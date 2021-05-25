file(GLOB utils
 base64.h base64.cpp
 bech32.h bech32.cpp
 bites.h bites.cpp
 fifo_map.hpp
 hex.h hex.cpp
 json.hpp
 params.h
 strchr.h
)

add_library(utils ${utils})
	
	

	
file(GLOB external
	json/json.hpp
	bech32/bech32.h bech32/bech32.cpp)

add_library(external ${external})
	

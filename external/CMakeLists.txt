file(GLOB external
        json/json.hpp
        http/httplib.h
        bech32/bech32.h bech32/bech32.cpp
        cliparser/cxxopts.hpp
        toml/cpptoml.h
        )

add_library(external ${external})
	

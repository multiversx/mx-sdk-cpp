file(GLOB external
        json/json.hpp
        http/httplib.h
        bech32/bech32.h bech32/bech32.cpp
        cliparser/cxxopts.hpp
        toml/cpptoml.h
        aes_128_ctr/aes.c
        keccak/sha3.c
        bigint/integer.h bigint/integer.cpp
        )

add_library(external ${external})
	

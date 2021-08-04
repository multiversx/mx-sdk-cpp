file(GLOB utils
        base64.h base64.cpp
        bits.h bits.cpp
        hex.h hex.cpp
        params.h
        errors.h
        common.h
        cfg.h
        )

add_library(utils ${utils})
	

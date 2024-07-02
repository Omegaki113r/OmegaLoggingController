set( TINY_JSON_HEADERS submodules/third-party/tiny-json )
set( TINY_JSON_SOURCES submodules/third-party/tiny-json/tiny-json.c )
set( STC_HEADERS submodules/third-party/STC/inc/c11 submodules/third-party/STC/inc/stc submodules/third-party/STC/inc/stc/algo submodules/third-party/STC/inc/stc/priv )

set( HEADER_DIRECTORIES inc ${TINY_JSON_HEADERS} ${STC_HEADERS} )
set( SOURCES src/OmegaLoggingSystemController.c ${TINY_JSON_SOURCES})

idf_component_register  ( 
                            INCLUDE_DIRS    ${HEADER_DIRECTORIES} 
                            SRCS            ${SOURCES}
                            PRIV_REQUIRES   esp_timer json spiffs
                            REQUIRES        OmegaFileSystemController OmegaBaseLoggingController OmegaUtilityDriver
                        )

set_property(TARGET ${COMPONENT_LIB} PROPERTY C_STANDARD 23)
# target_compile_options(${COMPONENT_LIB} PRIVATE 
# -Wall 
# -Werror 
# -Wextra 
# -Wpedantic
# )
set( TINY_JSON_HEADERS third-party/tiny-json )
set( TINY_JSON_SOURCES third-party/tiny-json/tiny-json.c )

set( HEADER_DIRECTORIES inc ${TINY_JSON_HEADERS})
set( SOURCES src/OmegaLoggingSystemController.c ${TINY_JSON_SOURCES})

idf_component_register  ( 
                            INCLUDE_DIRS    ${HEADER_DIRECTORIES} 
                            SRCS            ${SOURCES}
                            REQUIRES        esp_timer OmegaFileSystemController
                        )

set_property(TARGET ${COMPONENT_LIB} PROPERTY C_STANDARD 23)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wall -Werror -Wextra )
# -Wpedantic)
set( HEADER_DIRECTORIES inc)
set( SOURCES src/OmegaLoggingSystemController.c )

idf_component_register  ( 
                            INCLUDE_DIRS    ${HEADER_DIRECTORIES} 
                            SRCS            ${SOURCES}
                        )

set_property(TARGET ${COMPONENT_LIB} PROPERTY C_STANDARD 23)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wall -Werror -Wextra )
# -Wpedantic)
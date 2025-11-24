file(GLOB_RECURSE SOURCES "*.cpp" "*.c" "*.h" "*.hpp")

idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS ".")

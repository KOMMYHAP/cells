# add headers
target_sources(${PROJECT_NAME} PUBLIC
        assert.h
)

#target_compile_definitions(${PROJECT_NAME} PUBLIC LIBASSERT_PREFIX_ASSERTIONS)
#target_link_libraries(${PROJECT_NAME} PUBLIC libassert::assert)

# generate configuration header
option(COMMON_ASSERTS_ENABLED "Enable asserts" ON)
configure_file(asserts_config.h.in asserts_config.h @ONLY)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
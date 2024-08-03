# add headers
target_sources(${PROJECT_NAME} PUBLIC
        narrow_cast.h
        stub_error_code.h
)

# add sources
target_sources(${PROJECT_NAME} PRIVATE
        stub_error_code.cpp
)

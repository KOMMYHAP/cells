# add sources
target_sources(${PROJECT_NAME} PRIVATE
        __brain.cpp
        brain_processor.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        __brain.h
        __brain.hpp
        brain_packed_data.h
        brain_processor.h
)

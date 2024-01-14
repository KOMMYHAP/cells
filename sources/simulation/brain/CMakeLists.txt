# add sources
target_sources(${PROJECT_NAME} PRIVATE
        cell.cpp
        brain.cpp
        brain_processor.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        cell.h
        brain.h
        brain.hpp
        brain_packed_data.h
        brain_processor.h
)

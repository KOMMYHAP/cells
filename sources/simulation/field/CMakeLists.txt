# add sources
target_sources(${PROJECT_NAME} PRIVATE
        field.cpp
        field_search_proxy.cpp
        field_iterator.cpp)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        field.h
        field_search_proxy.h
        field_iterator.h
        field_iterator.hpp)

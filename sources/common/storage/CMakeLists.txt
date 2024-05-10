
# add sources
target_sources(${PROJECT_NAME} PRIVATE
        storage.cpp
        stack_storage.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        storage.h
        stack_storage.h
        storage.hpp
        impl/impl.h
)

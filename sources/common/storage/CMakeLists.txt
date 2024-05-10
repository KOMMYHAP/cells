
# add sources
target_sources(${PROJECT_NAME} PRIVATE
        storage.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        storage.h
        storage.hpp
        impl/impl.h
)

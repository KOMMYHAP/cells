# add sources
target_sources(${PROJECT_NAME} PRIVATE
        registrar.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        registrar.h
        registrar.hpp
        registrable_system.h
)

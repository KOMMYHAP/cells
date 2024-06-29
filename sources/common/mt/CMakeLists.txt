target_sources(${PROJECT_NAME} PUBLIC
        spin_lock.h
        spin_lock.hpp
)

target_sources(${PROJECT_NAME} PRIVATE
        spin_lock.cpp
)
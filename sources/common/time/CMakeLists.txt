target_sources(${PROJECT_NAME} PUBLIC
        time.h
        utils.h
)

target_sources(${PROJECT_NAME} PRIVATE
        utils.cpp
        time.cpp
)
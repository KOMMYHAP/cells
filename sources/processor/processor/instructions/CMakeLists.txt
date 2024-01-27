# add sources
target_sources(${PROJECT_NAME} PRIVATE
        instruction_nope.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        instructions.h
        instruction_nope.h
)
# add sources
target_sources(${PROJECT_NAME} PRIVATE
        processor.cpp
        processor_context.cpp
        processor_control_block.cpp
        memory.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        processor.h
        processor_context.h
        processor_context.hpp
        processor_state.h
        processor_control_block.h
        memory.h
        memory.hpp
)
# add sources
target_sources(${PROJECT_NAME} PRIVATE
        processor.cpp
        processor_context.cpp
        processor_control_block.cpp
        processor_control_block_guard.cpp
        processor_instruction.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        processor.h
        processor_context.h
        processor_context.hpp
        processor_state.h
        processor_control_block.h
        processor_control_block_guard.h
        processor_memory.h
        processor_memory.hpp
        processor_stack.h
        processor_stack.hpp
        processor_memory_type.h
        processor_instruction.h
        processor_debugger.h
        processor_user_data.h
)
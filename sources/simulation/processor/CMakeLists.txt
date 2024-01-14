# add sources
target_sources(${PROJECT_NAME} PRIVATE
        brain_processor.cpp
        processor.cpp
        procedure.cpp
        procedure_table.cpp
        processor_control_block.cpp
        memory.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        brain_processor.h
        processor.h
        procedure.h
        procedure_table.h
        processor_control_block.h
        memory.h
        memory.hpp
)

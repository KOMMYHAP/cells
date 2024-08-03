# add sources
target_sources(${PROJECT_NAME} PRIVATE
        procedure_table.cpp
        procedure_context.cpp
        procedure_executor.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        procedure.h
        procedure_id.h
        procedure_state.h
        procedure_table.h
        procedure_context.h
        procedure_context.hpp
        procedure_executor.h
)
# add sources
target_sources(${PROJECT_NAME} PRIVATE
        procedure_table.cpp
        procedure_context.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        procedure.h
        procedure_id.h
        procedure_table.h
        procedure_context.h
        procedure_context.hpp
        pending_procedure_id.h
)
# add sources
target_sources(${PROJECT_NAME} PRIVATE
        procedure.cpp
        procedure_table.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        procedure.h
        procedure_id.h
        procedure.hpp
        procedure_table.h
)
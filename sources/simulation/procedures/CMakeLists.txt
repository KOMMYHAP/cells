# add sources
target_sources(${PROJECT_NAME} PRIVATE
        move_procedure.cpp
        consume_procedure.cpp
        reproduction_procedure.cpp
        look_procedure.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        move_procedure.h
        consume_procedure.h
        direction.h
        reproduction_procedure.h
        look_procedure.h
)

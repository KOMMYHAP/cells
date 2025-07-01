add_subdirectory(base)
target_sources(${PROJECT_NAME} PRIVATE
        engine_summary_widget.cpp
        simulation_summary_widget.cpp
)

target_sources(${PROJECT_NAME} PUBLIC
        engine_summary_widget.h
        simulation_summary_widget.h
)
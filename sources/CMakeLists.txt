add_executable(${PROJECT_NAME})


# add SFML
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.6.1 EXACT REQUIRED graphics window)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window)


target_sources(${PROJECT_NAME} PRIVATE
    main.cpp pch.h
    world.h world.cpp)
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)
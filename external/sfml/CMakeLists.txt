message(STATUS "Fetching SFML...")

# We need only system submodule for usefull sf::Clock, sf::Vector and sf::Time
set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)
set(SFML_BUILD_GRAPHICS FALSE)
set(SFML_BUILD_WINDOW FALSE)

FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML
        GIT_TAG 69ea0cd863aed1d4092b970b676924a716ff718b # 2.6.1
        GIT_SHALLOW ON
)

FetchContent_MakeAvailable(sfml)
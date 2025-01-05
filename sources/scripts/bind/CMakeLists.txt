target_sources(${PROJECT_NAME} PRIVATE
        lua_menu.cpp
        lua_menu.h
        lua_menu_widget.cpp
        lua_menu_widget.h
)


# Compilation time of lua_imgui_bindings.cpp is too large, so it was excluded to separate library
add_library(cells-lua-imgui-bindings STATIC
        lua_imgui_bindings.cpp
        lua_imgui_bindings.h
)
target_include_directories(cells-lua-imgui-bindings PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cells-lua-imgui-bindings PUBLIC
        sol2::sol2
        lua
        ImGuiLib
)
target_compile_features(cells-lua-imgui-bindings PRIVATE cxx_std_23)
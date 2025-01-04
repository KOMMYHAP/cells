# Copy DLLs needed for runtime on Windows
function(target_copy_required_dll)
    if (NOT WIN32)
    endif ()

    if (NOT DEFINED ENV{VS_ASAN_PATH})
        # E.g. C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.40.33807\bin\Hostx64\x64
        message(FATAL_ERROR "You should specifiy environment variable VS_ASAN_PATH")
    endif ()

    set(asan-target $ENV{VS_ASAN_PATH}/clang_rt.asan_dynamic-x86_64.dll)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            -E copy
            -t $<TARGET_FILE_DIR:${PROJECT_NAME}>
            $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
            $<$<BOOL:${ENABLE_ASAN}>:${asan-target}>
            COMMAND_EXPAND_LISTS
    )
endfunction()
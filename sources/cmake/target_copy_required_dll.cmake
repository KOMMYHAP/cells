# Copy DLLs needed for runtime on Windows
function(target_copy_required_dll)
    if (NOT WIN32)
    endif()

    #set(asan-target-hack $ENV{VCToolsInstallDir}/bin/Hostx64/x64/clang_rt.asan_dynamic-x86_64.dll)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            -E copy
            -t $<TARGET_FILE_DIR:${PROJECT_NAME}>
            $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
            #$<$<BOOL:${ENABLE_ASAN}>:${asan-target-hack}>
            COMMAND_EXPAND_LISTS
    )
endfunction()
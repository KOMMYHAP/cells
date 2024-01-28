# add sources
target_sources(${PROJECT_NAME} PRIVATE
        add_registry_registry.cpp
        add_registry_value.cpp
        call.cpp
        compare_registry_registry.cpp
        compare_registry_value.cpp
        jump.cpp
        jump_if_equal.cpp
        jump_if_greater.cpp
        jump_if_greater_equal.cpp
        jump_if_less.cpp
        jump_if_less_equal.cpp
        jump_if_not_equal.cpp
        nope.cpp
        pop_stack_registry.cpp
        push_stack_registry.cpp
        push_stack_value.cpp
        subtract_registry_registry.cpp
        subtract_registry_value.cpp
        write_registry_registry.cpp
        write_registry_value.cpp
)


# add headers
target_sources(${PROJECT_NAME} PUBLIC
        add_registry_registry.h
        add_registry_value.h
        call.h
        compare_registry_registry.h
        compare_registry_value.h
        instruction_list.h
        jump.h
        jump_if_equal.h
        jump_if_greater.h
        jump_if_greater_equal.h
        jump_if_less.h
        jump_if_less_equal.h
        jump_if_not_equal.h
        nope.h
        pop_stack_registry.h
        push_stack_registry.h
        push_stack_value.h
        subtract_registry_registry.h
        subtract_registry_value.h
        write_registry_registry.h
        write_registry_value.h
)

add_executable(
    running_median
    RunningMedianMain
)

target_link_libraries(running_median
    median
    events
)

add_custom_command(TARGET running_median POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       ${CMAKE_CURRENT_SOURCE_DIR}/example_input.txt $<TARGET_FILE_DIR:running_median>
)

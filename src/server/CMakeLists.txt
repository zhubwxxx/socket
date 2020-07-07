project(server)

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_options(${PROJECT_NAME} PUBLIC -pthread)
target_link_libraries(${PROJECT_NAME} common)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

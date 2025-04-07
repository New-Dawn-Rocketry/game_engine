add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE main.cpp renderer.cpp)
# target_precompile_headers(${PROJECT_NAME} PRIVATE *.hpp)
target_link_libraries(${PROJECT_NAME} PRIVATE vendor)
project(FromFile)

add_executable(from_file_example main.cpp)
target_link_libraries(from_file_example PRIVATE ${project_name})
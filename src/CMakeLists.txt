target_sources(${project_name} PUBLIC 
    ipc.hpp
    ipc.cpp
    activity.hpp
    activity.cpp
)
target_include_directories(${project_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
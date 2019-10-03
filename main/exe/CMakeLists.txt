cmake_minimum_required(VERSION 3.12)

set(BUILDSYSTEM_PROJECTS_DIR
    server
    client
)

include(${PROJECT_SOURCE_DIR}/cmake/BuildSystem.cmake)

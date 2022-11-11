project(solarsystem)
add_executable(${PROJECT_NAME} main.cpp window.cpp body.cpp sun.cpp planet.cpp moon.cpp path.cpp)
enable_abcg(${PROJECT_NAME})

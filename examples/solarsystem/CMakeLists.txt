project(solarsystem)
add_executable(${PROJECT_NAME} main.cpp 
                                window.cpp 
                                body.cpp 
                                path.cpp
                                trackball.cpp
                                camera.cpp)
enable_abcg(${PROJECT_NAME})

project(solarsystem2)
add_executable(${PROJECT_NAME} main.cpp 
                                window.cpp 
                                body.cpp 
                                path.cpp
                                camera.cpp
                                skydome.cpp)
enable_abcg(${PROJECT_NAME})

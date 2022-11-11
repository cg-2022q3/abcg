#ifndef PATH_HPP_
#define PATH_HPP_

#include "abcgOpenGL.hpp"

class Path {
public:
  void create(GLuint program);
  void paint();
  void destroy();
  void update();


private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  glm::vec3 translation_axis{};
  float translation_radius{};
  
};

#endif
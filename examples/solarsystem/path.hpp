#ifndef PATH_HPP_
#define PATH_HPP_

#include "abcgOpenGL.hpp"

class Path {
public:
  void create(GLuint program, float orbit_radius);
  void paint();
  void destroy();
  void update(glm::vec3 pos);
  void computeModelMatrix();
  void render() const;
  void generateCircle(int num_vertices, float orbit_radius);


private:
  GLuint m_VAO{};
  GLuint m_VBO{};

  glm::vec3 translation_axis{};
  float translation_radius{};

  std::vector<glm::vec3> m_vertices;
  std::vector<GLuint> m_indices;

  glm::vec3 position{0.0f};


  float radius{};
  glm::vec4 color{};


  glm::mat4 modelMatrix{1.0f};


  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};

  
};

#endif
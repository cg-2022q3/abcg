#ifndef BODY_HPP_
#define BODY_HPP_

#include "abcgOpenGL.hpp"
#include "path.hpp"

struct Vertex {
  glm::vec3 position{};

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Body {
public:
  void create(GLuint program);
  void paint();
  void destroy();
  void update();
  void createBuffers();
  void render() const;

  [[nodiscard]] int getNumTriangles() const {
    return gsl::narrow<int>(m_indices.size()) / 3;
  }
  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  void generateUVSphere(int stacks, int slices);

  Body *satellite_of;
  Path path;

  float scale{1.0f};
  glm::vec3 position{};

  glm::vec3 rotation_axis{};
  glm::vec3 translation_axis{};
  float distance{};
  glm::vec4 color{};


  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};

  
};

#endif
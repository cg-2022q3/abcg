#ifndef BODY_HPP_
#define BODY_HPP_

#include "abcgOpenGL.hpp"
#include "path.hpp"

struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 texCoord{};
  
  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Body {
public:
  void create(GLuint program);
  void paint();
  void destroy();
  void update(float deltaTime,float rot_speed, float trans_speed);
  void computeModelMatrix();
  void render(glm::mat4 viewMatrix) const;
  void generateUVSphere(int stacks, int slices);

  std::string name{""};
  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;
  std::vector<GLuint> m_lines_indices;

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  
  GLuint m_program{};

  std::string texture_path{""};
  glm::vec3 m_lightPos{0.0f, 0.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{};
  GLuint m_diffuseTexture{};

  glm::vec4 color{};
  float scale{1.0f};
  Body *satellite_of;

  Path path;

  glm::vec3 position{0.0f};
  float orbit_radius{};
  float translation_angle{0.0f};
  float translation_speed{0.0f};
  float rotation_angle{0.0f};
  float rotation_speed{1.0f};

  glm::mat4 modelMatrix{1.0f};

  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};

  
};

#endif
#include "body.hpp"
#include <filesystem>


void Body::create(GLuint program){
  generateUVSphere(36,36);
  m_program = program;
  // Delete previous buffers
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);

  // VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     sizeof(m_vertices.at(0)) * m_vertices.size(),
                     m_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(m_indices.at(0)) * m_indices.size(),
                     m_indices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
  m_modelMatrixLoc = abcg::glGetUniformLocation(program, "modelMatrix");
  m_colorLoc = abcg::glGetUniformLocation(program, "color");


  // Release previous VAO
  abcg::glDeleteVertexArrays(1, &m_VAO);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);
  abcg::glBindVertexArray(m_VAO);

  // Bind EBO and VBO
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

  // Bind vertex attributes
  auto const positionAttribute{abcg::glGetAttribLocation(program, "inPosition")};
  if (positionAttribute >= 0) {
    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex), nullptr);
  }

  auto const normalAttribute{abcg::glGetAttribLocation(program, "inNormal")};
  if (normalAttribute >= 0) {
    abcg::glEnableVertexAttribArray(normalAttribute);
    auto const offset{offsetof(Vertex, normal)};
    abcg::glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex),
                                reinterpret_cast<void *>(offset));
  }

  auto const texCoordAttribute{
      abcg::glGetAttribLocation(program, "inTexCoord")};
  if (texCoordAttribute >= 0) {
    abcg::glEnableVertexAttribArray(texCoordAttribute);
    auto const offset{offsetof(Vertex, texCoord)};
    abcg::glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex),
                                reinterpret_cast<void *>(offset));
  }

  // End of binding
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  abcg::glBindVertexArray(0);

  if (std::filesystem::exists(texture_path)){
    abcg::glDeleteTextures(1, &m_diffuseTexture);
    m_diffuseTexture = abcg::loadOpenGLTexture({.path = texture_path});
  }


  if (satellite_of){
    position = satellite_of->position + glm::vec3{orbit_radius, 0.0f, 0.0f};
  }

  computeModelMatrix();
  path.orbit_radius = orbit_radius;
  path.create(program);
}

void Body::destroy(){
  abcg::glDeleteTextures(1, &m_diffuseTexture);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Body::update(float deltaTime, float speed){
  // updates position for bodies that are satellites of another body
  if (satellite_of){
    // translation
    auto angle = 10.0f * deltaTime * translation_speed * speed;
    translation_angle += angle;
    position.x = satellite_of->position.x + cos(translation_angle) * orbit_radius;
    position.z = satellite_of->position.z - sin(translation_angle) * orbit_radius;

    //rotation
    angle = 10.0f * deltaTime * rotation_speed * speed;
    rotation_angle += angle;
  
    path.update(satellite_of->position);
  }
  computeModelMatrix();
  
}
void Body::computeModelMatrix(){
  modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::translate(modelMatrix, position);
  modelMatrix = glm::rotate(modelMatrix, rotation_angle,glm::vec3(0.0f,1.0f,0.0f));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
}

void Body::generateUVSphere(int stacks, int sectors){
  // geneterates a UV sphere with radius 1.0f
  float sectorAngle, stackAngle, x, y, z, xz, u, v;
  float sectorStep = 2 * M_PI / sectors;
  float stackStep = M_PI / stacks;

  for(auto i : iter::range(stacks+1)){
    stackAngle = M_PI / 2 - i * stackStep;
    xz = std::cos(stackAngle);
    y = std::sin(stackAngle);

    for(auto j: iter::range(sectors+1)){
      sectorAngle = j * sectorStep;

      x = xz * std::sin(sectorAngle);
      z = xz * std::cos(sectorAngle);


      u = (float) j / sectors;
      v = 1.0f - (float) i / stacks;

      Vertex const vertex{.position = {x, y, z}, .normal = {x, y, z}, .texCoord = {u,v}};

      m_vertices.push_back(vertex);

    }
  }
  // now we create the indices for the triangles
  int k1, k2;
  for(int i = 0; i < stacks; ++i){
    k1 = i * (sectors + 1);     // beginning of current stack
    k2 = k1 + sectors + 1;      // beginning of next stack

    for(int j = 0; j < sectors; ++j, ++k1, ++k2)
    {
      // 2 triangles per sector excluding first and last stacks
      // k1 => k2 => k1+1
      if(i != 0)
      {
          m_indices.push_back(k1);
          m_indices.push_back(k2);
          m_indices.push_back(k1 + 1);
      }

      // k1+1 => k2 => k2+1
      if(i != (stacks-1))
      {
          m_indices.push_back(k1 + 1);
          m_indices.push_back(k2);
          m_indices.push_back(k2 + 1);
      }

      // store indices for lines
      // vertical lines for all stacks, k1 => k2
      m_lines_indices.push_back(k1);
      m_lines_indices.push_back(k2);
      if(i != 0)  // horizontal lines except 1st stack, k1 => k+1
      {
          m_lines_indices.push_back(k1);
          m_lines_indices.push_back(k1 + 1);
      }
    }
  }


}

void Body::render(glm::mat4 viewMatrix) const {
  // Set uniform variables for the current model
  abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);
  abcg::glUniform4fv(m_colorLoc, 1, &color[0]); 

    auto const normalMatrixLoc{
      abcg::glGetUniformLocation(m_program, "normalMatrix")};
  auto const lightDirLoc{
      abcg::glGetUniformLocation(m_program, "lightDirWorldSpace")};
  auto const shininessLoc{abcg::glGetUniformLocation(m_program, "shininess")};
  auto const IaLoc{abcg::glGetUniformLocation(m_program, "Ia")};
  auto const IdLoc{abcg::glGetUniformLocation(m_program, "Id")};
  auto const IsLoc{abcg::glGetUniformLocation(m_program, "Is")};
  auto const KaLoc{abcg::glGetUniformLocation(m_program, "Ka")};
  auto const KdLoc{abcg::glGetUniformLocation(m_program, "Kd")};
  auto const KsLoc{abcg::glGetUniformLocation(m_program, "Ks")};
  auto const diffuseTexLoc{abcg::glGetUniformLocation(m_program, "diffuseTex")};
  
  auto const lightDirRotated{m_lightDir};
  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);

  abcg::glUniform1i(diffuseTexLoc, 0);

  auto const modelViewMatrix{glm::mat3(viewMatrix * modelMatrix)};
  auto const normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);
  abcg::glUniform1f(shininessLoc, m_shininess);

  abcg::glBindVertexArray(m_VAO);

  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);

  // Set minification and magnification parameters
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Set texture wrapping parameters
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



  // Draw body triangles
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);


  // Draw body lines in darker color
  glm::vec4 line_color = {color[0] * 0.5f,color[1] * 0.5f,color[2] * 0.5f,1.0f};
  abcg::glUniform4fv(m_colorLoc, 1, &line_color[0]);
  abcg::glDrawElements(GL_LINES, m_indices.size(), GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  path.render();
}



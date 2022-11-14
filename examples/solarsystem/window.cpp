#include "window.hpp"

void Window::onEvent(SDL_Event const &event) {
  glm::ivec2 mousePosition;
  SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

  if (event.type == SDL_MOUSEWHEEL) {
    m_zoom += (event.wheel.y > 0 ? -1.0f : 1.0f) / 5.0f;
  }
}


void Window::onCreate() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);
  abcg::glEnable(GL_DEPTH_TEST);
  

  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "depth.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "depth.frag",
                                  .stage = abcg::ShaderStage::Fragment}});


  // create sun
  sun.name = "Sun";
  sun.scale = 0.3f;
  sun.color = {0.93f, 0.55f, 0.22f, 1.0f};
  sun.position = {0.0f,0.0f, 0.0f};
  sun.create(m_program);

  // create mercury
  Body mercury;
  mercury.name = "Mercury";
  mercury.scale = 0.1f;
  mercury.color = {0.33f, 0.33f, 0.34f, 1.0f};
  mercury.distance = 0.5f;
  mercury.satellite_of = &sun;
  planets.push_back(std::move(mercury));

  // create venus
  Body venus;
  venus.name = "Venus";
  venus.scale = 0.1f;
  venus.color = {0.48f, 0.38f, 0.14f, 1.0f};
  venus.distance = 1.0f;
  venus.satellite_of = &sun;
  planets.push_back(std::move(venus));

  // create earth
  Body earth;
  earth.name = "Earth";
  earth.scale = 0.1f;
  earth.color = {0.12f, 0.35f, 0.53f, 1.0f};
  earth.distance = 1.5f;
  earth.satellite_of = &sun;
  planets.push_back(std::move(earth));

  // create Mars
  Body mars;
  mars.name = "Mars";
  mars.scale = 0.1f;
  mars.color = {0.61f, 0.18, 0.21, 1.0f};
  mars.distance = 2.0f;
  mars.satellite_of = &sun;
  planets.push_back(std::move(mars));

  // create Jupiter
  Body jupiter;
  jupiter.name = "Jupiter";
  jupiter.scale = 0.2f;
  jupiter.color = {0.61f, 0.18, 0.21, 1.0f};
  jupiter.distance = 2.5f;
  jupiter.satellite_of = &sun;
  planets.push_back(std::move(jupiter));

  // create Saturn
  Body saturn;
  saturn.name = "Saturn";
  saturn.scale = 0.1f;
  saturn.color = {0.61f, 0.18, 0.21, 1.0f};
  saturn.distance = 3.0f;
  saturn.satellite_of = &sun;
  planets.push_back(std::move(saturn));

  // create Uranus
  Body uranus;
  uranus.name = "Uranus";
  uranus.scale = 0.1f;
  uranus.color = {0.29f, 0.35f, 0.36f, 1.0f};
  uranus.distance = 3.5f;
  uranus.satellite_of = &sun;
  planets.push_back(std::move(uranus));

  // create Neptune
  Body neptune;
  neptune.name = "Uranus";
  neptune.scale = 0.1f;
  neptune.color = {0.16f, 0.31f, 0.53f, 1.0f};
  neptune.distance = 4.0f;
  neptune.satellite_of = &sun;
  planets.push_back(std::move(neptune));

  for (auto &planet : planets){
    planet.create(m_program);
  }
  

  // create Moon
  Body moon;
  moon.name = "Moon";
  moon.scale = 0.1f;
  moon.color = {1.0f,1.0f,1.0f,1.0f};
  moon.distance = 0.3f;
  moon.satellite_of = &earth;
  moons.push_back(std::move(moon));


  for (auto &moon : moons){
    moon.create(m_program);
  }

  // fmt::print("{}\n",sun.m_vertices.size());

  // for(auto i: iter::range(sun.m_vertices.size())){
  //   fmt::print("x: {:.2f} y: {:.2f} z: {:.2f}\n",sun.m_vertices[i].position[0],sun.m_vertices[i].position[1],sun.m_vertices[i].position[2]);
  // }

}

void Window::onUpdate() {
  m_viewMatrix =
      glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f + m_zoom),
                  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  sun.update();
  for (auto &planet : planets){
    planet.update();
  }                
  // for (auto &moon : moons){
  //   moon.update();
  // }
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  // Get location of uniform variables
  auto const viewMatrixLoc{abcg::glGetUniformLocation(m_program, "viewMatrix")};
  auto const projMatrixLoc{abcg::glGetUniformLocation(m_program, "projMatrix")};

  // Set uniform variables that have the same value for every model
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);

  
  sun.render();
  for (auto &planet : planets){
    planet.render();
  }
  for (auto &moon : moons){
    moon.render();
  }


  abcg::glUseProgram(0);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();
  // Create a window for the other widgets
  {
    auto const widgetSize{ImVec2(222, 90)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - widgetSize.x - 5, 5));
    ImGui::SetNextWindowSize(widgetSize);
    ImGui::Begin("Widget window", nullptr, ImGuiWindowFlags_NoDecoration);

    auto const aspect{gsl::narrow<float>(m_viewportSize.x) / gsl::narrow<float>(m_viewportSize.y)};
    m_projMatrix = glm::perspective(glm::radians(45.0f), aspect, 1.0f, 100.0f);
    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_program);
}

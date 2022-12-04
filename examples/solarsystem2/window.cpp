#include "window.hpp"

void Window::onEvent(SDL_Event const &event) {
  glm::ivec2 mousePosition;
  SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_pedestalSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_pedestalSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_truckSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_truckSpeed = 1.0f;
  }
  if (event.type == SDL_KEYUP) {
    if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) && m_pedestalSpeed > 0)
      m_pedestalSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) && m_pedestalSpeed < 0)
      m_pedestalSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) && m_truckSpeed < 0)
      m_truckSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) && m_truckSpeed > 0)
      m_truckSpeed = 0.0f;
  }
  if (event.type == SDL_MOUSEMOTION) {
    m_camera.mouseMove(mousePosition);
  }
  if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
    m_camera.mousePress(mousePosition);
  }
  if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
    m_camera.mouseRelease(mousePosition);
  }

  if (event.type == SDL_MOUSEWHEEL) {
    m_camera.mouseScroll(event.wheel.preciseY);
  }


}


void Window::onCreate() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);
  abcg::glEnable(GL_DEPTH_TEST);
  

  auto const path{assetsPath + "shaders/"};
  m_program =
      abcg::createOpenGLProgram({{.source = path + "shader.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = path + "shader.frag",
                                  .stage = abcg::ShaderStage::Fragment}});


  glm::vec4 Ka = {0.2f,0.2f,0.2f,1.0f};
  glm::vec4 Kd = {1.0f,1.0f,1.0f,1.0f};
  glm::vec4 Ks = {0.1f,0.1f,0.1f,1.0f};

  // create sun
  sun.name = "Sun";
  sun.scale = 1.0f;
  sun.color = {0.93f, 0.55f, 0.22f, 1.0f};
  sun.texture_path = assetsPath + "maps/sun.jpg";
  sun.m_Ka = {1.0f,1.0f,1.0f,1.0f};
  sun.m_Kd = {1.0f,1.0f,1.0f,1.0f};
  sun.m_Ks = {1.0f,1.0f,1.0f,1.0f};
  sun.m_shininess = 100.0f;
  sun.satellite_of = nullptr;
  sun.create(m_program);

  // create mercury
  mercury.name = "Mercury";
  mercury.scale = 0.1f;
  mercury.color = {0.33f, 0.33f, 0.34f, 1.0f};
  mercury.texture_path = assetsPath + "maps/mercury.jpg";
  mercury.m_Ka = Ka;
  mercury.m_Kd = Kd;
  mercury.m_Ks = Ks;
  mercury.m_shininess = 100.0f;
  mercury.translation_speed = 1.0f/88.0f;
  mercury.rotation_speed = -1.0f/59.0f;
  mercury.orbit_radius = 1.5f;
  mercury.satellite_of = &sun;  
  mercury.create(m_program);

  // create venus
  venus.name = "Venus";
  venus.scale = 0.1f;
  venus.color = {0.48f, 0.38f, 0.14f, 1.0f};
  venus.texture_path = assetsPath + "maps/venus.jpg";
  venus.m_Ka = Ka;
  venus.m_Kd = Kd;
  venus.m_Ks = Ks;
  venus.m_shininess = 100.0f;
  venus.translation_speed = 1.0f/224.0f;
  venus.rotation_speed = 1.0f/243.0f;
  venus.orbit_radius = 2.0f;
  venus.satellite_of = &sun;
  venus.create(m_program);

  // create earth
  earth.name = "Earth";
  earth.scale = 0.1f;
  earth.color = {0.12f, 0.35f, 0.53f, 1.0f};
  earth.texture_path = assetsPath + "maps/earth_day.jpg";
  earth.m_Ka = Ka;
  earth.m_Kd = Kd;
  earth.m_Ks = Ks;
  earth.m_shininess = 100.0f;
  earth.translation_speed = 1.0f/365.0f;
  earth.rotation_speed = 1.0f;
  earth.orbit_radius = 2.5f;
  earth.satellite_of = &sun;
  earth.create(m_program);

  // create Mars
  mars.name = "Mars";
  mars.scale = 0.05f;
  mars.color = {0.61f, 0.18, 0.21, 1.0f};
  mars.texture_path = assetsPath + "maps/mars.jpg";
  mars.m_Ka = Ka;
  mars.m_Kd = Kd;
  mars.m_Ks = Ks;
  mars.m_shininess = 100.0f;
  mars.translation_speed = 1.0f/686.0f;
  mars.rotation_speed = 1.0f/1.03f;
  mars.orbit_radius = 3.0f;
  mars.satellite_of = &sun;
  mars.create(m_program);

  // create Jupiter
  jupiter.name = "Jupiter";
  jupiter.scale = 0.5f;
  jupiter.color = {0.76f, 0.70f, 0.50f, 1.0f};
  jupiter.texture_path = assetsPath + "maps/jupiter.jpg";
  jupiter.m_Ka = Ka;
  jupiter.m_Kd = Kd;
  jupiter.m_Ks = Ks;
  jupiter.m_shininess = 100.0f;
  jupiter.translation_speed = 1.0f/4344.0f;
  jupiter.rotation_speed = 1.0f/0.41f;
  jupiter.orbit_radius = 5.5f;
  jupiter.satellite_of = &sun;
  jupiter.create(m_program);

  // create Saturn
  saturn.name = "Saturn";
  saturn.scale = 0.4f;
  saturn.color = {0.38f, 0.38f, 0.25f, 1.0f};
  saturn.texture_path = assetsPath + "maps/saturn.jpg";
  saturn.m_Ka = Ka;
  saturn.m_Kd = Kd;
  saturn.m_Ks = Ks;
  saturn.m_shininess = 100.0f;
  saturn.translation_speed = 1.0f/10749.0f;
  saturn.rotation_speed = 1.0f/0.45f;
  saturn.orbit_radius = 7.5f;
  saturn.satellite_of = &sun;
  saturn.create(m_program);

  // create Uranus
  uranus.name = "Uranus";
  uranus.scale = 0.15f;
  uranus.color = {0.29f, 0.35f, 0.36f, 1.0f};
  uranus.texture_path = assetsPath + "maps/uranus.jpg";
  uranus.m_Ka = Ka;
  uranus.m_Kd = Kd;
  uranus.m_Ks = Ks;
  uranus.m_shininess = 100.0f;
  uranus.translation_speed = 1.0f/30660.0f;
  uranus.rotation_speed = 1.0f/0.72f;
  uranus.orbit_radius = 8.5f;
  uranus.satellite_of = &sun;
  uranus.create(m_program);

  // create Neptune
  neptune.name = "Uranus";
  neptune.scale = 0.15f;
  neptune.color = {0.16f, 0.31f, 0.53f, 1.0f};
  neptune.texture_path = assetsPath + "maps/neptune.jpg";
  neptune.m_Ka = Ka;
  neptune.m_Kd = Kd;
  neptune.m_Ks = Ks;
  neptune.m_shininess = 100.0f;
  neptune.translation_speed = 1.0f/60225.0f;
  neptune.rotation_speed = 1.0f/0.67f;
  neptune.orbit_radius = 9.0f;
  neptune.satellite_of = &sun;
  neptune.create(m_program);

  
  // create Moon
  moon.name = "Moon";
  moon.scale = 0.012f;
  moon.color = {1.0f,1.0f,1.0f,1.0f};
  moon.texture_path = assetsPath + "maps/moon.jpg";
  moon.m_Ka = Ka;
  moon.m_Kd = Kd;
  moon.m_Ks = Ks;
  moon.m_shininess = 100.0f;
  moon.translation_speed = 1.0f/27.3220f;
  moon.rotation_speed = 1.0f/27.0f;
  moon.orbit_radius = 0.2f;
  moon.satellite_of = &earth;
  moon.create(m_program);

  // create skydome
  skydome.texture_path = assetsPath + "maps/8k_stars_milky_way.jpg";
  skydome.create(abcg::createOpenGLProgram({{.source = path + "skydome.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = path + "skydome.frag",
                                  .stage = abcg::ShaderStage::Fragment}}));

}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};
  
  sun.update(deltaTime,m_rotation_speed,m_translation_speed);
  mercury.update(deltaTime,m_rotation_speed,m_translation_speed);
  venus.update(deltaTime,m_rotation_speed,m_translation_speed);
  earth.update(deltaTime,m_rotation_speed,m_translation_speed);
  mars.update(deltaTime,m_rotation_speed,m_translation_speed);
  jupiter.update(deltaTime,m_rotation_speed,m_translation_speed);
  saturn.update(deltaTime,m_rotation_speed,m_translation_speed);
  neptune.update(deltaTime,m_rotation_speed,m_translation_speed);
  uranus.update(deltaTime,m_rotation_speed,m_translation_speed);
  moon.update(deltaTime,m_rotation_speed,m_translation_speed);

  // Update LookAt camera
  m_camera.pedestal(m_pedestalSpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  // Get location of uniform variables
  auto const viewMatrixLoc{abcg::glGetUniformLocation(m_program, "viewMatrix")};
  auto const projMatrixLoc{abcg::glGetUniformLocation(m_program, "projMatrix")};

  // Set uniform variables that have the same value for every model
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.getProjMatrix()[0][0]);

  sun.render(m_camera.getViewMatrix());
  mercury.render(m_camera.getViewMatrix());
  venus.render(m_camera.getViewMatrix());
  earth.render(m_camera.getViewMatrix());
  mars.render(m_camera.getViewMatrix());
  jupiter.render(m_camera.getViewMatrix());
  saturn.render(m_camera.getViewMatrix());
  neptune.render(m_camera.getViewMatrix());
  uranus.render(m_camera.getViewMatrix());
  moon.render(m_camera.getViewMatrix());

  abcg::glUseProgram(0);

  skydome.render(m_camera.getViewMatrix(), m_camera.getProjMatrix());
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();
  // Create window for slider
  {
    ImGui::SetNextWindowPos(ImVec2(5, 100));
    ImGui::SetNextWindowSize(ImVec2(200, -1));
  
    ImGui::Begin("Movement Controls", nullptr,ImGuiWindowFlags_NoResize);

    // Create a slider to control the number of rendered triangles
    {
      // Slider will fill the space of the window
      ImGui::PushItemWidth(-1);
      ImGui::PushID(1);
      ImGui::SliderFloat("", &m_rotation_speed, 0.0f, 10.0f,"%.1fx Rotation speed");
      ImGui::PopID();
      
      ImGui::PushID(2);
      ImGui::SliderFloat("", &m_translation_speed, 0.0f, 10.0f,"%.1fx Translation speed");
      ImGui::PopID();
      ImGui::PopItemWidth();
      
    }

    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(5, 300));
    ImGui::SetNextWindowSize(ImVec2(200, 120));
  
    ImGui::Begin("Lighting Controls", nullptr,ImGuiWindowFlags_NoResize);

    // Create a slider to control the number of rendered triangles
    {
      // Slider will fill the space of the window
      ImGui::PushID(1);
      ImGui::SliderFloat("", &m_rotation_speed, 0.0f, 10.0f,"%.1fx Rotation speed");
      ImGui::PopID();
      
      ImGui::PushID(2);
      ImGui::SliderFloat("", &m_translation_speed, 0.0f, 10.0f,"%.1fx Translation speed");
      ImGui::PopID();

    }

    ImGui::End();
  }

}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_camera.resizeViewport(size);
  m_camera.computeProjectionMatrix(size);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_program);
}

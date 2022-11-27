#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"
#include "body.hpp"
#include "camera.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  glm::ivec2 m_viewportSize{};

  Camera m_camera;

  Body sun, mercury, venus, earth, mars, jupiter, saturn, neptune, uranus, moon;

  float m_pedestalSpeed{};
  float m_truckSpeed{};
  float speed{1.0f};
  
  GLuint m_program{};
};

#endif

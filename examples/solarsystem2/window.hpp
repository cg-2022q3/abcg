#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"
#include "body.hpp"
#include "camera.hpp"
#include "skydome.hpp"

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

  Skydome skydome;

  float m_pedestalSpeed{};
  float m_truckSpeed{};
  
  float m_rotation_speed{0.0f};
  float m_translation_speed{0.0f};

  bool cbSol = false;
  bool cbMer = false;
  bool cbVen = false;
  bool cbTer = false;
  bool cbMar = false;
  bool cbJup = false;
  bool cbSat = false;
  bool cbUra = false;
  bool cbNet = false;

  GLuint m_program{};
};

#endif

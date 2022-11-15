#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <limits>

void Camera::mouseMove(glm::ivec2 const &position) {
  // if (!m_mouseTracking)
  //   return;

  // auto const currentPosition{project(position)};

  // // Rotation axis
  // m_axis = glm::cross(m_lastPosition, currentPosition);

  // // Rotation angle
  // auto const angle{glm::length(m_axis)};

  // m_axis = glm::normalize(m_axis);

  // // Concatenate rotation: R_old = R_new * R_old
  // m_rotation = glm::rotate(glm::mat4(1.0f), angle, m_axis) * m_rotation;

  // m_lastPosition = currentPosition;
}

void Camera::mousePress(glm::ivec2 const &position) {
  // m_rotation = getRotation();
  // m_mouseTracking = true;
  // m_lastTime.restart();
  // m_lastPosition = project(position);
  // m_velocity = 0.0f;
}

void Camera::mouseRelease(glm::ivec2 const &position) {
  // mouseMove(position);
  // m_mouseTracking = false;
  // m_velocity = 0.0f;
}

void Camera::resizeViewport(glm::ivec2 const &size) {
  // m_viewportSize = size;
}


void Camera::computeProjectionMatrix(glm::vec2 const &size) {
  m_projMatrix = glm::mat4(1.0f);
  auto const aspect{size.x / size.y};
  m_projMatrix = glm::perspective(glm::radians(45.0f), aspect, 1.0f, 100.0f);
}

void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

void Camera::dolly(float speed) {
  // Compute forward vector (view direction)
  auto const forward{glm::normalize(m_at - m_eye)};

  // Move eye and center forward (speed > 0) or backward (speed < 0)
  m_eye += forward * speed;
  m_at += forward * speed;

  computeViewMatrix();
}

void Camera::truck(float speed) {
  // Compute forward vector (view direction)
  auto const forward{glm::normalize(m_at - m_eye)};
  // Compute vector to the left
  auto const left{glm::cross(m_up, forward)};

  // Move eye and center to the left (speed < 0) or to the right (speed > 0)
  m_at -= left * speed;
  m_eye -= left * speed;

  computeViewMatrix();
}
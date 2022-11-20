#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <limits>

void Camera::mouseMove(glm::ivec2 const &position) {
  if (!m_mouseTracking) return;

  auto const currentPosition{project(position)};

  if (m_lastPosition == currentPosition) return;

  // Rotation axis
  m_axis = glm::cross(m_lastPosition, currentPosition);

  // Rotation angle
  auto const angle{glm::length(m_axis)};

  m_axis = glm::normalize(m_axis);

  // Concatenate rotation: R_old = R_new * R_old
  m_rotation = glm::rotate(glm::mat4(1.0f), angle, m_axis);

  m_at = (m_at - m_eye) * (glm::mat3) m_rotation + m_eye;

  m_lastPosition = currentPosition;

} 

void Camera::mousePress(glm::ivec2 const &position) {  
  m_lastPosition = project(position);
  m_mouseTracking = true;
}

void Camera::mouseRelease(glm::ivec2 const &position) {
  mouseMove(position);
  m_mouseTracking = false;
}

void Camera::resizeViewport(glm::ivec2 const &size) {
  m_viewportSize = size;
}

void Camera::computeProjectionMatrix(glm::vec2 const &size) {
  m_projMatrix = glm::mat4(1.0f);
  auto const aspect{size.x / size.y};
  m_projMatrix = glm::perspective(glm::radians(45.0f), aspect, 1.0f, 100.0f);
}

void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

void Camera::pedestal(float speed) {
  // Compute up vector
  auto const up{glm::normalize(m_up)};

  // Move eye and center upward (speed > 0) or downward (speed < 0)
  m_eye += up * speed;
  m_at += up * speed;

  computeViewMatrix();
}

void Camera::truck(float speed) {
  // Compute forward vector (view direction)
  auto const forward{glm::normalize(m_at - m_eye)};
  // Compute vector to the left
  auto const left{glm::cross(m_up, forward)};

  // Move eye and center to the left (speed < 0) or to the right (speed > 0)
  m_eye -= left * speed;
  m_at -= left * speed;

  computeViewMatrix();
}


glm::vec3 Camera::project(glm::vec2 const &position) const {
  // Convert from window coordinates to NDC
  auto projected{glm::vec3(
      2.0f * position.x / gsl::narrow<float>(m_viewportSize.x) - 1.0f,
      1.0f - 2.0f * position.y / gsl::narrow<float>(m_viewportSize.y), 0.0f)};

  // Project to centered unit hemisphere
  if (auto const squaredLength{glm::length2(projected)};
      squaredLength >= 1.0f) {
    // Outside the sphere
    projected = glm::normalize(projected);
  } else {
    // Inside the sphere
    projected.z = std::sqrt(1.0f - squaredLength);
  }

  return projected;
}
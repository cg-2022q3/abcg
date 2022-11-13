#ifndef SUN_HPP_
#define SUN_HPP_

#include "abcgOpenGL.hpp"
#include "body.hpp"
#include "planet.hpp"

class Sun: public Body {
public:
  std::string name{""};

  std::vector<Planet> planets{};
};

#endif
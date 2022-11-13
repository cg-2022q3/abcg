#ifndef PLANET_HPP_
#define PLANET_HPP_

#include "abcgOpenGL.hpp"
#include "moon.hpp"

class Planet: public Body {
public:
  std::string name{""};

  std::vector<Moon> moons{};
};

#endif
#ifndef PLANET_HPP_
#define PLANET_HPP_

#include "abcgOpenGL.hpp"
#include "body.hpp"

class Planet: protected Body {
protected:
  std::string name{""};

  std::list<Body> satellites{};
};

#endif
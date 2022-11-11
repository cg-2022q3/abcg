#ifndef MOON_HPP_
#define MOON_HPP_

#include "abcgOpenGL.hpp"
#include "body.hpp"

class Moon: protected Body {
protected:
  std::string name{""};

};

#endif
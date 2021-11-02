// include standart headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

// Include GLEW, GLFW & GLM
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

//include the threaded clock
#include "customheaders/pclock.hpp"
// include terminal
#include "customheaders/terminal.hpp"
// include the base window function
//#include "basewindow.hpp"
#include "customheaders/renderer.hpp"
// include gamescript
#include "mainloop.hpp"

using namespace glm;

int main(void)
{
  // error-code
  int err;
  // initialize the terminal here
  TERMINAL term;
  // load the gamescript
  MAINLOOP New;
  // start the gamescript & catch a potential error
  err = New.start(term);

  return 0;
}

#ifndef MAINLOOP_HPP
#define MAINLOOP_HPP

// include standart headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <future>

#include <chrono>

// test the clock
#include "customheaders/Clockclass.hpp"
#include "customheaders/noisegenerator.hpp"


class MAINLOOP
{
  public:
    clogger c;
    NoiseGenerator noise;
    RenderTarget Win;

    bool terminate = false;

  int start(TERMINAL &term)
  {

    //noise:
    noise.setwidth(1024);
    noise.range = 400;
    noise.octaves = 7;
    noise.oceanfract = 1.f/7;
    noise.rdmseed = 87469;
    noise.generateseed();
    std::cout << "seed creation complete (" << noise.rdmseed << ")\n";
    
    noise.aplynoisemap();
    std::cout << "harmonisation of depthdata complete\n";

    //noise.noisemap.swap(noise.seed);

    //noise.mesh();
    noise.positionbasedmesh(0, 0, 700);
    //noise.positionbasedweirdmesh(0, 0, 512);
    //noise.generateweirdmesh();
    std::cout << "mesh generation complete\n";
    std::cout << "there are "<< noise.getwidth() * 4 << " triangles currently loaded" << std::endl;

    //create window
    Win.INIT(1920, 1080, true); //width, hight, fullscreen
    Win.setShaders("shaders/TransformVertexShader.vertexshader", "shaders/ColorFragmentShader.fragmentshader");
    Win.bindBuffers(&noise.vertecies, &noise.colours);

    //std::thread first (&MAINLOOP::secondtest, this);
    //pClockbegin(&test, 16);

    Win.Draw();
    terminate = true;

    //first.join();

    //pClockend();

    return 0;
  }

  int test()
  {
    std::cout << "I am a test to see whether or not this draw thing works" << std::endl;

    std::cout << "create new mesh"<< std::endl;
    //noise.mesh();
    //noise.noisemap.swap(noise.seed);
    //noise.generateweirdmesh();
    glm::mat4 ViewMatrix = Win.CamCon.getViewMatrix();
    std::cout << "position data: " << ViewMatrix[0][0] << std::endl;
    std::cout << "binding new mesh" << std::endl;

    std::cout << "binding complete return normal opperation" << std::endl;
    std::cout << "buffer swapped in 10 second" << std::endl;

    return 0;
  }

  int secondtest()
  {
      int oldx = (int)Win.CamCon.position[0];
      int oldy = (int)Win.CamCon.position[2];
      int maxdist = 600;

      do
      {
        glm::vec3 posi = Win.CamCon.position;

        if(std::abs((int)posi[0] - oldx) > maxdist / 4 || std::abs((int)posi[2] - oldy) > maxdist / 4)
        {
            noise.positionbasedmesh((int)posi[0], (int)posi[2], maxdist);
            //noise.positionbasedweirdmesh((int)posi[0], (int)posi[2], 500);
            Win.changeBufferData(&noise.vertecies, &noise.colours);

            oldx = (int)posi[0];
            oldy = (int)posi[2];
        }
        sleepcp(500);

      }while(terminate == false);

      return 0;
  }

};

#endif
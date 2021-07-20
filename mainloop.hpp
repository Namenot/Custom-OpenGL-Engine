// include standart headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

// test the clock
#include "Clockclass.hpp"
#include "noisegenerator.hpp"


class MAINLOOP
{
  public:
    clogger c;
    NoiseGenerator noise;
    RenderTarget Win;

  int start(TERMINAL &term)
  {
    // test (inheritance)
    term.test = 10;
    std::cout << term.test << std::endl;
    pClockbegin(c, 16);
    pClockbegin(c, 32);
    pClockend();
    // end test

    //noise:
    noise.width = 2048;
    noise.range = 700;
    noise.octaves = 5;
    noise.oceanfract = 1.f/7;
    noise.generateseed();
    std::cout << "seed creation complete\n";

    noise.aplynoisemap();
    std::cout << "harmonisation of depthdata complete\n";

    //noise.noisemap.swap(noise.seed);

    noise.mesh();
    //noise.generateweirdmesh();
    std::cout << "mesh generation complete\n";
    std::cout << "there are "<< noise.width * 4 << " triangles currently loaded" << std::endl;

    //create window
    Win.INIT(int(720 * (16.f/9)), 720, false);
    Win.setShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");
    Win.bindBuffers(&noise.vertecies, &noise.colours);

    //std::thread first (test, this);

    Win.Draw();

    //first.join();

    return 0;
  }

  int test()
  {
    std::cout << "I am a test to see whether or not this draw thing works" << std::endl;

    std::cout << "create new mesh"<< std::endl;
    noise.mesh();
    //noise.noisemap.swap(noise.seed);
    //noise.generateweirdmesh();
    std::cout << "binding new mesh" << std::endl;
    sleepcp(5000);
    Win.changeBufferData(&noise.vertecies, &noise.colours);
    std::cout << "binding complete return normal opperation" << std::endl;
    std::cout << "buffer swapped in 10 second" << std::endl;

    return 0;
  }

};

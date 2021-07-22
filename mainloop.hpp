// include standart headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

#include <chrono>

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
    noise.setwidth(512);
    noise.range = 350;
    noise.octaves = 11;
    noise.oceanfract = 1.f/7;
    noise.rdmseed = 123;
    //noise.randomizeseed();
    noise.generateseed();
    std::cout << "seed creation complete (" << noise.rdmseed << ")\n";

    //noise.aplynoisemap();
    std::cout << "harmonisation of depthdata complete\n";

    //noise.noisemap.swap(noise.seed);

    //noise.mesh();
    //noise.positionbasedmesh(0, 0, 500);
    noise.positionbasedweirdmesh(0, 0, 500);
    //noise.generateweirdmesh();
    std::cout << "mesh generation complete\n";
    std::cout << "there are "<< noise.getwidth() * 4 << " triangles currently loaded" << std::endl;

    //create window
    Win.INIT(1920, 1080, true);
    Win.setShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");
    Win.bindBuffers(&noise.vertecies, &noise.colours);

    std::thread first (secondtest, this);

    //pClockbegin(&test, 16);

    Win.Draw();

    first.join();
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
    //sleepcp(5000);
    //Win.changeBufferData(&noise.vertecies, &noise.colours);
    std::cout << "binding complete return normal opperation" << std::endl;
    std::cout << "buffer swapped in 10 second" << std::endl;

    return 0;
  }

    int secondtest()
    {
        do
        {
            glm::vec3 posi = Win.CamCon.position;

            std::cout << "pos : " << (int)posi[0] << " : " << (int)posi[0] << std::endl;

            noise.positionbasedmesh((int)posi[0], (int)posi[2], 200);
            //noise.positionbasedweirdmesh((int)posi[0], (int)posi[2], 200);
            Win.changeBufferData(&noise.vertecies, &noise.colours);

            //sleepcp(100);
        }while(true);
    }

};

// include standart headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <future>

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

    int terminate = 0;

  int start(TERMINAL &term)
  {
    // test (inheritance)
    term.test = 10;
    std::cout << term.test << std::endl;
    pClockbegin(c, 16);
    pClockbegin(c, 32);
    pClockend();
    loopthrough();
    // end test

    //noise:
    noise.setwidth(2048);
    noise.range = 600;
    noise.octaves = 7;
    noise.oceanfract = 1.f/7;
    noise.rdmseed = 234;
    //noise.randomizeseed();
    noise.generateseed();
    std::cout << -2049 % 2048 << std::endl;
    sleepcp(5000);
    std::cout << "seed creation complete (" << noise.rdmseed << ")\n";

    auto Tbegin = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    noise.aplynoisemap();
    auto Tend = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    std::cout << "the new method took : " << Tend - Tbegin << "ms to complete" << std::endl;
    std::cout << "harmonisation of depthdata complete\n";

    //noise.noisemap.swap(noise.seed);

    //noise.mesh();
    noise.positionbasedmesh(0, 0, 500);
    //noise.positionbasedweirdmesh(0, 0, 512);
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

    terminate = 1;

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
        int oldx = (int)Win.CamCon.position[0];
        int oldy = (int)Win.CamCon.position[2];
        int maxdist = 800;

        do
        {
            glm::vec3 posi = Win.CamCon.position;

            //std::cout << "pos : " << (int)posi[0] << " : " << (int)posi[2] << std::endl;

            if(std::abs((int)posi[0] - oldx) > maxdist / 4 || std::abs((int)posi[2] - oldy) > maxdist / 4)
            {
                noise.positionbasedmesh((int)posi[0], (int)posi[2], maxdist);
                //noise.positionbasedweirdmesh((int)posi[0], (int)posi[2], 500);
                Win.changeBufferData(&noise.vertecies, &noise.colours);

                oldx = (int)posi[0];
                oldy = (int)posi[2];
                sleepcp(500);
            }

        }while(terminate == 0);

        return 0;
    }

    void loopthrough()
    {
        std::cout << "creating the array\n";
        int len = 1000000000;
        std::vector<int> test;
        for(int i = 0; i < len; ++i)
            test.push_back(i);
        std::cout << "array created and filled\n";

        for(int i = 0; i < len; ++i)
        {
            test[i+1] += test[i];
        }

        std::cout << "the last digit of test is: " << test.back() << std::endl;

    }

};

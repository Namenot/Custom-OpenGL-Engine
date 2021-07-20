#include <iostream>
#include <cstddef> // std::size_t
#include<bits/stdc++.h>
#include <time.h>
#include <chrono>
#include <stdlib.h>
#include <iterator>
#include <vector>
#include <cmath>
#include <map>
#include <string>
#include <sstream>

struct polar3d
{
  unsigned int id;
  unsigned int vertical;
  unsigned int horizontal;
  unsigned int distance;
};

struct map3d
{
    double pos[3]; // (x/y/z)
    double color[4]; // [r, g, b, alpha]
    int adjacent[3]; //made off the id's of the 3 adjacent Triangles
};

class MeshProcessor
{
private:
    int calcdegree()
    {

        return 0;
    }

public:



    //this should be put into a constructor so that a change of degree is possible at creation
    unsigned int degree = 360; //this is the precision at which each point is being calculated
    std::array<std::array<std::vector<unsigned int>, degree>, degree> polar3darray;

    /*this  should be able to do the following things:
    1.0) sort an array of points into the polar3darray using bucket sort for the 360 degrees and radix sort for the vectors

    2.1) be able to extract a section of the sorted array
    2.2) be able to set a roughness factor that filters points out, the further they are away based on how much space is necessary
        .> roughness works based on a perceived distance between 2 objects and whether the distance would be distinguishable from a simple point
    2.3) be able to set a max size for that array to make sure that the gpu doesnt suffer too much

    3.1) be compatible with my noise generator
    3.2) generate a proper mesh from vertecies that can then be passed to the render function
    */

    void colapseedge()
    {
        //this functions sole function is to colabse a single edge
        //and remove a
    }

    void rateedge()
    {
        //gives each edge a score based rating,
        //depending on factors like:
        // - distance to the player
        // - player looking direction
        // - number of adjacent triangles to the edge
        // - difference between normal vectors of adjacent triangles to the edge
    }

    void
};

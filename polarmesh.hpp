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

struct point3d
{
    std::array<GLfloat, 3> coordinates; // x / y / z coordinate of the point

    std::vector<edge3d *> edges;        // every edge that is connected to this point
    std::vector<face3d *> triangles;    // every triangle this point is a part of

};

struct edge3d
{
    float score = 1;                   // scores the edge in terms of its collapsibility

    std::array<point3d *, 2> points;   // both points which together make the edge
    std::array<face3d *, 2> triangles; // both triangles adjacent to the edge

};

struct face3d
{
    std::array<point3d *, 3> points; // the 3 points that make the triangle

};

class MeshSorter
{
/*
works by sorting every point by its distance to 3 root points in space
1.0) sort each point by its distance to all 3 points
1.1) each points gets a "strong" and a "weak" distance assigned (one is precise and the other is set to a fixed radial "ring" with origin (1, 2, 3) as its center)

2.0) hash function:
2.1) set a definitive hash for each point (probably just its index)
2.3) pick a point in space and calculate its distance to each point

*/
private:

    std::array<GLfloat, 3> origin = {0,0,0};
    std::array<GLfloat, 3> renderposition; //position arround which you want to draw

    std::vector<std::array<GLfloat, 4>> points; //distance to origin, x, y, z

    GLfloat calc_distance(std::array<GLfloat, 3> p1, std::array<GLfloat, 3> p2)
    {
        return std::sqrt(std::pow(p1[0] - p2[0]) + std::pow(p1[1] - p2[1]) + std::pow(p1[2] - p2[2]));
    }

    void sort_mesh()
    {
        /*
        radix sort points by there distance to origin
        */
    }

public:
    MeshSorter()
    {

    }

    void set_origin(GLfloat x, GLfloat y, GLfloat z)
    {
        origin[0] = x;
        origin[1] = y;
        origin[2] = z;
        sort_mesh();
    }

};

class MeshProcessor
{
private:
    int calcdegree()
    {

        return 0;
    }

public:

    MeshProcessor()
    {

    }


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

    void scoreedge(edge3d * edg)
    {
        //gives each edge a score based rating,
        //depending on factors like:
        // - distance to the player
        // - player looking direction
        // - number of adjacent triangles to the edge
        // - difference between normal vectors of adjacent triangles to the edge
    }

};

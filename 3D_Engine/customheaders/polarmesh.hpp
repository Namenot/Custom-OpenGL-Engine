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

typedef struct
{
    std::array<GLfloat, 3> coordinates; // x / y / z coordinate of the point

    std::vector<edge3d *> edges;        // every edge that is connected to this point
    std::vector<face3d *> triangles;    // every triangle this point is a part of

}point3d;

typedef struct
{
    float score = 1;                   // scores the edge in terms of its collapsibility

    std::array<point3d *, 2> points;   // both points which together make the edge
    std::array<face3d *, 2> triangles; // both triangles adjacent to the edge

}edge3d;

typedef struct
{
    std::array<point3d *, 3> points; // the 3 points that make the triangle

}face3d;

class MeshSorter
{
/*
works by sorting every point by its distance to 3 root points in space
1.0) sort each point by its distance to all 3 points
*/
private:

    std::array<GLfloat, 3> origin = {0,0,0};
    std::array<GLfloat, 3> renderposition = {0,0,0}; //position arround which you want to draw

    std::vector<GLfloat> *rectangles; //x, y, z
    std::vector<GLfloat> output;

    bool issorted = false;

    GLfloat calc_dist(std::array<GLfloat, 9> rect, std::array<GLfloat, 3> opoint)
    {
        //calculate the weighted center of the rectangel by averaging the axis
        std::array<GLfloat, 3> midpoint;
        midpoint[0] = (rect[0] + rect[3] + rect[6]) / 3;
        midpoint[1] = (rect[1] + rect[4] + rect[7]) / 3;
        midpoint[2] = (rect[2] + rect[5] + rect[8]) / 3;

        //return the distance between the midpoint and the point of interrest
        return std::sqrt(std::pow(midpoint[0] - opoint[0]) + std::pow(midpoint[1] - opoint[1]) + std::pow(midpoint[2] - opoint[2]));
    }

public:

    void set_origin(GLfloat x, GLfloat y, GLfloat z)
    {
        origin[0] = x;
        origin[1] = y;
        origin[2] = z;
        sort_mesh();
    }

    void set_rederpos(GLfloat x, GLfloat y, GLfloat z)
    {
        renderposition[0] = x;
        renderposition[1] = y;
        renderposition[2] = z;
    }

    void sort_mesh()
    {
        //create and fill an array that will be used as a swapable pointer to sort in
        std::vector<GLfloat> SorterArray;
        SorterArray.resize(rectangles->size());

        //sort the distances
        size_t length = 512;
        int *prefixarray = new int[length]();

        //create an element list as a hack in order to not change the calc_dist function
        std::array<GLfloat, 9> elem = {};

        //whats the biggest element in the list
        int maxElem = 0;
        int elemdist;
        for(int i = 0; i < rectangles->size()-1; i+=9)
        {
            for(int k = 0; k < 9; ++k)
                elem[k] = rectangles->at(i+k);

            elemdist = int(calc_dist(elem, origin));
            maxElem = (maxElem < elemdist) ? elemdist : maxElem;
        }

        //how many times is it devisible by length
        int iterations = 0;
        for(;maxElem > std::pow(length, iterations); ++iterations);


        int power = 1;
        for(int i = 0; i < iterations; ++i)
        {
            //filling the prefixarray array with a length based prefix
            for(int i = 0; i < rectangles->size()-1; i+=9)
            {
                for(int k = 0; k < 9; ++k)
                    elem[k] = rectangles->at(i+k);

                int index = (calc_dist(elem, origin) % int(std::pow(length, power))) / int(std::pow(length, power-1));
                prefixarray[index] += 9;
            }

            //add the prefixs together
            for(int i = 1; i < length; ++i)
            {
                  prefixarray[i] += prefixarray[i-1];
            }

            //rebuild into DistanceArray from back to front
            for(int i = SorterArray.size()-1; i >= 0; i -= 9) //this implicitly accepts the fact that each vector of rectangles must be divisible by 9
            {
                for(int k = 0; k < 9; ++k)
                    elem[8-k] = rectangles->at(i-k);

                int index = (calc_dist(elem, origin) % int(std::pow(length, power))) / int(std::pow(length, power-1));

                for(int k = 0; k < 9; ++k)
                {
                    prefixarray[index] --;
                    SorterArray[prefixarray[index]] = rectangles->at(i-k);
                }
            }

            //swap DistanceArray with SorterArray
            rectangles->swap(SorterArray);
            //increase the power
            power ++;
            //reset the prefixarray
            prefixarray = new int[length]();
        }
    }

    void get_vertecies(GLfloat radius)
    {
        /*fills the output vector and returns it with the apropriate data*/
        GLfloat dist_O_renderpos = std::sqrt(std::pow(renderposition[0] - origin[0]) + std::pow(renderposition[1] - origin[1]) + std::pow(renderposition[2] - origin[2]));

        //THIS IS KINDA STUPID AND NEEDS TO CHANGE
        //this finds the indices for the two furthest points that fit the distance in comparison to Origin
        int lowidx = 0;
        int highidx = 0;

        std::array<GLfloat, 9> elem = {};
        GLfloat distcheck;

        for(int i = 0; i < rectangles->size() - 1; i+=9)
        {
            for(int k = 0; k < 9; ++k)
                elem[k] = rectangles->at(i+k);

            distcheck = calc_dist(elem, origin);

            if(distcheck > dist_O_renderpos + radius)
                break;

            if(distcheck < dist_O_renderpos - radius)
                lowidx += 9;

            highidx += 9;

        }

        //this goes through the points in the selected range and checks each on whether its is also in range to the render position
        for(int i = lowidx; i < highidx; i+=9)
        {
            for(int k = 0; k < 9; ++k)
                elem[k] = rectangles->at(i+k);

            GLfloat check = calc_dist(elem, renderposition);

            if(check <= radius)
            {
                for(auto & e : elem)
                    output.push_back(e);
            }
        }

    }



};
/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
class MeshProcessor
{
private:
    int calc_edge_degree()
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

    void collapse_edge()
    {
        //this functions sole function is to collapse a single edge
        //and remove a
    }

    void score_edge(edge3d * edg)
    {
        //gives each edge a score based rating,
        //depending on factors like:
        // - distance to the player
        // - player looking direction
        // - number of adjacent triangles to the edge
        // - difference between normal vectors of adjacent triangles to the edge
    }

};

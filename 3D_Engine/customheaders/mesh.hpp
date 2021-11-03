#ifndef MESH_HPP
#define MESH_HPP

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

class mapmeshdata
{
public:

  std::vector<polar3d> DistanceArray = {};

    /*

    */
    void PasteObject(std::vector<polar3d> *in, polar3d obj) //rewrite this and split it into find and insert
    {
        std::size_t length = {in->size()};

        int UpperBound = (length) + (-1 * (length > 1)); //by default the index of the highest integer in the array
        int LowerBound = 0;

        float probe;
        int roundup;

        if(length == 1)
        {
            if(in->back().distance > obj.distance) //if the last array is bigger than our new element inster at 0
            UpperBound = 0;
        }

        if(length >= 2)
        {
            if (in->front().distance >= obj.distance) //wie?!
            {
                UpperBound = LowerBound;
            }

            if (in->back().distance <= obj.distance)
            {
                UpperBound = length;
            }else
            {
                do
                {
                    probe = float(UpperBound - (LowerBound+(UpperBound > 1))) / 2;
                    roundup = int(probe) + (probe > int(probe));

                    LowerBound += roundup * (in->at(LowerBound + roundup).distance < obj.distance);
                    UpperBound = UpperBound - (UpperBound * (in->at(LowerBound + roundup).distance > obj.distance)) + ((LowerBound + roundup) * (in->at(LowerBound + roundup).distance > obj.distance));

                    if(in->at(LowerBound + roundup).distance == obj.distance)
                    {
                        LowerBound += roundup;
                        UpperBound = LowerBound;
                    }

                }while (LowerBound+1 < UpperBound);
            }
        }

        in->insert(in->begin() + UpperBound, obj);
    }

/*
this is usefull for collision detection aswell
as for rough highspeed / teleport compensation / portals and
anything that has the potential to leave a loaded area
*/
    std::vector<polar3d> findObjects(std::vector<polar3d> *in, float radius, double pos[3])
    {
        std::vector<polar3d> x;
        return x;
    }

    void SortPolar3Dvector(std::vector<polar3d> *in)
    {
        //create and fill an array that will be used as a swapable pointer to sort in
        std::vector<polar3d> SorterArray;
        SorterArray.resize(in->size());

        //sort the distances
        size_t length = 512;
        int *prefixarray = new int[length]();

        //whats the biggest element in the list
        int maxElem = 0;
        for(auto & elem : *(in))
          {maxElem = (maxElem * (maxElem > elem.distance)) + ((maxElem <= elem.distance) * elem.distance);}

        //how many times is it devisible by length
        int iterations = 0;
        for(;maxElem > std::pow(length, iterations); ++iterations);


        int power = 1;
        for(int i = 0; i < iterations; ++i)
        {
            //filling the prefixarray array with a length based prefix
            for(auto & elem : *(in))
            {
                int index = (elem.distance % int(std::pow(length, power))) / int(std::pow(length, power-1));
                prefixarray[index] ++;
            }

            //add the prefixs together
            for(int i = 1; i < length; ++i)
            {
                  prefixarray[i] += prefixarray[i-1];
            }

            //rebuild into DistanceArray from back to front
            for(int i = SorterArray.size()-1; i >= 0; --i)
            {
                int index = (in->at(i).distance % int(std::pow(length, power))) / int(std::pow(length, power-1));
                prefixarray[index] --;
                SorterArray[prefixarray[index]] = in->at(i);
            }

            //swap DistanceArray with SorterArray
            in->swap(SorterArray);
            //increase the power
            power ++;
            //reset the prefixarray
            prefixarray = new int[length]();
        }

        int polarlen = 360;
        int *polars = new int[polarlen](); // 0� = polarlen�

        for(auto & elem : *(in))
        {
            polars[elem.horizontal % polarlen]++;
        }

        for(int i = 1; i < polarlen; ++i)
        {
            polars[i] += polars[i-1];
        }

        for(int i = in->size()-1; i >= 0; --i)
        {
            polar3d elem = in->at(i);
            polars[elem.horizontal % polarlen] --;
            SorterArray[polars[elem.horizontal % polarlen]] = elem;
        }

        //repeat what happend above for the verticals
        //maybe loop this as it is repetativ code and makes things harder to read
        in->swap(SorterArray);

        //this  is repetativ but doesnt make it slower so it should be fine
        polars = new int[polarlen]();

        for(auto & elem : *in)
        {
            polars[elem.vertical % polarlen]++;
        }

        for(int i = 1; i < polarlen; ++i)
        {
            polars[i] += polars[i-1];
        }

        for(int i = in->size()-1; i >= 0; --i)
        {
            polar3d elem = in->at(i);
            polars[elem.vertical % polarlen] --;
            SorterArray[polars[elem.vertical % polarlen]] = elem;
        }

        //finaly clean up the arrays and set DistanceArray to the main array
        delete[] polars;
        delete[] prefixarray;

        in->swap(SorterArray);
        SorterArray.erase(SorterArray.begin(), SorterArray.end());
    }

};

#endif

/*
i want to be able to :
	find an objects relative position for both its index in the array aswell as its relativ distance to spawn
	to add and delete objects with out much computational overhead

i have:

	the objects position relativ to Origin for 3 different Origins as a number
	the objects actual position in space (x,y,z)


Maps in cpp:

basically a dict, <int, int>

	<distance from O1, O2, O3 as a vector | pos in array>

	on pos change:
		delete key on distance change and add a new version + updated pos in array

Arrays in cpp:

	std::copy can copy parts of an array (useful to delete items efficiently)

*/

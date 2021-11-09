#ifndef NOISEGENERATOR_HPP
#define NOISEGENERATOR_HPP

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <vector>
#include <array>

#include <thread>

#include <cmath>


class NoiseGenerator
{
private:

    int width;
    int maxoctaves = 1;

    GLfloat interp(GLfloat val1, GLfloat val2, int posbetween, int lengthbetween)
    {
        //posbetween is the relative x value of the x position of the position at which i want to interp - the x value of val1
        GLfloat out = val1 - ((float(val1 - val2) / lengthbetween) * posbetween);
        //out -= std::pow(1, rand() % 2) * (std::abs(val1 - val2)/20) * std::max((GLfloat) rand() / (RAND_MAX), 0.5f);
        return out;
    }

    GLfloat sample(int x, int y, int oct)
    {

        GLfloat out = 0.f;
        int rate = width;

        rate = rate / std::pow(2, oct);

        int x0 = (x / rate) * rate;
        int x1 = (x0+rate) % width;

        int y0 = (y / rate) * rate;
        int y1 = (y0 + rate) % width;

        GLfloat interpx = interp(seedmap[x0][y0], seedmap[x1][y0], x % rate, rate);
        GLfloat interpy = interp(seedmap[x0][y1], seedmap[x1][y1], x % rate, rate);

        out = interp(interpx, interpy, y % rate, rate) / std::pow(2, oct);

        return out;
    }

    GLfloat avgheight(float x, float y)
    {
        return GLfloat(x+y) / 2;
    }

    GLfloat seed(int x, int y)
    {

        GLfloat out;
        //out = seedmap[((x+x) * y) % (width * range)]; //maybe be

        return out;
    }

public:
    NoiseGenerator() { srand(time(0)); }

    //create a vector of size width x width with only 0
    //std::vector<std::vector<GLfloat>> seedmap;
    std::vector<std::vector<GLfloat>> seedmap;
    std::vector<std::vector<GLfloat>> noisemap;

    int range = 255;
    int octaves = 7;
    float mountainfreq = 3.5;
    int rdmseed = 1;
    GLfloat smallest;

    float oceanfract = 2/7;

    std::vector<GLfloat> vertecies;
    std::vector<GLfloat> colours;

    void setwidth(int wdth)
    {
        int out;
        for(out = 2; out*2 <= wdth; out*=2){maxoctaves += 1;}
        width = out;
    }

    int getwidth()
    {
        return width;
    }

    void generateseed()
    {

        
        seedmap.resize(width, std::vector<GLfloat>(width));
        noisemap.resize(width, std::vector<GLfloat>(width));

        for(int i=0; i < width; ++i)
            for(int j=0; j < width; ++j)
            {   
                //this sets a seed everytime, maybe this is better for larger world as it takes a lot less memory, 
                //tho a hybrid method might just be the way to go
                srand(rdmseed* (i*width + j * rdmseed));
                seedmap[i][j] = (GLfloat)rand() / (RAND_MAX);
            }

    }

    float noisevalue(int x, int y)
    {
        //make sure that x and y are in bounds and are looping back on themselves
        x = (width*(x<0)) + (x % width);
        y = (width*(y<0)) + (y % width);

        GLfloat out = 0.0f;
        int i;

        //aplies one octave after the other ... could technically be in paralel
        for(i = 0; i <= std::min(maxoctaves, octaves); ++i)
        {
            out += sample(x,y, i); //position(x,y), current octave
            //threads[i] = std::thread(sample,this, x, y, i, &sampled[i]);
        }

        return out;
    }

    GLfloat noisedata(int x, int y)
    {
        //make sure that x and y are in bounds and are looping back on themselvesww
        x %= width;
        x += width*(x<0);

        y %= width;
        y += width*(y<0);

        return (noisemap[x][y] - smallest) * range;
    }

    void aplynoisemap()
    {
        noisemap.resize(width, std::vector<GLfloat>(width));

        smallest = noisevalue(0, 0);

        for(int i = 0; i < width; ++i)
            for(int j = 0; j < width; ++j)
            {
                noisemap[i][j] = noisevalue(i, j);
                smallest = smallest * (noisemap[i][j] >= smallest) + noisemap[i][j] * ((noisemap[i][j] < smallest));
            }
    }
    
    void positionbasedmesh(int posx, int posy, int maxdist)
    {
        //rate is set to the resolution the lowest sampler
        int rate = width;
        rate = rate / std::pow(2, std::min(maxoctaves, octaves));

        //maxdist must be a multiple of rate
        maxdist -= maxdist % rate;
        maxdist = std::max(rate, maxdist);

        //vsize is equal to the amount of data that is placed in the vector
        unsigned int vsize = (maxdist*maxdist)/rate *2*9;

        if(vertecies.size() != vsize)
        {
            vertecies.clear();
            colours.clear();

            vertecies.resize(vsize);
            colours.resize(vsize);
        }

        std::array<GLfloat, 3> nValues = {0, 0, 0};

        /*
        possible combinations: 8
            - (-1,-1) / (-1, 0) / center
            - (-1, 0) / (-1, 1) / center
            - (-1, 1) / ( 0, 1) / center
            - ( 0, 1) / ( 1, 1) / center
            - ( 1, 1) / ( 1, 0) / center
            - ( 1, 0) / ( 1,-1) / center
            - ( 1,-1) / ( 0,-1) / center
            - ( 0,-1) / (-1,-1) / center
        */
        std::array<std::array<int, 2>, 8> pointoffsets
        {
            {{-1,-1}, {-1, 0},
             {-1, 1}, { 0, 1},
             { 1, 1}, { 1, 0},
             { 1,-1}, { 0,-1}}
        };

        int newx1;
        int newx2;
        int newy1;
        int newy2;

        posx -= maxdist/2;
        posy -= maxdist/2;

        posx -= posx % rate;
        posy -= posy % rate;

        int index = 0;

        for(int x = posx; x < posx + maxdist-1; x += rate)
            for(int y = posy; y < posy + maxdist-1; y += rate)
            {
                nValues[0] = noisedata(x, y);

                for(int i = 0; i < 2; ++i)
                {
                    
                    //set new point
                    newx1 = x + (pointoffsets[i*4][0] * rate);
                    newy1 = y + (pointoffsets[i*4][1] * rate);
                    newx2 = x + (pointoffsets[(i*4+1) % 8][0] * rate);
                    newy2 = y + (pointoffsets[(i*4+1) % 8][1] * rate);

                    nValues[1] = noisedata(newx1,newy1);
                    nValues[2] = noisedata(newx2,newy2);

                    vertecies[index + 0] = newx1;
                    vertecies[index + 1] = nValues[1];
                    vertecies[index + 2] = newy1;

                    vertecies[index + 3] = newx2;
                    vertecies[index + 4] = nValues[2];
                    vertecies[index + 5] = newy2;

                    vertecies[index + 6] = x;
                    vertecies[index + 7] = nValues[0];
                    vertecies[index + 8] = y;


                    GLfloat avg = std::abs(nValues[0] + nValues[1] + nValues[2]) /3;

                    //assign colours
                    GLfloat c1 = 0.4 * (avg < range * 2/7);
                    GLfloat c2 = avg / range * (avg >= range * 2/7) ;
                    GLfloat c3 = 0.1f;

                    colours[index + 0] = c3;
                    colours[index + 1] = c2;
                    colours[index + 2] = c1;

                    colours[index + 3] = c3;
                    colours[index + 4] = c2;
                    colours[index + 5] = c1;

                    colours[index + 6] = c3;
                    colours[index + 7] = c2;
                    colours[index + 8] = c1;

                    index += 9;
                }
            }
       //std::cout << "there are now " << index << " elements in the vector with " << vsize << " expected." << std::endl;
    }
};

#endif

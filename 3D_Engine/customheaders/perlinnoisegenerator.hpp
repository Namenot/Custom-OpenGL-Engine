#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <vector>
#include <array>

#include <cmath>


class Perlinnoise
{
private:

    float interp(int val1, int val2, int posbetween, int range)
    {
        //posbetween is the relative x value of the x position of the position at which i want to interp - the x value of val1
        float out = val1 - ((float(val1 - val2) / range) * posbetween);
        return out;
    }

    float sample(int x, int y, int oct)
    {
        float out = 0.0f;
        float normalizer = 0.f;
        int rate = width;

        //for(int i = 0; i < oct; ++i)
            //rate = rate/2;

        rate = rate / std::pow(2, oct);

        int x0 = (x / rate) * rate;
        int x1 = (x0+rate) % width;

        int y0 = (y / rate) * rate;
        int y1 = (y0 + rate) % width;

        float interpx = interp(seed[x0][y0], seed[x1][y0], x % rate, rate);
        float interpy = interp(seed[x0][y1], seed[x1][y1], x % rate, rate);

        out = interp(interpx, interpy, y % rate, rate) / std::pow(2, oct);

        //std::cout << "out : " << out << std::endl;
        //std::cout << "norm: " << normalizer << std::endl;

        return out;
    }

    GLfloat avgheight(float x, float y)
    {
        return GLfloat(x+y) / 2;
    }

    void ajustwidth()
    {
        int out;
        for(out = 2; out*2 <= width; out*=2);
        width = out;
    }

public:

    //create a vector of size width x width with only 0
    std::vector<std::vector<GLfloat>> seed;
    std::vector<std::vector<GLfloat>> perlinmap;

    int range = 255;
    int width = 16;
    int octaves = 7;
    GLfloat smallest;

    float oceanfract = 2/7;

    std::vector<GLfloat> vertecies;
    std::vector<GLfloat> colours;

    void generateseed()
    {

        ajustwidth();

        srand(time(0));
        seed.resize(width, std::vector<GLfloat>(width));
        perlinmap.resize(width, std::vector<GLfloat>(width));

        for(int i=0; i < width; ++i)
            for(int j=0; j < width; ++j)
            {
                seed[i][j] = rand() % range;// + (1/(rand() % range));
                //seed[i][j] += range * (i == 0|| j == 0 || j == width-1|| i == width-1);
                //seed[i][j] -=  (seed[i][j]/1) * (i == 0|| j == 0 || j == width-1|| i == width-1);
            }

        seed[width/2][width/2] == rand() % (range/2);
    }

    float perlinvalue(int x, int y)
    {
        //make sure that x and y are in bounds and are looping back on themselves
        x = (width*(x<0)) + (x % width);
        y = (width*(y<0)) + (y % width);

        float out = 0.0f;

        for(int i = 0; i < octaves; ++i)
        {
            out += sample(x,y, i); //position(x,y), current octave
        }

        return out;
    }

    float perlindata(int x, int y)
    {
        //make sure that x and y are in bounds and are looping back on themselves
        x = (width*(x<0)) + (x % width);
        y = (width*(y<0)) + (y % width);

        return perlinmap[x][y] - smallest;

    }

    void aplyperlinmap()
    {
        smallest = perlinvalue(0, 0);

        for(int i = 0; i < width; ++i)
            for(int j = 0; j < width; ++j)
            {
                perlinmap[i][j] = perlinvalue(i, j);
                smallest = smallest * (perlinmap[i][j] >= smallest) + perlinmap[i][j] * ((perlinmap[i][j] < smallest));
            }
    }

    void generatemesh()
    {

        srand(time(0));
        float center;
        float e1;
        float e2;
        float e3;
        float e4;
        std::vector<GLfloat> corners;
        corners.resize(4);

        vertecies.push_back(0.f);
        vertecies.push_back(GLfloat(range) * (oceanfract));
        vertecies.push_back(0.f);

        vertecies.push_back(0.f);
        vertecies.push_back(GLfloat(range) * (oceanfract));
        vertecies.push_back(GLfloat(width));

        vertecies.push_back(GLfloat(width));
        vertecies.push_back(GLfloat(range) * (oceanfract));
        vertecies.push_back(0.f);

        vertecies.push_back(GLfloat(width));
        vertecies.push_back(GLfloat(range) * (oceanfract));
        vertecies.push_back(GLfloat(0.f));

        vertecies.push_back(0.f);
        vertecies.push_back(GLfloat(range) * (oceanfract));
        vertecies.push_back(GLfloat(width));

        vertecies.push_back(GLfloat(width));
        vertecies.push_back(GLfloat(range) * (oceanfract));
        vertecies.push_back(GLfloat(width));

        colours.push_back(0.1f);
        colours.push_back(0.f);
        colours.push_back(0.3f);
        colours.push_back(0.1f);
        colours.push_back(0.f);
        colours.push_back(0.4f);
        colours.push_back(0.1f);
        colours.push_back(0.f);
        colours.push_back(0.5f);

        colours.push_back(0.1f);
        colours.push_back(0.f);
        colours.push_back(0.5f);
        colours.push_back(0.1f);
        colours.push_back(0.f);
        colours.push_back(0.4f);
        colours.push_back(0.1f);
        colours.push_back(0.f);
        colours.push_back(0.3f);




        //here we can add a lot of optimisation by keeping all corners in memory and just reusing them
        for(int i = 0; i < width; ++i)
            for(int j = 0; j < width; ++j)
            {
                center = perlindata(i,j);
                e1 = avgheight(avgheight(center, perlindata(i-1,j)), avgheight(perlindata(i-1,j-1), perlindata(i,j-1))); //upper-left corner
                e2 = avgheight(avgheight(center, perlindata(i+1,j)), avgheight(perlindata(i+1,j-1), perlindata(i,j-1))); //upper-right corner
                e3 = avgheight(avgheight(center, perlindata(i+1,j)), avgheight(perlindata(i+1,j+1), perlindata(i,j+1))); //lower-right corner
                e4 = avgheight(avgheight(center, perlindata(i-1,j)), avgheight(perlindata(i-1,j+1), perlindata(i,j+1))); //lower-left corner

                //triangle 1:
                vertecies.push_back(i+1);
                vertecies.push_back(e2);
                vertecies.push_back(j);

                vertecies.push_back(i);
                vertecies.push_back(e1);
                vertecies.push_back(j);

                vertecies.push_back(i+0.5);
                vertecies.push_back(center);
                vertecies.push_back(j+0.5);

                //triangle 2:
                vertecies.push_back(i+1);
                vertecies.push_back(e3);
                vertecies.push_back(j+1);

                vertecies.push_back(i+1);
                vertecies.push_back(e2);
                vertecies.push_back(j);

                vertecies.push_back(i+0.5);
                vertecies.push_back(center);
                vertecies.push_back(j+0.5);

                //triangle 3:
                vertecies.push_back(i);
                vertecies.push_back(e4);
                vertecies.push_back(j+1);

                vertecies.push_back(i+1);
                vertecies.push_back(e3);
                vertecies.push_back(j+1);

                vertecies.push_back(i+0.5);
                vertecies.push_back(center);
                vertecies.push_back(j+0.5);

                //triangle 4:
                vertecies.push_back(i);
                vertecies.push_back(e1);
                vertecies.push_back(j);

                vertecies.push_back(i);
                vertecies.push_back(e4);
                vertecies.push_back(j+1);

                vertecies.push_back(i+0.5);
                vertecies.push_back(center);
                vertecies.push_back(j+0.5);

                //assign colours
                GLfloat c1 = 1.0f / ((rand() % 5)+0.1);
                GLfloat c2;
                c1 = 0.4 * (e1 < range * oceanfract);
                c2 = e1 / range * (e1 >= range * oceanfract) ;
                GLfloat c3 = 0.1f;

                //colour 1

                c1 = 0.4 * (avgheight(avgheight(e2,e1), center) < range * oceanfract);
                c2 = avgheight(avgheight(e2,e1), center) / range * (avgheight(avgheight(e2,e1), center) >= range * oceanfract);

                colours.push_back(c3);
                colours.push_back(c2);
                colours.push_back(c1);

                colours.push_back(c3);
                colours.push_back(c2);
                colours.push_back(c1);

                colours.push_back(c3);
                colours.push_back(c2);
                colours.push_back(c1);

                //colour 2

                c1 = 0.4 * (avgheight(avgheight(e2,e3), center) < range * oceanfract);
                c2 = avgheight(avgheight(e2,e3), center) / range * (avgheight(avgheight(e2,e3), center) >= range * oceanfract);

                colours.push_back(c3);
                colours.push_back(c2);
                colours.push_back(c1);
                colours.push_back(c3);
                colours.push_back(c2);
                colours.push_back(c1);
                colours.push_back(c3);
                colours.push_back(c2);
                colours.push_back(c1);

                //colour 3

                c1 = 0.4 * (avgheight(avgheight(e4,e3), center) < range * oceanfract);
                c2 = avgheight(avgheight(e4,e3), center) / range * (avgheight(avgheight(e4,e3), center) >= range * oceanfract);

                colours.push_back(c3);
                colours.push_back(c2);
                colours.push_back(c1);

                colours.push_back(c3);
                colours.push_back(c2);
                colours.push_back(c1);

                colours.push_back(c3);
                colours.push_back(c2);
                colours.push_back(c1);

                //colour 4

                c1 = 0.4 * (avgheight(avgheight(e4,e1), center) < range * oceanfract);
                c2 = avgheight(avgheight(e4,e1), center) / range * (avgheight(avgheight(e4,e1), center) >= range * oceanfract);

                colours.push_back(c3);
                colours.push_back(c2);
                colours.push_back(c1);

                colours.push_back(c3);
                colours.push_back(c2);
                colours.push_back(c1);

                colours.push_back(c3);
                colours.push_back(c2);
                colours.push_back(c1);

            }
    }

    void generateweirdmesh()
    {
        srand(time(0));
        float center;

        vertecies.push_back(0.f);
        vertecies.push_back(GLfloat(range) * (oceanfract));
        vertecies.push_back(0.f);

        vertecies.push_back(0.f);
        vertecies.push_back(GLfloat(range) * (oceanfract));
        vertecies.push_back(GLfloat(width));

        vertecies.push_back(GLfloat(width));
        vertecies.push_back(GLfloat(range) * (oceanfract));
        vertecies.push_back(0.f);

        vertecies.push_back(GLfloat(width));
        vertecies.push_back(GLfloat(range) * (oceanfract));
        vertecies.push_back(GLfloat(0.f));

        vertecies.push_back(0.f);
        vertecies.push_back(GLfloat(range) * (oceanfract));
        vertecies.push_back(GLfloat(width));

        vertecies.push_back(GLfloat(width));
        vertecies.push_back(GLfloat(range) * (oceanfract));
        vertecies.push_back(GLfloat(width));

        colours.push_back(0.1f);
        colours.push_back(0.f);
        colours.push_back(0.3f);
        colours.push_back(0.1f);
        colours.push_back(0.f);
        colours.push_back(0.4f);
        colours.push_back(0.1f);
        colours.push_back(0.f);
        colours.push_back(0.5f);

        colours.push_back(0.1f);
        colours.push_back(0.f);
        colours.push_back(0.5f);
        colours.push_back(0.1f);
        colours.push_back(0.f);
        colours.push_back(0.4f);
        colours.push_back(0.1f);
        colours.push_back(0.f);
        colours.push_back(0.3f);

        for(int i = 0; i < width; ++i)
            for(int j = 0; j <width; ++j)
            {

                center = perlindata(i,j);

                vertecies.push_back(i+1);
                vertecies.push_back(center);
                vertecies.push_back(j);

                vertecies.push_back(i);
                vertecies.push_back(center);
                vertecies.push_back(j);

                vertecies.push_back(i);
                vertecies.push_back(center);
                vertecies.push_back(j+1);

                vertecies.push_back(i+1);
                vertecies.push_back(center);
                vertecies.push_back(j+1);

                vertecies.push_back(i+1);
                vertecies.push_back(center);
                vertecies.push_back(j);

                vertecies.push_back(i);
                vertecies.push_back(center);
                vertecies.push_back(j+1);

                GLfloat c1 = 1.0f / ((rand() % 5)+0.1);
                c1 = 0.4 * (center < range * 2/7);
                GLfloat c2 = center / range * (center >= range * 2/7) ;
                GLfloat c3 = 0.1f;

                colours.push_back(c3);
                colours.push_back(c2);
                colours.push_back(c1);

                colours.push_back(c3);
                colours.push_back(c2);
                colours.push_back(c1);

                colours.push_back(c3);
                colours.push_back(c2);
                colours.push_back(c1);

                colours.push_back(c3);
                colours.push_back(c2);
                colours.push_back(c1);

                colours.push_back(c3);
                colours.push_back(c2);
                colours.push_back(c1);

                colours.push_back(c3);
                colours.push_back(c2);
                colours.push_back(c1);
            }

    }

};

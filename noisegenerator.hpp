#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <vector>
#include <array>

#include <cmath>


class NoiseGenerator
{
private:

    GLfloat interp(GLfloat val1, GLfloat val2, int posbetween, int lengthbetween)
    {
        //posbetween is the relative x value of the x position of the position at which i want to interp - the x value of val1
        GLfloat out = val1 - ((float(val1 - val2) / lengthbetween) * posbetween);
        //out -= std::pow(1, rand() % 2) * (std::abs(val1 - val2)/20) * std::max((GLfloat) rand() / (RAND_MAX), 0.5f);
        return out;
    }

    float sample(int x, int y, int oct)
    {
        float out = 0.0f;
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
    std::vector<std::vector<GLfloat>> noisemap;

    int range = 255;
    int width = 16;
    int octaves = 7;
    int mountainfreq = 3.5;
    GLfloat smallest;

    float oceanfract = 2/7;

    std::vector<GLfloat> vertecies;
    std::vector<GLfloat> colours;

    void generateseed()
    {

        ajustwidth();

        srand(time(0));
        seed.resize(width, std::vector<GLfloat>(width));
        noisemap.resize(width, std::vector<GLfloat>(width));

        for(int i=0; i < width; ++i)
            for(int j=0; j < width; ++j)
            {
                seed[i][j] = (GLfloat) rand() / (RAND_MAX);// * range;// + (1/(rand() % range));
                //seed[i][j] += range * (i == 0|| j == 0 || j == width-1|| i == width-1);
                //seed[i][j] -=  (seed[i][j]/1) * (i == 0|| j == 0 || j == width-1|| i == width-1);

                //test with just a few highpoints
                //seed[i][j] = rand() % (range);
                //seed[i][j] += ((rand() % (range)) > ((range*8)/9)) * (range - seed[i][j]);
            }

        seed[width/2][width/2] = 0.5f;//std::max(rand() % (range/2), range/4);
    }

    float noisevalue(int x, int y)
    {
        //make sure that x and y are in bounds and are looping back on themselves
        x = (width*(x<0)) + (x % width);
        y = (width*(y<0)) + (y % width);

        float out = 0.0f;

        //octaves can be exceeded which leeds the program to crash
        for(int i = 0; i <= octaves; ++i)
        {
            out += sample(x,y, i); //position(x,y), current octave
        }

        return out;
    }

    GLfloat noisedata(int x, int y)
    {
        //make sure that x and y are in bounds and are looping back on themselves
        x = (width*(x<0)) + (x % width);
        y = (width*(y<0)) + (y % width);

        return (noisemap[x][y] - smallest) * range;
    }

    void aplynoisemap()
    {
        smallest = noisevalue(0, 0);

        for(int i = 0; i < width; ++i)
            for(int j = 0; j < width; ++j)
            {
                noisemap[i][j] = noisevalue(i, j);
                smallest = smallest * (noisemap[i][j] >= smallest) + noisemap[i][j] * ((noisemap[i][j] < smallest));
            }
    }

    void generateweirdmesh()
    {
        srand(time(0));
        float center;

        /*vertecies.push_back(0.f);
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
        colours.push_back(0.3f);*/

        for(int i = 0; i < width; ++i)
            for(int j = 0; j <width; ++j)
            {

                center = noisedata(i,j);

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


    void mesh()
    {
        srand(time(0));

        std::array<std::array<int, 2>, 8> pointoffsets
        {
            /*possible combinations: 8
                - (-1,-1) / (-1, 0) / center
                - (-1, 0) / (-1, 1) / center
                - (-1, 1) / ( 0, 1) / center
                - ( 0, 1) / ( 1, 1) / center
                - ( 1, 1) / ( 1, 0) / center
                - ( 1, 0) / ( 1,-1) / center
                - ( 1,-1) / ( 0,-1) / center
                - ( 0,-1) / (-1,-1) / center
            */
            {{-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}}
        };

        int newx1;
        int newx2;
        int newy1;
        int newy2;

        for(int x = 0; x < width; x += 2)
            for(int y = 0; y < width; y += 2)
                for(int i = 0; i < 8; ++i)
                {
                    //set new points
                    newx1 = x + pointoffsets[i][0];
                    newy1 = y + pointoffsets[i][1];
                    newx2 = x + pointoffsets[(i+1) % 8][0];
                    newy2 = y + pointoffsets[(i+1) % 8][1];

                    //check if new points are valid
                    if(newx1 < 0){continue;}
                    if(newy1 < 0){continue;}
                    if(newx2 < 0){continue;}
                    if(newy2 < 0){continue;}

                    vertecies.push_back(newx1);
                    vertecies.push_back(noisedata(newx1,newy1));
                    vertecies.push_back(newy1);

                    vertecies.push_back(newx2);
                    vertecies.push_back(noisedata(newx2,newy2));
                    vertecies.push_back(newy2);

                    vertecies.push_back(x);
                    vertecies.push_back(noisedata(x,y));
                    vertecies.push_back(y);

                    //assign colours
                    GLfloat c1 = 1.0f / ((rand() % 7)+0.1); //Red
                    GLfloat c2 = 1.0f / ((rand() % 3)+0.1); //Green
                    GLfloat c3 = 0.1f;                      //Blue

                    //colour 1
                    c1 = 0.4 * (avgheight(avgheight(noisedata(newx1,newy1),noisedata(newx2,newy2)), noisedata(x,y)) < range * oceanfract);
                    c2 = avgheight(avgheight(noisedata(newx1,newy1),noisedata(newx2,newy2)), noisedata(x,y)) / range * (avgheight(avgheight(noisedata(newx1,newy1),noisedata(newx2,newy2)), noisedata(x,y)) >= range * oceanfract);

                    float avg;// = avgheight(avgheight(noisedata(x,y), noisedata(newx2,newy2)), noisedata(newx1,newy1));
                    avg = std::max(std::max(noisedata(x,y), noisedata(newx2,newy2)), noisedata(newx1,newy1));
                    float mStart = mountainfreq-0.2f;// - ((rand() % 100) / 100);

                    c2 += (1 - (c2*2)) * (avg > range / mStart);
                    c2 -= 0.6*c2*(avg > range / mStart);

                    c1 -= (c1-std::abs(c2-c1))*(avg > range / mStart);
                    c3 -= (c3-std::abs(c2-c3))*(avg > range / mStart);

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

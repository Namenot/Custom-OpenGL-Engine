#include "mesh.hpp"

// terminal is a set variables that can changed from anywhere in the code

class TERMINAL
{
public:
  // testing variable
    int test = 0;

//3D Meshes:
    //Polar Coords based array
    std::vector<polar3d> PolarMesh;
    //Dictionary of each Triangle
    std::map<unsigned int, map3d> PolarMeshLookUp;



//2D Stuff

// window settings / graphics settings
    int windoowheight = 1080;
    int windowwidth = 1920;

    int fullscreen = 1; // 0: windowed, 1: fullscreen, 2: fullscreen window

    int maxfps = 144;
    int fov = 90; // the cameras field of view
};

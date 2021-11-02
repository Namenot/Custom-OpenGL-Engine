// include standart headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// Include GLEW, GLFW & GLM
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader.hpp>
#include "camcontroller.hpp"

GLFWwindow* window;
using namespace glm;

class BaseWindow
{
public:

    vector<GLfloat> vertecies = {
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
             1.0f, 1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
             1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
             1.0f,-1.0f,-1.0f,
             1.0f, 1.0f,-1.0f,
             1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
             1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
             1.0f,-1.0f, 1.0f,
             1.0f, 1.0f, 1.0f,
             1.0f,-1.0f,-1.0f,
             1.0f, 1.0f,-1.0f,
             1.0f,-1.0f,-1.0f,
             1.0f, 1.0f, 1.0f,
             1.0f,-1.0f, 1.0f,
             1.0f, 1.0f, 1.0f,
             1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
             1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
             1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
             1.0f,-1.0f, 1.0f,

            -2.0f,-2.0f,-2.0f,
            -2.0f,-2.0f, 2.0f,
            -2.0f, 2.0f, 2.0f,
             2.0f, 2.0f,-2.0f,
            -2.0f,-2.0f,-2.0f,
            -2.0f, 2.0f,-2.0f,
             2.0f,-2.0f, 2.0f,
            -2.0f,-2.0f,-2.0f,
             2.0f,-2.0f,-2.0f,
             2.0f, 2.0f,-2.0f,
             2.0f,-2.0f,-2.0f,
            -2.0f,-2.0f,-2.0f,
            -2.0f,-2.0f,-2.0f,
            -2.0f, 2.0f, 2.0f,
            -2.0f, 2.0f,-2.0f,
             2.0f,-2.0f, 2.0f,
            -2.0f,-2.0f, 2.0f,
            -2.0f,-2.0f,-2.0f,
            -2.0f, 2.0f, 2.0f,
            -2.0f,-2.0f, 2.0f,
             2.0f,-2.0f, 2.0f,
             2.0f, 2.0f, 2.0f,
             2.0f,-2.0f,-2.0f,
             2.0f, 2.0f,-2.0f,
             2.0f,-2.0f,-2.0f,
             2.0f, 2.0f, 2.0f,
             2.0f,-2.0f, 2.0f,
             2.0f, 2.0f, 2.0f,
             2.0f, 2.0f,-2.0f,
            -2.0f, 2.0f,-2.0f,
             2.0f, 2.0f, 2.0f,
            -2.0f, 2.0f,-2.0f,
            -2.0f, 2.0f, 2.0f,
             2.0f, 2.0f, 2.0f,
            -2.0f, 2.0f, 2.0f,
             2.0f,-2.0f, 2.0f
        };
    vector<GLfloat> colors = {};/*
            0.9f,  0.0f,  0.0f,
            0.9f,  0.0f,  0.0f,
            0.9f,  0.0f,  0.0f,
            0.9f,  0.0f,  0.0f,
            0.9f,  0.0f,  0.0f,
            0.9f,  0.0f,  0.0f,

            0.0f,  0.9f,  0.0f,
            0.0f,  0.9f,  0.0f,
            0.0f,  0.9f,  0.0f,
            0.0f,  0.9f,  0.0f,
            0.0f,  0.9f,  0.0f,
            0.0f,  0.9f,  0.0f,

            0.0f,  0.0f,  0.9f,
            0.0f,  0.0f,  0.9f,
            0.0f,  0.0f,  0.9f,
            0.0f,  0.0f,  0.9f,
            0.0f,  0.0f,  0.9f,
            0.0f,  0.0f,  0.9f,

            0.9f,  0.0f,  0.9f,
            0.9f,  0.0f,  0.9f,
            0.9f,  0.0f,  0.9f,
            0.9f,  0.0f,  0.9f,
            0.9f,  0.0f,  0.9f,
            0.9f,  0.0f,  0.9f,

            0.9f,  0.9f,  0.0f,
            0.9f,  0.9f,  0.0f,
            0.9f,  0.9f,  0.0f,
            0.9f,  0.9f,  0.0f,
            0.9f,  0.9f,  0.0f,
            0.9f,  0.9f,  0.0f,

            0.0f,  0.9f,  0.9f,
            0.0f,  0.9f,  0.9f,
            0.0f,  0.9f,  0.9f,
            0.0f,  0.9f,  0.9f,
            0.0f,  0.9f,  0.9f,
            0.0f,  0.9f,  0.9f,

            0.9f,  0.0f,  0.0f,
            0.9f,  0.0f,  0.0f,
            0.9f,  0.0f,  0.0f,
            0.9f,  0.0f,  0.0f,
            0.9f,  0.0f,  0.0f,
            0.9f,  0.0f,  0.0f,

            0.0f,  0.9f,  0.0f,
            0.0f,  0.9f,  0.0f,
            0.0f,  0.9f,  0.0f,
            0.0f,  0.9f,  0.0f,
            0.0f,  0.9f,  0.0f,
            0.0f,  0.9f,  0.0f,

            0.0f,  0.0f,  0.9f,
            0.0f,  0.0f,  0.9f,
            0.0f,  0.0f,  0.9f,
            0.0f,  0.0f,  0.9f,
            0.0f,  0.0f,  0.9f,
            0.0f,  0.0f,  0.9f,

            0.9f,  0.0f,  0.9f,
            0.9f,  0.0f,  0.9f,
            0.9f,  0.0f,  0.9f,
            0.9f,  0.0f,  0.9f,
            0.9f,  0.0f,  0.9f,
            0.9f,  0.0f,  0.9f,

            0.9f,  0.9f,  0.0f,
            0.9f,  0.9f,  0.0f,
            0.9f,  0.9f,  0.0f,
            0.9f,  0.9f,  0.0f,
            0.9f,  0.9f,  0.0f,
            0.9f,  0.9f,  0.0f,

            0.0f,  0.9f,  0.9f,
            0.0f,  0.9f,  0.9f,
            0.0f,  0.9f,  0.9f,
            0.0f,  0.9f,  0.9f,
            0.0f,  0.9f,  0.9f,
            0.0f,  0.9f,  0.9f,
        };*/

    std::array<GLfloat, 3> spawn = {0,0,0};

    int initWindow()
    {
        if(!glfwInit())
        {
            fprintf( stderr, "Failed to initialize GLFW\n" );
            return -1;
        }

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow( 1600, 700, "Test-Cube", NULL, NULL);
        if( window == NULL )
        {
            fprintf( stderr, "Failed to open GLFW window\n" );
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(window);

        // Initialize GLEW
        glewExperimental = true; // Needed for core profile
        if (glewInit() != GLEW_OK)
        {
            fprintf(stderr, "Failed to initialize GLEW\n");
            glfwTerminate();
            return -1;
        }

        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
        // Hide the mouse and enable unlimited mouvement
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Set the mouse at the center of the screen
        glfwPollEvents();
        glfwSetCursorPos(window, 1600/2, 700/2);

        glClearColor(0.2f, 0.7f, 0.9f, 0.f);// r, g ,b, alpha

        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);
        // Cull triangles which normal is not towards the camera
        glEnable(GL_CULL_FACE);

        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        // Create and compile our GLSL program from the shaders
        GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");

        // Get a handle for our "MVP" uniform
        GLuint MatrixID = glGetUniformLocation(programID, "MVP");

        // Projection matrix : 90 Field of View, 16:9 ratio, display range : 0.1 unit <-> 100 units(renderdistance)
        glm::mat4 Projection = glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100000.0f);

        // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
        // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
        static const GLfloat *g_vertex_buffer_data = &vertecies[0];
        size_t vertexsize = vertecies.size()* sizeof(GLfloat);

        std::cout << "vertexsize: " << vertexsize /4 << std::endl;

        // One color for each vertex. They were generated randomly.
        static const GLfloat *g_color_buffer_data = &colors[0];
        size_t colorsize = colors.size()*sizeof(GLfloat);

        std::cout << "colorsize: " << colorsize << std::endl;

        std::cout << "size of colourbuffer and vertexbuffer combined: " << ((vertexsize/4 + colorsize/4)* sizeof(GLfloat))/(1024*1024) << " mega-bytes" << std::endl;
        std::cout << "sizeof colors + vertex: " << sizeof(colors) + sizeof(vertecies) << std::endl;

        GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, vertexsize, g_vertex_buffer_data, GL_STREAM_DRAW); // this line is the key.. we can just rebind the buffer

        GLuint colorbuffer;
        glGenBuffers(1, &colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, colorsize, g_color_buffer_data, GL_STREAM_DRAW);


        //fps vars
        double lastTime = glfwGetTime();
        int nbFrames = 0;

        int framerate = 144; // set fps
        float frametime = float(1000) / framerate;
        glfwSwapInterval(0); // 1 sets fps to display settings (in my case to 144) whilst 0 unlocks fps

        do{

            //very stupid test

            vertecies[0] += 1;

            //fps counter
            double currentTime = glfwGetTime();
            nbFrames++;
            if ( currentTime - lastTime >= 1.0 ){
                    // printf and reset timer
                    //std::cout << double(nbFrames) << "frames per second" << std::endl;
                    //std::cout << "average frametime: "<< 1000/double(nbFrames) << "ms" << std::endl;
                    //std::cout << " wished frametime: " << frametime << "ms" << std::endl;
                    //std::cout << "frametime varried by " << 1000/double(nbFrames) - frametime << "ms" << std::endl << std::endl;
                    nbFrames = 0;
                    lastTime += 1.0;
            }

            sleepcp(frametime); //sleep some milliseconds (works waaay better on linux)

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Use our shader
            glUseProgram(programID);

            // Compute the MVP matrix from keyboard and mouse input
            //setSpawnPoint(spawn[0], spawn[1], spawn[2]);
            computeMatricesFromInputs();
            glm::mat4 ProjectionMatrix = getProjectionMatrix();
            glm::mat4 ViewMatrix = getViewMatrix();
            glm::mat4 ModelMatrix = glm::mat4(1.0);
            glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            // Send our transformation to the currently bound shader,
            // in the "MVP" uniform
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

            // 1rst attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glVertexAttribPointer(
                    0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_TRUE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
            );

            // 2nd attribute buffer : colors
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
            glVertexAttribPointer(
                    1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                    3,                                // size
                    GL_FLOAT,                         // type
                    GL_TRUE,                         // normalized?
                    0,                                // stride
                    (void*)0                          // array buffer offset
            );

            // Draw the triangle !
            glDrawArrays(GL_TRIANGLES, 0, vertexsize/(4*3)); // 12*3 indices starting at 0 -> 12 triangles

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);

            // Swap buffers
            glfwSwapBuffers(window);
            glfwPollEvents();

        } // Check if the ESC key was pressed or the window was closed
        while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
                glfwWindowShouldClose(window) == 0 );

        // Cleanup VBO and shader
        glDeleteBuffers(1, &vertexbuffer);
        glDeleteBuffers(1, &colorbuffer);
        glDeleteProgram(programID);
        glDeleteVertexArrays(1, &VertexArrayID);

        // Close OpenGL window and terminate GLFW
        glfwTerminate();


        return 0;
    }

    void drawMesh()
    {
        glClearColor(0.2f, 0.7f, 0.9f, 1.0f); //r,g,b,a

        //static const GLfloat g_vertex_buffer_data[] = ;


    }

};

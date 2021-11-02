// include standart headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// Include GLEW, GLFW & GLM
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "common/shader.hpp"
#include "camcontroller.hpp"

using namespace glm;

class RenderTarget
{
private:
    GLFWwindow* window;

public:
    CamController CamCon;

    std::vector<GLfloat> vertecies = {};
    std::vector<GLfloat> colors = {};

    GLuint programID;
    GLuint VertexArrayID;
    GLuint MatrixID;

    glm::mat4 Projection;

    GLuint vertexbuffer;
    GLuint colorbuffer;

    size_t vertexsize;
    size_t colorsize;

    bool terminate = false;
    bool changed = false;

    int INIT(int width, int height, bool fullscreenmode)
    {
        if(!glfwInit())
        {
            fprintf( stderr, "Failed to initialize GLFW\n" );
            return -1;
        }

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWmonitor* monitor = NULL;
        if(fullscreenmode == true)
            monitor = glfwGetPrimaryMonitor();

        //window = glfwCreateWindow( width, height, "Test-Cube", glfwGetPrimaryMonitor(), NULL); //Fullscreen variante
        window = glfwCreateWindow( width, height, "Test-Cube", monitor, NULL); //windowed variante

        if( window == NULL )
        {
            fprintf( stderr, "Failed to create GLFW window\n");
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
        glfwSetCursorPos(window, width/2, height/2);

        glClearColor(0.2f, 0.7f, 0.9f, 0.f);// r, g ,b, alpha

        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);
        // Cull triangles which normal is not towards the camera
        glEnable(GL_CULL_FACE);

        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        // Projection matrix : 90 Field of View, 16:9 ratio, display range : 0.1 unit <-> 100 units(renderdistance)
        Projection = glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100000.0f);
        //Projection = glm::ortho(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 100000.0f);

        glfwSwapInterval(1); // 1 sets fps to display settings (in my case to 144) whilst 0 unlocks fps

        CamCon.window = window;
        CamCon.width = width;
        CamCon.height = height;

        return 0;
    }

    void setShaders(const char * vertexshadername, const char * fragmentshadername)
    {
        // Create and compile our GLSL program from the shaders
        programID = LoadShaders(vertexshadername, fragmentshadername);

        // Get a handle for our "MVP" uniform
        MatrixID = glGetUniformLocation(programID, "MVP");
    }

    void bindBuffers(std::vector<GLfloat> *vert, std::vector<GLfloat> *col)
    {
        //swap vertex and color buffers
        vertecies.swap(*vert);
        colors.swap(*col);

        //calculate size of buffer
        vertexsize = vertecies.size()*sizeof(GLfloat);
        colorsize = colors.size()*sizeof(GLfloat);

        //bind new buffer
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, vertexsize, &vertecies[0], GL_STREAM_DRAW);

        //bind new buffer
        glGenBuffers(1, &colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, colorsize, &colors[0], GL_STREAM_DRAW);
    }

    void changeBufferData(std::vector<GLfloat> *vert, std::vector<GLfloat> *col)
    {

        vertecies.swap(*vert);
        colors.swap(*col);

        //calculate size of buffer
        vertexsize = vertecies.size()*sizeof(GLfloat);
        colorsize = colors.size()*sizeof(GLfloat);

        std::cout << "vertexsize: " << vertexsize << std::endl;
        std::cout << "triangles: " << vertecies.size() / 3  << std::endl;

        changed = true;
    }

    void updateBuffers()
    {
        //bind new buffer
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, vertexsize, &vertecies[0], GL_STREAM_DRAW);
        //bind new buffer
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, colorsize, &colors[0], GL_STREAM_DRAW);
    }

    void cleanup()
    {
        glDeleteBuffers(1, &vertexbuffer);
        glDeleteBuffers(1, &colorbuffer);
        glDeleteProgram(programID);
        glDeleteVertexArrays(1, &VertexArrayID);
    }

    void Draw()
    {
        double lastTime = glfwGetTime();
        int nbFrames = 0;

        do{

            // Measure speed
            double currentTime = glfwGetTime();
            nbFrames++;
             if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
                 // printf and reset timer
                 printf("%f ms/frame\n", 1000.0/double(nbFrames));
                 nbFrames = 0;
                 lastTime += 1.0;
             }

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Use our shader
            glUseProgram(programID);

            // Compute the MVP matrix from keyboard and mouse input
            //setSpawnPoint(spawn[0], spawn[1], spawn[2]);
            CamCon.computeMatricesFromInputs();
            glm::mat4 ProjectionMatrix = CamCon.getProjectionMatrix();
            glm::mat4 ViewMatrix = CamCon.getViewMatrix();
            glm::mat4 ModelMatrix = glm::mat4(1.0);
            glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

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
            glDrawArrays(GL_TRIANGLES, 0, vertexsize/4); // 12*3 indices starting at 0 -> 12 triangles

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);

            // Swap buffers
            glfwSwapBuffers(window);
            glfwPollEvents();

            if(changed == true)
            {
                updateBuffers();
                changed = false;
            }

            /*change this to while(terminate == false)*/
        }while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
                    glfwWindowShouldClose(window) == 0 );

        cleanup();
        // Close OpenGL window and terminate GLFW
        glfwTerminate();
    }

};

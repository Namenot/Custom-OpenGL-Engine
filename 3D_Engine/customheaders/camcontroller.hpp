// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <iostream>
#include <algorithm>

class CamController
{
public:

    GLFWwindow* window;

    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;

    // Initial position : on +Z
    glm::vec3 position = glm::vec3( -0, 700, 0); // - 1796
    // Initial horizontal angle : toward -Z
    float horizontalAngle = 3.14f;
    // Initial vertical angle : none
    float verticalAngle = 0.0f;
    // Initial Field of View
    float initialFoV = 107.0f;

    int width = 1600;
    int height = 900;

    int renderdistance = 10000;

    float speed = 300.0f; // 3 units / second
    float mouseSpeed = 0.005f;

    glm::mat4 getViewMatrix()
    {
        return ViewMatrix;
    }
    glm::mat4 getProjectionMatrix()
    {
        return ProjectionMatrix;
    }

    glm::vec3 setSpawnPoint(GLfloat x, GLfloat y, GLfloat z)
    {
        return glm::vec3( x, y, z);
    }

    void computeMatricesFromInputs()
    {
        // glfwGetTime is called only once, the first time this function is called
        static double lastTime = glfwGetTime();

        // Compute time difference between current and last frame
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);

        // Get mouse position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Reset mouse position for next frame
        glfwSetCursorPos(window, width/2, height/2);

        // Compute new orientation
        horizontalAngle += mouseSpeed * float(width/2 - xpos );
        verticalAngle   += mouseSpeed * float( height/2 - ypos );

        verticalAngle = std::max(verticalAngle, -1.6f);
        verticalAngle = std::min(verticalAngle, 1.6f);

        // Direction : Spherical coordinates to Cartesian coordinates conversion
        glm::vec3 direction(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
        );

        // Right vector
        glm::vec3 right = glm::vec3(
            sin(horizontalAngle - 3.14f/2.0f),
            0,
            cos(horizontalAngle - 3.14f/2.0f)
        );

        // Up vector
        glm::vec3 up = glm::cross( right, direction );

        // Move forward
        if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
            position += direction * deltaTime * speed;
        }
        // Move backward
        if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
            position -= direction * deltaTime * speed;
        }
        // Strafe right
        if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
            position += right * deltaTime * speed;
        }
        // Strafe left
        if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
            position -= right * deltaTime * speed;
        }
        if (glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS){
            speed = 30;
        }else{
            speed = 300;
        }

        float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

        // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        ProjectionMatrix = glm::perspective(glm::radians(FoV), float(width) / float(height), 0.1f, float(renderdistance));
        // Camera matrix
        ViewMatrix       = glm::lookAt(
                                    position,           // Camera is here
                                    position+direction, // and looks here : at the same position, plus "direction"
                                    up                  // Head is up (set to 0,-1,0 to look upside-down)
                               );

        // For the next frame, the "last time" will be "now"
        lastTime = currentTime;
    }
};









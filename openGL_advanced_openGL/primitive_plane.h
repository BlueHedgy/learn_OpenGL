#ifndef PRIMITIVE_PLANE_H
#define PRIMITIVE_PLANE_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <vector>

const std::vector<float> VERTICES = {
    // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)

    // positions         // texCoords
     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
     5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};

class Plane
{
public:
    // default Plane attribute variables

    glm::vec3 Position;
    glm::vec3 ColorViewport;
    std::vector<float> Vertices;

    // Constructor
    Plane ( glm::vec3 position          = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 color_viewport    = glm::vec3(0.8f, 0.8f, 0.8f),
            std::vector<float> vertices = VERTICES)
    {
        Position        = position;
        ColorViewport   = color_viewport;
        Vertices        = vertices;
    }

};
#endif // ! PRIMITIVE_PLANE_H



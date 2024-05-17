#ifndef GL_VERTEX_H
#define GL_VERTEX_H
#include <global.h>

typedef struct gl_vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

#endif
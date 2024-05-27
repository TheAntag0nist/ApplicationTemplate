#ifndef VERTEX_H
#define VERTEX_H
#include <global.h>

typedef struct base_vertex {
    glm::vec3 pos;
    glm::vec3 norm;
    glm::vec2 tex_coords;
};

#endif
#version 460 core
layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 vert_norm;
layout (location = 2) in vec2 vert_tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 tex_coord;

void main()
{
    mat4 mvp = projection * view * model;
    gl_Position = mvp * vec4(vert_pos, 1.0f);
    tex_coord = vec2(vert_tex_coord.x, vert_tex_coord.y);
} 
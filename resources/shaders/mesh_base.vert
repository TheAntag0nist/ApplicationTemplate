#version 460 core
layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_tex_coord;

out vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat4 mvp = projection * view * model;
    gl_Position = mvp * vec4(vert_pos, 1.0f);
    tex_coords = vec2(vert_tex_coord.x, vert_tex_coord.y);
} 
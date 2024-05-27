#version 460 core
out vec4 FragColor;

in vec2 tex_coord;
uniform sampler2D albedo_tex;

void main() {
	FragColor = texture(albedo_tex, tex_coord);
}
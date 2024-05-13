#ifndef GL_MESH_H
#define GL_MESH_H
#include <render/gl_program.h>
#include <render/gl_texture.h>
#include <global.h>

class gl_mesh {
public:
	gl_mesh();
	virtual ~gl_mesh();

	void draw(gl_shader& shader)

private:
	std::vector<gl_texture> m_textures;
	std::vector<glm::vec3> m_vertexes;
	std::vector<uint32_t> m_indexes;
};

#endif
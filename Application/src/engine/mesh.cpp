#include <engine/mesh.h>

mesh::mesh(std::vector<base_vertex>& vertexes, std::vector<uint32_t>& indexes, std::vector<base_texture>& textures) {
	m_textures = textures;
	m_vertexes = vertexes;
	m_indexes = indexes;
	m_vertex_array = 0;
}

void mesh::destroy() {
	glDeleteVertexArrays(1, &m_vertex_array);
}

void mesh::init() {
	glGenVertexArrays(1, &m_vertex_array);
	m_vertex_buffer.init(GL_ARRAY_BUFFER);
	m_index_buffer.init(GL_ELEMENT_ARRAY_BUFFER);

	glBindVertexArray(m_vertex_array);

	m_vertex_buffer.bind();
	m_vertex_buffer.write(m_vertexes);

	m_index_buffer.bind();
	m_index_buffer.write(m_indexes);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(base_vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(base_vertex), (void*)offsetof(base_vertex, norm));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(base_vertex), (void*)offsetof(base_vertex, tex_coords));

	glBindVertexArray(0);
}

void mesh::draw(mat_base& material) {
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = m_textures[i].get_type();
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		material.set_integer(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].get_id());
	}
	glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(m_vertex_array);
	glDrawElements(GL_TRIANGLES, m_indexes.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
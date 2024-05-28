#include <engine/model.h>

void model::draw() const {
	for (mesh& mesh_item : m_meshes)
		mesh_item.draw(m_material);
}

void model::load_model(std::string filepath) {
	m_filepath = filepath;
}
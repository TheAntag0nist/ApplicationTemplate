#include <engine/base_texture.h>

std::string base_texture::get_path() const {
	return m_image_path;
}

std::string base_texture::get_type() const {
	return m_type;
}

uint32_t base_texture::get_id() const {
	return m_texture_id;
}
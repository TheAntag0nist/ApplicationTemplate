#include <render/gl_texture.h>

gl_texture::gl_texture() : 
	m_texture_id(0), m_is_initialized(false),
	m_width(0), m_height(0), m_channels(0) {}
gl_texture::~gl_texture() { destroy(); }

size_t gl_texture::get_height() const {
	return m_height;
}

size_t gl_texture::get_width() const {
	return m_width;
}

int gl_texture::get_channels() const {
	return m_channels;
}

uint32_t gl_texture::get_id() const {
	return m_texture_id;
}

void gl_texture::init(const char* img_path) {
	m_image_path = img_path;
	glGenTextures(1, &m_texture_id);
	glBindTexture(GL_TEXTURE_2D, m_texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* data = stbi_load(img_path, &m_width, &m_height, &m_channels, 0);
	if (data != nullptr)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "[ERR]:> Failed to load texture" << std::endl;

	stbi_image_free(data);
}

void gl_texture::destroy() {
	if (!m_is_initialized)
		return;

	glDeleteTextures(1, &m_texture_id);
	m_is_initialized = false;
	m_image_path = "";
	m_texture_id = 0;
	m_channels = 0;
	m_height = 0;
	m_width = 0;
}


void gl_texture::bind() {
	glBindTexture(GL_TEXTURE_2D, m_texture_id);
}

void gl_texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H
#define STB_IMAGE_IMPLEMENTATION
#include <engine/base_texture.h>
#include <global.h>

class gl_texture : public base_texture {
public:
	gl_texture();
	~gl_texture();

	void init();
	void init(const char* img_path);
	void destroy();

	void bind() const;
	void unbind() const;

	size_t get_height() const;
	size_t get_width() const;
	int get_channels() const;

private:
	std::string m_image_path;
	bool m_is_initialized;
	uint32_t m_texture_id;

	int m_channels;
	int m_height;
	int m_width;
};

#endif
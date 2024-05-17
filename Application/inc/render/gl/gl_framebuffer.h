#ifndef GL_FRAMEBUFFER_H
#define GL_FRAMEBUFFER_H
#include <global.h>
#include <engine/logger.h>
#include <render/gl/gl_buffer.h>
#include <render/gl/gl_texture.h>

class gl_framebuffer {
public:
	gl_framebuffer();

	void init(int width, int height);
	void destroy();

	void rescale(int width, int height);
	gl_texture& get_texture();

	int get_height() const;
	int get_Width() const;

	void unbind() const;
	void bind() const;
	
private:
	uint32_t m_render_buffer_id;
	uint32_t m_frame_buffer_id;
	gl_texture m_texture;

	int m_height;
	int m_width;
};

#endif
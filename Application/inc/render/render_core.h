#ifndef RENDER_CORE_H
#define RENDER_CORE_H
#include <render/gl/gl_framebuffer.h>
#include <render/gl/gl_program.h>
#include <render/gl/gl_texture.h>
#include <render/gl/gl_camera.h>
#include <render/gl/gl_buffer.h>
#include <file_system/fs_helper.h>
#include <global.h>

class render_core {
public:
	static render_core& get_instance();
	virtual ~render_core() = default;

	void set_width(uint32_t width);
	void set_height(uint32_t height);

	std::shared_ptr<gl_framebuffer> get_framebuffer();

	void init();
	void update();
	void destroy();

protected:
	std::shared_ptr<gl_framebuffer> m_scene_buffer;
	gl_camera global_camera;
	bool m_first = true;

	uint32_t m_height;
	uint32_t m_width;

	// test mesh
	uint32_t VAO;
	gl_texture texture_1;
	gl_buffer vertex_buff;
	gl_buffer indexes_buff;
	gl_program base_program;
	
	render_core() = default;

};

#endif
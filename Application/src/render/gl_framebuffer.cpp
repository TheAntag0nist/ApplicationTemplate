#include <render/gl_framebuffer.h>

gl_framebuffer::gl_framebuffer(): m_width(0), m_height(0) {}

void gl_framebuffer::init(int width, int height) {
	logger& log_inst = logger::get_instance();
	m_height = height;
	m_width = width;

	glGenFramebuffers(1, &m_frame_buffer_id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer_id);

	m_texture.init();
	m_texture.bind();
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL
	);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
		GL_TEXTURE_2D, m_texture.get_id(), 0
	);

	glGenRenderbuffers(1, &m_render_buffer_id);
	glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_render_buffer_id);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		log_inst.ftl("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_texture.unbind();
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void gl_framebuffer::destroy() {
	glDeleteFramebuffers(1, &m_frame_buffer_id);
	m_texture.destroy();
	glDeleteRenderbuffers(1, &m_render_buffer_id);
}

void gl_framebuffer::rescale(int width, int height) {
	m_width = width;
	m_height = height;
	auto tex_id = m_texture.get_id();
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_render_buffer_id);
	glBindTexture(GL_TEXTURE_2D, 0);
}

gl_texture& gl_framebuffer::get_texture() {
	return m_texture;
}

int gl_framebuffer::get_height() const {
	return m_height;
}

int gl_framebuffer::get_Width() const {
	return m_width;
}

void gl_framebuffer::unbind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void gl_framebuffer::bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer_id);
}
#include <render/gl_buffer.h>

gl_buffer::gl_buffer() {
	m_is_initialized = false;
	m_buffer_type = 0;
	m_buffer_obj = 0;
}

gl_buffer::~gl_buffer() {
	destroy();
}

void gl_buffer::bind() const {
	if (!m_is_initialized)
		return;
	glBindBuffer(m_buffer_type, m_buffer_obj);
}

void gl_buffer::unbind() const {
	glBindBuffer(m_buffer_type, 0);
}


void gl_buffer::init(int type) {
	m_buffer_type = type;
	m_is_initialized = true;
	glGenBuffers(1, &m_buffer_obj);
}

void gl_buffer::destroy() {
	if (!m_is_initialized)
		return;

	glDeleteBuffers(1, &m_buffer_obj);
	m_is_initialized = false;
	m_buffer_type = 0;
	m_buffer_obj = 0;
}